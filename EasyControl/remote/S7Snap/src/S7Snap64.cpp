//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module S7Snap64
///
/// @file   S7Snap64.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------

#include "DeviceSnap.h"
#include "include/S7Snap64.h"

static CDeviceSnap* g_pDeviceImpl = nullptr;

S7SNAP64_API_DLL int32_t __cdecl GetS7Snap64Interface(remote::IDeviceInterface** pService)
{
    if (!g_pDeviceImpl)
    {
        g_pDeviceImpl = new CDeviceSnap;
    }
    *pService = g_pDeviceImpl;
    return 0;
};
//************************************************************************************************
//************************************************************************************************
S7SNAP64_API_DLL int32_t __cdecl ReleaseS7Snap64Interface(void)
{
    if (g_pDeviceImpl)
    {
        delete g_pDeviceImpl;
        g_pDeviceImpl = nullptr;
    }
    return 0;
};


