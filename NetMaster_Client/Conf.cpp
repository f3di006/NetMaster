#include"Globals.h"
#include"IPC.h"
#include"LUrlParser.h"
#include"Strings.h"
#include"Net.h"
#include <sstream>
#include <fstream>
namespace Config {

	std::string ip;
	std::string host;
	std::string url;
	std::string path;
	unsigned short port;
	DWORD Updatefreq;
	wchar_t currentpath[MAX_PATH];
	char guid[150] = { '\0' };
	wchar_t username[250]= { L'\0' };
	wchar_t name[250]= { L'\0' };
	wchar_t fullname[500]= { L'\0' };
	BOOL getGuid() {
		memset(guid, 0, sizeof(guid));
		HKEY k;
		DWORD len = sizeof(guid);
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ | KEY_WOW64_64KEY, &k) != ERROR_SUCCESS) { return -1; }
		if (RegGetValueA(k, NULL, "MachineGuid", RRF_RT_REG_SZ, NULL, (void*)guid, &len) != ERROR_SUCCESS) {
			RegCloseKey(k); Log::SaveLog((wchar_t*)L"Error Getting Machine Guid"); return FALSE;
		}

		RegCloseKey(k);
		return TRUE;

	}

	BOOL getName() {
		DWORD namen = 250;
		if (!GetComputerName(name, &namen)) { Log::SaveLog((wchar_t*)L"Error GetComputerName"); return FALSE; }
		return TRUE;

	}
	BOOL parseConfig() {
		if (!getGuid()) {
			strcpy_s(guid, "UNKNOWNID!");
		}

		if (!getName()) {
			wcscpy_s(name, L"UNKNOWN_NAME");

		}
		Updatefreq = 3000;
		
		

		std::ifstream inputFile(Strings::GetStr("config"));
		if (!inputFile.is_open()) {
			Log::SaveLog((wchar_t*)L"Error Opening config file");
			return FALSE;
		}

		
		std::string line;
		while (std::getline(inputFile, line)) {
			std::istringstream iss(line);
			std::string key, value;
			if (std::getline(iss, key, '=') && std::getline(iss, value)) {
				value.erase(0, value.find_first_not_of(" \t"));
				value.erase(value.find_last_not_of(" \t") + 1);
				if (key == "url") {
					url = value;
				}
				else if (key == "Updatefreq") {
					try {
						Updatefreq = std::stoi(value);
					}
					catch (const std::exception& e) {
						Log::SaveLog((wchar_t*)L"Error converting Updatefreq to integer , using default value");
					}
				}
			}
		}
		inputFile.close();
		if (url.empty()) { Log::SaveLog((wchar_t*)L"Error parsing url from config file"); return FALSE; }
		

		////////
		
		const auto URLparsed = LUrlParser::ParseURL::parseURL(url);

		if (URLparsed.isValid())
		{
			
			if (URLparsed.port_.empty()) {
				if (URLparsed.scheme_ == "https") { port = 443; }
				else { port = 80; }
			}
			if (!URLparsed.port_.empty()) {
				try {
					port = stoi(URLparsed.port_);
				}
				catch (...) { port = 80; }

			}
			host = URLparsed.host_;
			path = URLparsed.path_;
			

		}

		ip = Net::htoip((char*)host.c_str());
		if (ip.empty()) {
			Log::SaveLog((wchar_t*)L"Error resolving host ip");
			return FALSE;
		}

		
		if (!GetModuleFileName(nullptr, currentpath, MAX_PATH)) { 
			Log::SaveLog((wchar_t*)L"Error Getting filepath");
			return FALSE; 
		}
		return TRUE;
	}


}