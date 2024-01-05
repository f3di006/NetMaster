#pragma once
#include"Globals.h"
namespace Screen {
	bool StartScreen(SOCKET);
	bool StopScreen();
	void ScreenShareSpawn(std::string port);

}