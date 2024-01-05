#pragma once
#include"Globals.h"
namespace Config {

	extern std::string ip;
	extern std::string url;
	extern std::string host;
	extern std::string path;
	extern DWORD Updatefreq;
	extern wchar_t currentpath[MAX_PATH];
	extern wchar_t fullname[500];
	extern char guid[150];
	extern wchar_t name[250];
	extern wchar_t username[250];
	extern unsigned short port;
	BOOL parseConfig();

}