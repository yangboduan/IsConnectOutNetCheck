#include "capture_packet.h"
#include "Winsock2.h"
#include <mstcpip.h>
#include "IpHeader.h"
#include "myDateTime.h"
#include <atomic> 
#include <sstream>
#include <set>
#include "IsIPInRange.h"
#include <fstream>
using std::ofstream;
#define RECVBUFFMAXSIZE 4096
extern std::atomic<int >g_isCanRead;
extern std::atomic<int >g_ncout;
extern std::set<string> g_set_ip;
extern std::atomic<int >g_isCanRead;
extern std::atomic<int >g_capture_packet_function_num;
extern vector<iprange_str> g_vec_iprange_str;
extern ofstream g_logfile;
int capture_packet(string szIP) {
	g_capture_packet_function_num++;
	//����SOCK_RAW��socket
	SOCKET ReceiveSocket;
	//ReceiveSocket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	ReceiveSocket = WSASocket(AF_INET, SOCK_RAW, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ReceiveSocket == INVALID_SOCKET)
	{
		printf("socket failed with error %d\n", WSAGetLastError());
		g_capture_packet_function_num--;
		return 0;
	}

	//��socket
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
	//���������������ݰ�
	/*Result = ioctlsocket(ReceiveSocket, SIO_RCVALL, &InOutParam);*/
	DWORD dwBufferLen[10];
	DWORD dwBufferInLen = 1;
	DWORD dwBytesReturned = 0;
	Result = WSAIoctl(ReceiveSocket, SIO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen), &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL);
	if (Result == SOCKET_ERROR)
	{
		printf("WSAIoctl failed with error %d \n", WSAGetLastError());
		closesocket(ReceiveSocket);
		g_capture_packet_function_num--;
		return 0;
	}
	char RecvBuffer[RECVBUFFMAXSIZE];
	memset(RecvBuffer, 0, RECVBUFFMAXSIZE);

	sockaddr_in addr;
	Ip_Header* ip;
	int Len = sizeof(addr);
	int is_decrease = 0;
	int nRecvBuf = 32 * 1024;
	setsockopt(ReceiveSocket, SOL_SOCKET, SO_RCVBUF, (const char*)& nRecvBuf, sizeof(int));
	int packet_count = 0; //��¼д�뵽�ļ��а���¼�ĸ���
	int isContinue_write = 1;//�Ƿ�������ļ���д����־��Ϣ
	while (1)
	{
		
		//��������ÿ��g_ncout =2ʱ�����鱾���Ƿ񻹴���IP��ַ��
		if (g_ncout == 2 && g_isCanRead == 1) {
			auto iter = g_set_ip.find(szIP);
			
			if (iter == g_set_ip.end()) {//��������Ѿ��޴�IP��ַ�����˳��˳���ֹͣץ��
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
		Result = recvfrom(ReceiveSocket, RecvBuffer, RECVBUFFMAXSIZE, 0, (sockaddr*)& addr, &Len);
		if (Result == SOCKET_ERROR)//������ܱ��Ĵ���
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
		//printf("ip source IP: %s\t", inet_ntoa(a));
		a.s_addr = ip->DestinationAddr;
		string szDestinationAddr = inet_ntoa(a);
		//printf("ip destination IP: %s\t", inet_ntoa(a));
		//printf("protocol: %d\n", ip->Protocol);
		std::ostringstream ossString;
		string szConnectResutl ;
		if (isIPInRange(szSourceIP, g_vec_iprange_str)) {
			szConnectResutl = "Internal";
		}
		else {
			szConnectResutl = "External";
		}
		////ֻ������յ��İ�
		if (szDestinationAddr == szIP && szConnectResutl == "External") {
			if (isContinue_write == 1) {
				packet_count++;
				if (packet_count >= 5000) {//�������5000����¼���򲻼������ļ���д����־����ֹ�ļ���־������
					isContinue_write = 0;
				}
			}
			
			//ossString << "number:" << number << "\t " << "Time:" << myDateTime() << "\t" << szSourceIP << "  ------>  " << szDestinationAddr
				//<< "\tprotocol:" << int(ip->Protocol) << "\t ConnectResult:" << szConnectResutl << "\n";

			ossString << "Time:" << myDateTime() << "\t" << szSourceIP << "  ------>  " << szDestinationAddr
				<< "\tprotocol:" << int(ip->Protocol)<<std::endl  ;
			std::cout << ossString.str()<<std::endl;
			if (isContinue_write == 1) {
				g_logfile << ossString.str();
				g_logfile.flush();
			}
			
			//std::cout.flush();
		}
		
	}


	if (closesocket(ReceiveSocket) == SOCKET_ERROR)
	{
		printf("closesocket failed with error %d\n", WSAGetLastError());
		g_capture_packet_function_num--;
		return 0;
	}
}