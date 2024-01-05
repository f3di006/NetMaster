#include"Globals.h"
#include"Log.h"
#include"Conf.h"
#include<Pdh.h>
#include<WtsApi32.h>
#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib,"Pdh.lib")
namespace Mon {
    void initCpuMon();
	ULONGLONG lastsent = 0;
	ULONGLONG lastrecv = 0;
    ULONG interfaceIndex = 0;
    DWORD result;
    ///
    PDH_HQUERY cpuq;
    PDH_HCOUNTER cpuload;
    BOOL cpuMon = FALSE;

	double bytesToMegabytes(unsigned long long bytes) {
		return static_cast<double>(bytes) / (1024.0 * 1024.0);
	}
    double bytesToMegabits(unsigned long long bytes) {
        unsigned long long bits = bytes * 8;
        double megabits = static_cast<double>(bits) / 1000000.0;
        return megabits;
    }

    void initMon() {
        sockaddr_in destination;
        destination.sin_family = AF_INET;
        if (inet_pton(AF_INET, "8.8.8.8", &destination.sin_addr.s_addr) != 1) {
            Log::SaveLog((wchar_t*)L"Monitor inet_pton Failed\n");
            return;

        }
        result = GetBestInterfaceEx((sockaddr*)&destination, &interfaceIndex);
        initCpuMon();

    }


    void CheckBandwidth(double &mbrecv,double &mbsent) {
        if (result != NO_ERROR) { return; }
        MIB_IF_ROW2 ifRow = { 0 };
        ifRow.InterfaceIndex = interfaceIndex;

        if (GetIfEntry2(&ifRow) == NO_ERROR) {

            ULONGLONG currentsent = (ifRow.OutOctets - lastsent)/(Config::Updatefreq/1000);
            ULONGLONG currentrec = (ifRow.InOctets - lastrecv)/(Config::Updatefreq/1000);
            mbsent = bytesToMegabits(currentsent);
            mbrecv = bytesToMegabits(currentrec);
            lastsent = ifRow.OutOctets;
            lastrecv = ifRow.InOctets;

        }
        else {
            Log::SaveLog((wchar_t*)L"GetIfEntry2 Error");
        }
        
    }
    
    void initCpuMon() {
        if (PdhOpenQuery(NULL, NULL, &cpuq) != ERROR_SUCCESS) {
            Log::SaveLog((wchar_t*)L"PdhOpenQuery Error");
            return;
        }
        if (PdhAddEnglishCounter(cpuq, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuload) != ERROR_SUCCESS) {
            Log::SaveLog((wchar_t*)L"PdhAddEnglishCounter Error");
            return;
        }
        if (PdhCollectQueryData(cpuq) != ERROR_SUCCESS) {
            Log::SaveLog((wchar_t*)L"PdhCollectQueryData Error");
            return;
        }
        cpuMon = TRUE;
    }

    double CpuCurrentLoad() {
        if (!cpuMon) { return 0; }
        PDH_FMT_COUNTERVALUE counterVal;

        if (PdhCollectQueryData(cpuq) != ERROR_SUCCESS) {
            Log::SaveLog((wchar_t*)L"PdhCollectQueryData Error");
            return 0;
        }
        if (PdhGetFormattedCounterValue(cpuload, PDH_FMT_DOUBLE, NULL, &counterVal)!=ERROR_SUCCESS) {
            Log::SaveLog((wchar_t*)L"PdhGetFormattedCounterValue Error");
            return 0;

        }
        return counterVal.doubleValue;
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

    bool IsSessionLocked()
    {

        WTSINFOEXW* pInfo = NULL;
        WTS_INFO_CLASS wtsic = WTSSessionInfoEx;
        bool bRet = false;
        LPTSTR ppBuffer = NULL;
        DWORD dwBytesReturned = 0;
        LONG dwFlags = 0;

        DWORD dwSessionID = WTSGetActiveConsoleSessionId();
        if (WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, dwSessionID, wtsic, &ppBuffer, &dwBytesReturned))
        {
            if (dwBytesReturned > 0)
            {
                pInfo = (WTSINFOEXW*)ppBuffer;
                if (pInfo->Level == 1)
                {
                    dwFlags = pInfo->Data.WTSInfoExLevel1.SessionFlags;
                }
                if (dwFlags == WTS_SESSIONSTATE_LOCK)
                {
                    bRet = true;
                }
            }
            WTSFreeMemory(ppBuffer);
            ppBuffer = NULL;
        }



        return bRet;
    }
}