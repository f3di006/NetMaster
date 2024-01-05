#include"Globals.h"
#include<WS2tcpip.h>
#include"Strings.h"
namespace Net {

	BOOL init() {

		WSAData d;
		if (WSAStartup(MAKEWORD(2, 0), &d)) {
			std::wstring error(L" Error WSAStartup : " + std::to_wstring(GetLastError()));
			Log::SaveLog((wchar_t*)error.c_str());
			return FALSE; }

		return TRUE;


	}
	SOCKET connectSrv(char* ip, unsigned short port) {
		SOCKADDR_IN inf;
		memset(&inf, 0, sizeof(inf));
		inf.sin_family = AF_INET;
		inf.sin_port = htons(port);
		if (inet_pton(AF_INET, ip, &inf.sin_addr.s_addr) != 1) { return SOCKET_ERROR; }
		SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
		if (s == SOCKET_ERROR) { return s; }
		if (connect(s, (sockaddr*)&inf, sizeof(inf)) != SOCKET_ERROR) { return s; }
		closesocket(s);
		std::wstring wip = Strings::strtow(ip);
		std::wstring error(L"Error connecting to server : " + std::wstring(wip) + L":" + std::to_wstring(port));
		Log::SaveLog((wchar_t*)error.c_str());
		return SOCKET_ERROR;




	}

	std::string htoip(char* host) {
		PADDRINFOA r;
		ADDRINFO hints = { 0 };
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		if (getaddrinfo(host, NULL, &hints, &r)) { return ""; }
		sockaddr_in* ipv4 = (sockaddr_in*)r->ai_addr;
		char ip[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
		return ip;

	}



}