#pragma once
#include<WinSock2.h>
#include<netioapi.h>
#include<windows.h>
#include <iphlpapi.h>
#include<WS2tcpip.h>
#include<iostream>
#include<string>
#include"Log.h"
#include<mutex>
#include<vector>
#pragma comment(lib,"ws2_32")
#pragma comment(lib, "iphlpapi.lib")
#define SERVICE_NAME L"NetMaster"
void _stdcall _Main();
extern std::vector<HANDLE> processes;