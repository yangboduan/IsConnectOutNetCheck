#pragma once
#pragma once
#include <string>
#include <vector>
#include "struct_iprange_str.h"
#include "struct_iprange_unit.h"
using std::string;
using std::vector;


void SplitString(const string& s, vector<string>& v, const string& c);
vector<iprange_uint> convert_iprangestr_to_iprangeuint(vector<iprange_str> vec_iprange_str);
bool isIPInRange(string szIP, vector<iprange_str> vec_iprange);

