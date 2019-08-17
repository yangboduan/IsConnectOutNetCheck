#pragma once
#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;
typedef struct iprange_uint
{
	unsigned int start_ip;
	unsigned int end_ip;
} iprange_uint;

typedef struct iprange_str {
	string start_ip;
	string end_ip;
} iprange_str;
void SplitString(const string& s, vector<string>& v, const string& c);
vector<iprange_uint> convert_iprangestr_to_iprangeuint(vector<iprange_str> vec_iprange_str);
bool isIPInRange(string szIP, vector<iprange_str> vec_iprange);

