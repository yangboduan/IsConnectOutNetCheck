#pragma once
#ifndef IPHEADER
#define IPHEADER
typedef struct IpHeader
{
	unsigned char Version_HLen;
	unsigned char TOS;
	unsigned short Length;
	unsigned short Ident;
	unsigned short Flags_Offset;
	unsigned char TTL;
	unsigned char Protocol;
	unsigned short Checksum;
	unsigned int SourceAddr;
	unsigned int DestinationAddr;
} Ip_Header;
#endif