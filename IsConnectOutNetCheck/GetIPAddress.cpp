#include "GetIPAddress.h"
#include "Winsock2.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

set<string> getIPAddress() {
	set<string> set_ip;
	int Result;
	char Name[255];
	memset(Name, 0, 255);
	Result = gethostname(Name, 255);
	if (Result == SOCKET_ERROR)
	{
		std::cout<<"gethostname failed with error ----"<< WSAGetLastError()<<std::endl;
		//return 0;
		return set_ip;
	}

	struct hostent* pHostent;
	pHostent = (struct hostent*)malloc(sizeof(struct hostent));
	pHostent = gethostbyname(Name);

	for (int i = 0;; i++)
	{

		set_ip.insert(inet_ntoa(*(IN_ADDR*)pHostent->h_addr_list[i]));
			
		if (pHostent->h_addr_list[i] + pHostent->h_length >= pHostent->h_name) {
			break;
		}				
	}
	return set_ip;

}