#pragma once
#include"Globals.h"
namespace Net {

	BOOL init();
	SOCKET connectSrv(char* ip, unsigned short port);
	std::string htoip(char* host);
}