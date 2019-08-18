#include <iostream>
#include <sstream>
#include <fstream>
#include "getconfigcontent.h"

using std::ostringstream;
using std::string;
using std::ios;
using std::fstream;

#define MAX_LENGTH 500
string getconfigcontent(string szConfigFileName,string szAppName, string szKeyName) {

	//����ļ��Ƿ����
	fstream _file;
	_file.open(szConfigFileName, ios::in);
	if (!_file)
	{
		std::cerr << szConfigFileName << " �ļ�������" << std::endl;
		exit(-1);
	}
	
	char SName[MAX_LENGTH];

	//ȥ�����ļ���
	//"C:\\config.ini"        �� ".\\student.ini"
	//GetPrivateProfileString((LPCTSTR)"isConnExternalNetCheck", (LPCTSTR)"IPRange", (LPCTSTR)"DefaultName", (LPTSTR)SName, MAX_LENGTH, (LPCTSTR)szConfigFileName.c_str());
	GetPrivateProfileString( (LPCTSTR)szAppName.c_str(), (LPTSTR)szKeyName.c_str(), (LPCTSTR)"DefaultValue", (LPTSTR)SName, MAX_LENGTH, (LPCTSTR)szConfigFileName.c_str());
	//std::cout << SName << std::endl;
	string strvalue;
	ostringstream ossvalue;
	ossvalue << SName;
	
	return ossvalue.str();

}