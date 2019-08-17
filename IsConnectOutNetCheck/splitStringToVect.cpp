//ref url:https://www.bbsmax.com/A/QW5YA6AY5m/
#include "splitStringToVect.h"
int splitStringToVect(const string& srcStr, vector<string>& destVect, const string& strFlag)
{
	int pos = srcStr.find(strFlag, 0);
	int startPos = 0;
	int splitN = pos;
	string lineText(strFlag);

	while (pos > -1)
	{
		lineText = srcStr.substr(startPos, splitN);
		startPos = pos + 1;
		pos = srcStr.find(strFlag, pos + 1);
		splitN = pos - startPos;
		destVect.push_back(lineText);
	}

	lineText = srcStr.substr(startPos, srcStr.length() - startPos);
	destVect.push_back(lineText);

	return destVect.size();
}