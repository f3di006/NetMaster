#include"UserProc.h"

namespace UserProc {



    BOOL isIdle() {
        LASTINPUTINFO lii = { sizeof(LASTINPUTINFO) };
        if (!GetLastInputInfo(&lii)) { return FALSE; }
        ULONGLONG current = GetTickCount64();

        if (current - lii.dwTime > 30000) {
            return TRUE;
        }
        return FALSE;

    }
    std::wstring GetWindowTxt() {

        HWND hwnd = GetForegroundWindow();
        if (hwnd != NULL) {
            int length = GetWindowTextLength(hwnd);
            wchar_t* windowText = new wchar_t[length + 1];
            GetWindowText(hwnd, windowText, length + 1);
            std::wstring e(windowText);
            delete[] windowText;
            return e;
        }
        else {
            return L"";
        }


    }

	void mainuser() {
        if (!IPC::userIPCinit()) { return; }
        DWORD nusr=sizeof(IPC::pData->username)/sizeof(TCHAR);
        if (!GetUserNameW(IPC::pData->username, &nusr)) { Log::SaveLog((wchar_t*)L"Error GetUserName"); return; }

		while (1) {
            std::wstring text = GetWindowTxt();
            memset(IPC::pData->windowTitle, 0, sizeof(IPC::pData->windowTitle));
            memcpy_s(IPC::pData->windowTitle, sizeof(IPC::pData->windowTitle) -2, text.c_str(), text.length() * sizeof(wchar_t));
            if (isIdle()) {
                memcpy(&IPC::pData->idle, "1", 1);
            }
            else {
                memcpy(&IPC::pData->idle, "0", 1);
            }
            Sleep(3000);

		}



	}


}