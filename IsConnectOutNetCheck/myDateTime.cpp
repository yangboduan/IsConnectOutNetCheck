#define _CRT_SECURE_NO_WARNINGS

#include "myDateTime.h"
#include <ctime>
#include <sstream>
#include <iostream>
using std::ostringstream;
using std::cout;
using std::endl;


string myDateTime() {
	time_t rawtime;
	struct tm* ptminfo;
	char timeBuf[20];
	memset(timeBuf, 0, 20);
	time(&rawtime);
	ptminfo = localtime(&rawtime);
	sprintf(timeBuf, "%02d-%02d-%02d %02d:%02d:%02d",
		ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
		ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
	return timeBuf;

}