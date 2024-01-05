#pragma once
#include"Globals.h"
#include<sddl.h>


namespace IPC {

    struct ipc_data {

        wchar_t windowTitle[1000];
        wchar_t username[250];
        char idle;
        //others to add
    };

    BOOL serviceIPCinit();
    BOOL userIPCinit();
    extern ipc_data* pData;

}