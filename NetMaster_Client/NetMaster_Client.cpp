// NetMaster_Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//NetMaster
//f3di006@gmail.com
//https://github.com/f3di006

#include "Globals.h"
#include"service.h"
#include"Utils.h"
#include"Net.h"
#include"Screen.h"
#include "Strings.h"
#include"UserProc.h"
using namespace Utils;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    std::string e = std::to_string(argc);
    if (argc <= 1) { goto serviceStart; }
    //Actions:

    if (!wcscmp(L"logon", argv[1])) {

        if (argc >= 3) {

            Utils::SendText(argv[2]);
        }
    }
    else if (!wcscmp(L"userproc", argv[1])) {

        UserProc::mainuser();
        return 0;
        
    }
    
    else if(!wcscmp(L"logonuser", argv[1])) {

        INPUT inputs[5];
        int i = 0;
        inputs[i].type = INPUT_KEYBOARD;
        inputs[i].ki.wVk = VK_RETURN;
        inputs[i].ki.dwFlags = 0;
        inputs[i].ki.wScan = 0;
        inputs[i].ki.time = 0;
        inputs[i].ki.dwExtraInfo = GetMessageExtraInfo();
        i++;
        inputs[i].type = INPUT_KEYBOARD;
        inputs[i].ki.wVk = VK_RETURN;
        inputs[i].ki.dwFlags = KEYEVENTF_KEYUP;
        inputs[i].ki.wScan = 0;
        inputs[i].ki.time = 0;
        inputs[i].ki.dwExtraInfo = GetMessageExtraInfo();
        i++;
        SendInput(1, inputs, sizeof(INPUT));

    }
    else if (!wcscmp(L"lock", argv[1])) {

            LockWorkStation();

        }
    else if (!wcscmp(L"screen", argv[1])) {
        int port;
        if (argc >= 4) {
            try {
                port = std::stoi(argv[3]);
            }
           catch (...) { return -2; }
           Log::LogInit();
           if (!Net::init()) { return -1; }
           char ip[50];
           size_t tmp = 0;
           wcstombs_s(&tmp,ip,sizeof(ip), argv[2], wcslen(argv[2]));
           SOCKET s = Net::connectSrv(ip, port);
           if (s == SOCKET_ERROR) {  closesocket(s); return -3; }
           
           Strings::StringsInit();
           Screen::StartScreen(s);


            
        }

    }

    return 0;



    serviceStart:
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        {(wchar_t*)SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}
    };

    if (StartServiceCtrlDispatcher(ServiceTable) == FALSE) {
        return GetLastError();
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
