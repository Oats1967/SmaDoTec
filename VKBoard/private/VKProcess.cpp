//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module VKProcess
///
/// @file   VKProcess.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <Windows.h>
#include <cassert>
#include <tlhelp32.h>
#include <atlstr.h>
#include "VKProcess.h"



static void ExtractFilename(LPCSTR sz, LPSTR path, const size_t pathLength, LPSTR filename, const size_t filenameLength)
{
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath_s(LPCSTR(sz), drive, dir, fname, ext);
    sprintf_s(path, pathLength, "%s\\%s\\", drive, dir);
    sprintf_s(filename, filenameLength, "%s%s", fname, ext);
}
//***************************************************************************************************************************
//***************************************************************************************************************************
static void startup(const std::string& filename)
{
    assert(!filename.empty());
    TCHAR wFile[1024];
    _tcscpy_s(wFile, filename.c_str());
    STARTUPINFO start_info;
    PROCESS_INFORMATION process_info;
    BOOL success = FALSE;
    for ( uint32_t retry = 0; retry < 3U; retry++)
    {
        ZeroMemory(&start_info, sizeof(STARTUPINFO));
        ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));
        auto result = CreateProcess(NULL, (LPSTR)wFile, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &start_info, &process_info);
        if (result != 0)
        {
            success = TRUE;
            break;
        }
        Sleep(2000);
    }
    if (success)
    {
        (void)WaitForSingleObject(process_info.hProcess, 3000);
    }
    CloseHandle(process_info.hThread);
    CloseHandle(process_info.hProcess);    // additional information
}
//***************************************************************************************************************************
//***************************************************************************************************************************
void VKProcessRunning(const std::string& filename)
{
    PROCESSENTRY32 pe32 = { 0 };

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe32.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hSnap, &pe32);     // Can throw away, never an actual app

    TCHAR wFile[1024];
    char cFile[256];
    {
        char path[512];
        ExtractFilename(filename.c_str(), path, sizeof(path), cFile, sizeof(cFile));
        _tcscpy_s(wFile, cFile);
    }

    BOOL bProcessFound  = FALSE;   //init values
    BOOL bNext          = FALSE;
    do
    {
        bNext = Process32Next(hSnap, &pe32);
        if (strcmp(pe32.szExeFile, wFile) == 0)    // Did we find our process?
        {
            bProcessFound = TRUE;
            break;
        }
    } 
    while (bNext);

    if (bProcessFound)    // if we didn't find it running...
    {
        char cBuffer[256];
        sprintf_s(cBuffer, "taskkill /f /im %s", cFile);
        system(cBuffer);
        Sleep(2000);
        bProcessFound = FALSE;
    }

    if (!bProcessFound)    // if we didn't find it running...
    {
        startup(filename);
    }
}

//***************************************************************************************************************************
//***************************************************************************************************************************
void GetScreenSize(int& width, int& height)
{
    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);
}

