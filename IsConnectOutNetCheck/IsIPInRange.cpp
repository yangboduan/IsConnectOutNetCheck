#include "IsIPInRange.h"
#include <iostream>
using std::cout;
using std::endl;


//SplitString ref Url:https://www.cnblogs.com/yuehouse/p/10230589.html
//分割字符串函数 
//s:需被分割的字符串  v:存储分割后的字符串  c:分隔符
void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

//将内涵string 类型的IP地址的vector 类型转换成unsigned 的vector类型。
vector<iprange_uint> convert_iprangestr_to_iprangeuint(vector<iprange_str> vec_iprange_str) {
	vector<iprange_uint> vec_iprange_uint;

	for (auto it = vec_iprange_str.begin(); it != vec_iprange_str.end(); it++) {
		vector<string> v1;
		SplitString(it->start_ip, v1, "."); //可按多个字符来分隔;
		string tmpstr = v1[0] + v1[1] + v1[2] + v1[3];
		unsigned int uip_start = stoi(v1[0], 0, 10) * 256 * 256 * 256 + stoi(v1[1], 0, 10) * 256 * 256
			+ stoi(v1[2], 0, 10) * 256 + stoi(v1[3], 0, 10);
		vector<string> v2;
		SplitString(it->end_ip, v2, ".");
		string tmpstr2 = v2[0] + v2[1] + v2[2] + v2[3];

		unsigned int uip_end = stoi(v2[0], 0, 10) * 256 * 256 * 256 + stoi(v2[1], 0, 10) * 256 * 256
			+ stoi(v2[2], 0, 10) * 256 + stoi(v2[3], 0, 10);

		iprange_uint iprange_uintobj;
		iprange_uintobj.start_ip = uip_start;
		iprange_uintobj.end_ip = uip_end;
		vec_iprange_uint.push_back(iprange_uintobj);


	}

	return vec_iprange_uint;
}


//检测IP地址是否在指定的IP地址范围内；在范围内范围true，否则范围false；
//szIP :需检测的IP 
//vec_iprange_str :vector 类型的IP范围;
bool isIPInRange(string szIP, vector<iprange_str> vec_iprange_str) {
	vector<string> v;
	SplitString(szIP, v, ".");
	string tmpstr = v[0] + v[1] + v[2] + v[3];
	unsigned int uip = stoi(v[0], 0, 10) * 256 * 256 * 256 + stoi(v[1], 0, 10) * 256 * 256
		+ stoi(v[2], 0, 10) * 256 + stoi(v[3], 0, 10);

	vector<iprange_uint>  vec_iprange_uintobj;

	vec_iprange_uintobj = convert_iprangestr_to_iprangeuint(vec_iprange_str);

	for (auto it = vec_iprange_uintobj.begin(); it != vec_iprange_uintobj.end(); it++) {
		if (uip >= it->start_ip && uip <= it->end_ip) {
			return true;
		}
	}

	return false;
}