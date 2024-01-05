#pragma once
#include"Globals.h"
class command
{

private:
	std::string commandline;
	std::string commandarg;
	int commandtype = -1;
	void parse_command();
	void LaunchCommand();
public:
	command(std::string commandline);
	~command();



};