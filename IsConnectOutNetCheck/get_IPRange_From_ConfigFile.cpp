#include "get_IPRange_From_ConfigFile.h"
#include "getconfigcontent.h"
#include "splitStringToVect.h"
#include "IsIPInRange.h"

extern vector<iprange_str> g_vec_iprange_str;


//去掉字符串所有空格
string& ClearAllSpace(string& str)
{
	int index = 0;
	if (!str.empty())
	{
		while ((index = str.find(' ', index)) != string::npos)
		{
			str.erase(index, 1);
		}
	}

	return str;
}

vector<iprange_str> get_IPRange_From_ConfigFile(string szConfigFileName, string szAppName, string szKeyName) {
	vector<string>   destVect;//第一次分割后的字符存储在此vector变量中

	//从配置文件中获取IP地址范围
	string szIP_range = getconfigcontent(szConfigFileName,szAppName,szKeyName);

	
	if (szIP_range == "DefaultValue") {
		std::cerr << "请检查配置文件中是否存在 APPName:" << szAppName << " 和 KeyName" << szKeyName << endl;
		exit(-1);
	}

	ClearAllSpace(szIP_range);//去掉szIP_range变量的所有空格
	if (szIP_range.length() == 0) {
		std::cout << "配置文件中的" << szKeyName << "不能为空" << std::endl;
		exit(-1);
	}

	splitStringToVect(szIP_range, destVect, ";");
	iprange_str iprange_strobj;

	vector<iprange_str> tmp_vec_iprange_str;
	for (auto iter = destVect.begin(); iter != destVect.end(); iter++) {
		vector<string>   ip_src_dest_Vect;//第二次分割后的字符存储在此vecotr变量中
		if (*iter == "") {//防止将空字符往vecotr压，造成后面程序运行错误，报out of range
			break;
		}
		cout << "ip:" << *iter << endl;
		
		splitStringToVect(*iter, ip_src_dest_Vect, "-");
		int i = 1;
		for (auto iter2 = ip_src_dest_Vect.begin(); iter2 != ip_src_dest_Vect.end(); iter2++) {
			
			if (i == 1) {
				cout << "strart_IP:" << *iter2 << endl;
				iprange_strobj.start_ip = *iter2;
			}
			else {
				cout << "end_IP:" << *iter2 << endl;
				iprange_strobj.end_ip = *iter2;
			}
			i++;
		}
		tmp_vec_iprange_str.push_back(iprange_strobj);
		//g_vec_iprange_str.push_back(iprange_strobj);
	}
	return tmp_vec_iprange_str;
}