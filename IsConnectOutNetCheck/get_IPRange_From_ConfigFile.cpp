#include "get_IPRange_From_ConfigFile.h"
#include "getconfigcontent.h"
#include "splitStringToVect.h"
#include "IsIPInRange.h"
extern vector<iprange_str> g_vec_iprange_str;

int get_IPRange_From_ConfigFile(string szConfigFilename) {
	vector<string>   destVect;
	string szIP_range = getconfigcontent(szConfigFilename.c_str());
	splitStringToVect(szIP_range, destVect, ";");
	iprange_str iprange_strobj;
	for (auto iter = destVect.begin(); iter != destVect.end(); iter++) {
		vector<string>   ip_src_dest_Vect;
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
		g_vec_iprange_str.push_back(iprange_strobj);
	}
	return 0;
}