#pragma once
#include"Globals.h"
#include <map>
#include <vector>
namespace Strings {


	extern std::map<std::string, std::vector<unsigned char>> strings;
	void StringsInit();
	std::string GetStr(const char*);
	std::vector<unsigned char> GetVect(const char*);
	std::wstring strtow(const std::string&);
	std::string wtostr(const std::wstring&);
}