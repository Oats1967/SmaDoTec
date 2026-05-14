//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteServiceS7
///
/// @file   RemoteServiceS7.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "include/S7Simulator64.h"
#include "include/DeviceSimulator.h"


static CDeviceSimulator* g_pDeviceImpl = nullptr;
 
S7SIMULATOR64_API_DLL int32_t __cdecl GetS7SimulatorInterface(remote::IDeviceInterface** pService)
{
    if (!g_pDeviceImpl)
    {
        g_pDeviceImpl = new CDeviceSimulator;
    }
    *pService = g_pDeviceImpl;
    return 0;
}
//******************************************************************************************************
//******************************************************************************************************
S7SIMULATOR64_API_DLL int32_t __cdecl ReleaseS7SimulatorInterface(void)
{
    if (g_pDeviceImpl)
    {
        delete g_pDeviceImpl;
        g_pDeviceImpl = nullptr;
    }
    return 0;
}




