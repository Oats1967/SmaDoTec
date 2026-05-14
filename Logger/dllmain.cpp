// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "pch.h"
#include "OutputDebug.h"


BOOL APIENTRY DllMain( HMODULE ,
                       DWORD  ul_reason_for_call,
                       LPVOID 
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        OpenDebug();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        CloseDebug();
        break;
    }
    return TRUE;
}

