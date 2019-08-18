#include "get_IPRange_From_ConfigFile.h"
#include "getconfigcontent.h"
#include "splitStringToVect.h"
#include "IsIPInRange.h"

extern vector<iprange_str> g_vec_iprange_str;


//ȥ���ַ������пո�
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
	vector<string>   destVect;//��һ�ηָ����ַ��洢�ڴ�vector������

	//�������ļ��л�ȡIP��ַ��Χ
	string szIP_range = getconfigcontent(szConfigFileName,szAppName,szKeyName);

	
	if (szIP_range == "DefaultValue") {
		std::cerr << "���������ļ����Ƿ���� APPName:" << szAppName << " �� KeyName" << szKeyName << endl;
		exit(-1);
	}

	ClearAllSpace(szIP_range);//ȥ��szIP_range���������пո�
	if (szIP_range.length() == 0) {
		std::cout << "�����ļ��е�" << szKeyName << "����Ϊ��" << std::endl;
		exit(-1);
	}

	splitStringToVect(szIP_range, destVect, ";");
	iprange_str iprange_strobj;

	vector<iprange_str> tmp_vec_iprange_str;
	for (auto iter = destVect.begin(); iter != destVect.end(); iter++) {
		vector<string>   ip_src_dest_Vect;//�ڶ��ηָ����ַ��洢�ڴ�vecotr������
		if (*iter == "") {//��ֹ�����ַ���vecotrѹ����ɺ���������д��󣬱�out of range
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