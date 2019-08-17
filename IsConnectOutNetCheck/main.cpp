//ref url::https://bbs.csdn.net/topics/380112724
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "stdio.h"
#include "Winsock2.h"
#include "mstcpip.h"
#include <thread> 
#include <iostream>
#include "GetIPAddress.h"
#include "IpHeader.h"
#include "capture_packet.h"
#include "ThreadPool.h"
#include "sstream"

#include "get_IPRange_From_ConfigFile.h"
#include <vector>
using std::vector;
#include "IsIPInRange.h"

#define THREAD_NUM 6  //线程池中的线程数量
#pragma comment( lib, "ws2_32.lib")
std::atomic<int >g_isCanRead = 1; //是否可以读 g_set_ip变量
std::atomic<int >g_ncout = 1; //计数器
set<string> g_set_ip; //存储本机的IP地址
std::atomic<int >g_capture_packet_function_num = 0;//当前抓包函数capture_packet运行的个数；
vector<iprange_str> g_vec_iprange_str;
int main(int argc, char* argv[])
{
	
	get_IPRange_From_ConfigFile("C:\\config.ini");
	Ip_Header* ip;
	int Timeout = 1000;
	SOCKET ReceiveSocket;
	char RecvBuffer[1502];
	sockaddr_in addr;
	int Len = sizeof(addr);
	int Result;
	WSADATA wsaData;
	if ((Result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		printf("WSAStartup failed with error %d\n", Result);
		return 0;
	}
	//创建线程池，线程池种的数量
	ThreadPool g_ThreadPool(THREAD_NUM);

	set<string> current_set_ip,last_set_ip;
	current_set_ip = getIPAddress();
	g_set_ip = current_set_ip ;
	last_set_ip = current_set_ip;

	for (auto iter = g_set_ip.begin(); iter != g_set_ip.end(); iter++) {
		std::cout << "ip:" << *iter << std::endl;
		g_ThreadPool.enqueue(capture_packet, *iter);
		
	}

	//周期性轮询本地IP地址；如果有IP地址增加，则增加抓包分析线程
	
	while (1) {
		std::stringstream ossTmp;
		g_ncout++;
		Sleep(1 * 1000);
		current_set_ip = getIPAddress();
		if (current_set_ip != last_set_ip) {
			for (auto iter = current_set_ip.begin(); iter != current_set_ip.end(); iter++) {
				if (last_set_ip.find(*iter) == last_set_ip.end()) {
					g_ThreadPool.enqueue(capture_packet, *iter);
				}
			}
			g_isCanRead = 0;
			g_set_ip = current_set_ip;
			last_set_ip = current_set_ip;
			g_isCanRead = 1;
			
		}
		/*for (auto iter = g_set_ip.begin(); iter != g_set_ip.end(); iter++) {
			std::cout << "setIP:\t" << *iter << std::endl;
		}*/
		if (g_ncout == 3) {
			g_ncout = 1;
		}

		//ossTmp << "==================运行抓包程序的个数:" << g_capture_packet_function_num <<"=============\n";
		std::cout << ossTmp.str();
		std::cout.flush();

	}
	
	

	if (WSACleanup() == SOCKET_ERROR)
	{
		printf("WSACleanup failed with error %d\n", WSAGetLastError());
		return 0;
	}
	return 1;
}