#pragma once
#include <iostream>
#include <vector>
#include "struct_iprange_str.h"

using std::string;
using std::cout;
using std::endl;

std::vector<iprange_str> get_IPRange_From_ConfigFile(string szConfigFileName, string szAppName, string szKeyName);
