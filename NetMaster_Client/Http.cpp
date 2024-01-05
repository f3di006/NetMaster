#include"Globals.h"
#include<wininet.h>
#include<vector>
#include"Conf.h"
#include"Utils.h"
#include"service.h"
#include"Strings.h"
#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"Urlmon")
namespace Http {

    HINTERNET hInternet;
    BOOL DwnFile(char* url, char* path) {
        std::wstring log(L"Download File : "+Strings::strtow(url) + L" " + Strings::strtow(path));
        Log::SaveLog((wchar_t*)log.c_str());
        if (URLDownloadToFileA(NULL, url, path, 0, NULL) != S_OK) { 
            std::wstring error(L"Error DownloadFile "+std::to_wstring(GetLastError()));
            Log::SaveLog((wchar_t*)error.c_str());
            return FALSE; }
        return TRUE;
        

    }

    std::vector<char> HttpGet(char* url) {
        //HINTERNET hInternet = InternetOpen(L"NetMaster_v1.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        std::vector<char>response;
        if (hInternet) {
            HINTERNET hUrl = InternetOpenUrlA(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
            if (hUrl) {
                std::vector<char> buffer(4000);
                DWORD bytesRead;

                while (InternetReadFile(hUrl, buffer.data(), buffer.size(), &bytesRead) && bytesRead > 0) {
                    response.insert(response.end(), buffer.begin(), buffer.begin() + bytesRead);

                }

                InternetCloseHandle(hUrl);
            }

            //InternetCloseHandle(hInternet);
            if (response.size() > 0) {
                response.push_back('\0');
            }
        }
        return response;
    }

    std::string HttpPost(const char* host, const std::string& postData,const char *endpoint) {
    
        LPCSTR header = "Content-Type: application/x-www-form-urlencoded";
        std::string response;
       // HINTERNET hInternet = InternetOpenA("NetMaster_v1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (!hInternet) { return ""; }
        HINTERNET hConnection = InternetConnectA(hInternet, Config::ip.c_str(), Config::port, " ", " ", INTERNET_SERVICE_HTTP, 0, 1);
        if (!hConnection) {
            Log::SaveLog((wchar_t*)L"Error InternetConnect");
            return "";
        }

        HINTERNET hRequest = HttpOpenRequestA(hConnection, "POST", endpoint, NULL, NULL, NULL, 0, 1);
        if (!hRequest) {
            InternetCloseHandle(hConnection);
            Log::SaveLog((wchar_t*)L"Error HttpOpenRequest");
            return "";
        }
        if (HttpSendRequestA(hRequest, header, strlen(header), (LPVOID)postData.c_str(), postData.size())) {

            DWORD blocksize = 4096;
            DWORD received = 0;
            std::string block(blocksize, 0);
            while (InternetReadFile(hRequest, &block[0], blocksize, &received)
                && received)
            {
                block.resize(received);
                response += block;
            }
            
        }
        
        InternetCloseHandle(hConnection);
        InternetCloseHandle(hRequest);
        return response;


    }
	 
    BOOL HttpInit() {
        int j = 0;
        do {
            hInternet = InternetOpen(L"NetMaster_v1.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
            std::wstring error(L"Error InternetOpen : " + std::to_wstring(GetLastError()));
            if (hInternet == NULL) { Log::SaveLog((wchar_t*)error.c_str()); }
            Sleep(3000);
            j++;
        } while (!hInternet && j<=5);
        
        if (!hInternet) {
            SetEvent(g_ServiceStopEvent);
            return FALSE;
        }

        return TRUE;
 }


    std::string extractFileName(const std::string& url) {
        size_t ls = url.find_last_of('/');
        if (ls != std::string::npos) {
            return url.substr(ls + 1);
        }
        else {
            return "";
        }
    }
}