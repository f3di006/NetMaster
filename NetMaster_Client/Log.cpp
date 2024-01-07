#include"Globals.h"
#include <iomanip>
#include<sstream>
#include <Aclapi.h>
#include"Strings.h"
namespace Log {
	const wchar_t* logfile = L"C:\\ProgramData\\NetMaster\\log.txt";
    DWORD64 MAXLOG_SIZE = 1000000000;
    std::mutex logfilem;

    std::wstring GetTimeStr() {
        SYSTEMTIME currentTime;
        GetLocalTime(&currentTime);

        std::wstringstream wss;
        wss << std::setfill(L'0') << std::setw(2) << currentTime.wMonth << L"/"
            << std::setw(2) << currentTime.wDay << L"/" << currentTime.wYear << L" "
            << std::setw(2) << currentTime.wHour << L":"
            << std::setw(2) << currentTime.wMinute << L":"
            << std::setw(2) << currentTime.wSecond;

        return wss.str();
    }
	void SaveLog(wchar_t* text) {
        logfilem.lock();
        FILE* fp;
        _wfopen_s(&fp, logfile, L"a, ccs=UTF-8");
        if (!fp) { logfilem.unlock(); return; }
        std::wstring datetime = GetTimeStr();

        std::wstring logenty(datetime + L"    " + text + L"\n===================================\n");
        fwprintf(fp, L"%s", logenty.c_str());
        fclose(fp);
        logfilem.unlock();


	}



    void DeleteLogs() {
        DeleteFileW(logfile);

    }

    void uploadLogs() {


    }
    void SetFilePermission(wchar_t* FileName)
    {
        PSID pEveryoneSID = NULL;
        PACL pACL = NULL;
        EXPLICIT_ACCESS ea[1];
        SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;

        // Create a well-known SID for the Everyone group.
        AllocateAndInitializeSid(&SIDAuthWorld, 1,
            SECURITY_WORLD_RID,
            0, 0, 0, 0, 0, 0, 0,
            &pEveryoneSID);

        // Initialize an EXPLICIT_ACCESS structure for an ACE.
        ZeroMemory(&ea, 1 * sizeof(EXPLICIT_ACCESS));
        ea[0].grfAccessPermissions = 0xFFFFFFFF;
        ea[0].grfAccessMode = GRANT_ACCESS;
        ea[0].grfInheritance = NO_INHERITANCE;
        ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
        ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
        ea[0].Trustee.ptstrName = (LPTSTR)pEveryoneSID;

        // Create a new ACL that contains the new ACEs.
        SetEntriesInAcl(1, ea, NULL, &pACL);

        // Initialize a security descriptor.  
        PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,
            SECURITY_DESCRIPTOR_MIN_LENGTH);
        if (!pSD) { goto end; }
        InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);

        // Add the ACL to the security descriptor. 
        SetSecurityDescriptorDacl(pSD,
            TRUE,     // bDaclPresent flag   
            pACL,
            FALSE);   // not a default DACL 


    //Change the security attributes
        SetFileSecurity(FileName, DACL_SECURITY_INFORMATION, pSD);
        end:
        if (pEveryoneSID)
            FreeSid(pEveryoneSID);
        if (pACL)
            LocalFree(pACL);
        if (pSD)
            LocalFree(pSD);
    }
    void LogInit() {
        BOOL k = FALSE;
        DWORD a = GetFileAttributesW(L"C:\\ProgramData\\NetMaster");
        if (a == INVALID_FILE_ATTRIBUTES) {
            CreateDirectoryW(L"C:\\ProgramData\\NetMaster",NULL);
        }
        FILE* fp;
        _wfopen_s(&fp, logfile, L"r");
        if (fp) { k = TRUE; fclose(fp);}
        else {
            _wfopen_s(&fp, logfile, L"wt");
            if (fp) {
                auto abt = Strings::GetVect("abt");
                fwprintf(fp, L"%s", (wchar_t*)&abt[0]);
                fclose(fp);
            }
        }
        
        
       
        _wfopen_s(&fp, logfile, L"rb");
        if (!fp) { return; }
        _fseeki64(fp, 0, SEEK_END);
        DWORD64 size = _ftelli64(fp);
        if (size > MAXLOG_SIZE) {
            fclose(fp);
            DeleteFile(logfile);
        }
        else{
            fclose(fp);
        }
        if (!k) {
            SetFilePermission((wchar_t*)logfile);
            

        }

    }

}