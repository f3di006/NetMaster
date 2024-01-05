#include"IPC.h"
#include"Log.h"
namespace IPC {
    ipc_data* pData=nullptr;

	BOOL serviceIPCinit() {

        SECURITY_ATTRIBUTES attributes;
        ZeroMemory(&attributes, sizeof(attributes));
        attributes.nLength = sizeof(attributes);
        ConvertStringSecurityDescriptorToSecurityDescriptor(
            L"D:P(A;OICI;GA;;;SY)(A;OICI;GA;;;BA)(A;OICI;GWGR;;;IU)",
            SDDL_REVISION_1,
            &attributes.lpSecurityDescriptor,
            NULL);
        HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, &attributes, PAGE_READWRITE, 0, sizeof(ipc_data), L"Global\\NetMaster");

        if (hFileMapping == NULL) {
            Log::SaveLog((wchar_t*)L"error CreateFileMapping service");
            return FALSE;
        }
        pData = (ipc_data*)MapViewOfFile(hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(ipc_data));
        if (!pData) {
            Log::SaveLog((wchar_t*)L"error MapViewOfFile service");
            return FALSE;
        }
        memset(pData, 0, sizeof(ipc_data));
        return TRUE;
	}

    BOOL userIPCinit() {

        HANDLE hFileMapping = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, L"Global\\NetMaster");

        if (hFileMapping == NULL) {
            Log::SaveLog((wchar_t*)L"error OpenFileMapping user");
            return FALSE;
        }
        pData = (ipc_data*)MapViewOfFile(hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(ipc_data));

        if (pData == NULL) {
            Log::SaveLog((wchar_t*)L"error MapViewOfFile user");
            return FALSE;
        }

        return TRUE;
    }


}


