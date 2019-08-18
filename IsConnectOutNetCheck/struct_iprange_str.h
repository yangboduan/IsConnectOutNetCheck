#pragma once
#ifndef _IPRANGE_STR
#define _IPRANGE_STR
#include <string>
using std::string;
typedef struct iprange_str {
	string start_ip;
	string end_ip;
} iprange_str;
#endif