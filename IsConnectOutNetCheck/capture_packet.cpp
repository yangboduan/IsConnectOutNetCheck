#include "capture_packet.h"
#include "Winsock2.h"
#include <mstcpip.h>
#include "IpHeader.h"
#include "myDateTime.h"
#include <atomic> 
#include <sstream>
#include <set>

extern std::atomic<int >g_isCanRead;
extern std::atomic<int >g_ncout;
extern std::set<string> g_set_ip;
extern std::atomic<int >g_isCanRead;
extern std::atomic<int >g_capture_packet_function_num;
int capture_packet(string szIP) {
	g_capture_packet_function_num++;
	//创建SOCK_RAW的socket
	SOCKET ReceiveSocket;
	ReceiveSocket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (ReceiveSocket == INVALID_SOCKET)
	{
		printf("socket failed with error %d\n", WSAGetLastError());
		g_capture_packet_function_num--;
		return 0;
	}

	//绑定socket
	SOCKADDR_IN sock;
	sock.sin_family = AF_INET;
	sock.sin_port = htons(5555);
	sock.sin_addr.s_addr = inet_addr(szIP.c_str());
	int Result;
	Result = bind(ReceiveSocket, (PSOCKADDR)& sock, sizeof(sock));
	if (Result == SOCKET_ERROR)
	{
		printf("bind failed with error %d\n", WSAGetLastError());
		closesocket(ReceiveSocket);
		g_capture_packet_function_num--;
		return 0;
	}

	


	u_long InOutParam = 1;
	//接收网络所有数据包
	Result = ioctlsocket(ReceiveSocket, SIO_RCVALL, &InOutParam);
	if (Result == SOCKET_ERROR)
	{
		printf("WSAIoctl failed with error %d \n", WSAGetLastError());
		closesocket(ReceiveSocket);
		g_capture_packet_function_num--;
		return 0;
	}
	char RecvBuffer[1502];
	memset(RecvBuffer, 0, 1502);

	sockaddr_in addr;
	Ip_Header* ip;
	int Len = sizeof(addr);
	int is_decrease = 0;
	while (1)
	{
		
		//计算器，每当g_ncout =2时，则检查本机是否还存在IP地址；
		if (g_ncout == 2 and g_isCanRead == 1) {
			auto iter = g_set_ip.find(szIP);
			
			if (iter == g_set_ip.end()) {//如果本机已经无此IP地址，则退出此程序，停止抓包
				if (closesocket(ReceiveSocket) == SOCKET_ERROR)
				{
					printf("closesocket failed with error %d\n", WSAGetLastError());
					
				}
				
				g_capture_packet_function_num--;
				return 0;
				
			}

		}
		//std::cout << "gncout:" << g_ncout << "\tg_isCanRead:" << g_isCanRead << std::endl;

		memset(RecvBuffer, 0, sizeof(RecvBuffer));
		memset(&addr, 0, sizeof(addr));
		Result = recvfrom(ReceiveSocket, RecvBuffer, 1502, 0, (sockaddr*)& addr, &Len);
		if (Result == SOCKET_ERROR)//如果接受报文错误
		{
			printf("recvfrom failed with error %d \n", WSAGetLastError());
			closesocket(ReceiveSocket);
			g_capture_packet_function_num--;
			return 0;
		}
		
		static int number = 0;
		number++;
		
		ip = (Ip_Header*)RecvBuffer;
		struct in_addr a;
		a.s_addr = ip->SourceAddr;
		string szSourceIP = inet_ntoa(a);
		
		a.s_addr = ip->DestinationAddr;
		string szDestinationAddr = inet_ntoa(a);
		std::ostringstream ossString;
		ossString <<"number:"<<number<<"\t "<<"Time:"<< myDateTime()<<"\t"<<szSourceIP << "  ------>  " << szDestinationAddr 
			<< "\tprotocol:"<< int(ip->Protocol)<<"\n";
		std::cout << ossString.str() ;
		std::cout.flush();
	}


	if (closesocket(ReceiveSocket) == SOCKET_ERROR)
	{
		printf("closesocket failed with error %d\n", WSAGetLastError());
		g_capture_packet_function_num--;
		return 0;
	}
}