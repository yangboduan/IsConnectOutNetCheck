#include <iostream>
#include <sstream>
#include "getconfigcontent.h"

using std::ostringstream;
using std::string;

#define MAX_LENGTH 500
string getconfigcontent(string szConfigFileName) {
	char SName[MAX_LENGTH];

	//"C:\\config.ini"
	GetPrivateProfileString((LPCTSTR)"isConnExternalNetCheck", (LPCTSTR)"IPRange", (LPCTSTR)"DefaultName", (LPTSTR)SName, MAX_LENGTH, (LPCTSTR)szConfigFileName.c_str());
	std::cout << SName << std::endl;
	string strvalue;
	ostringstream ossvalue;
	ossvalue << SName;
	return ossvalue.str();

}