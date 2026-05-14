//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteServiceModbusStandard
///
/// @file   RemoteServiceModbusStandard.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "remote/RemoteServiceModbusStandard/include/RemoteServiceModbusStandard.h"
#include "RemoteModbusStandard.h"

static CRemoteModbusStandard* g_pRemoteServerImpl = nullptr;

 
REMOTE_API_DLL int32_t __cdecl GetRemoteServiceModbusStandardInterface(remote::IRemoteServiceInterface** pService)
{
    if (!g_pRemoteServerImpl)
    {
        g_pRemoteServerImpl = new CRemoteModbusStandard;
    }
    *pService = g_pRemoteServerImpl;
    return 0;
};
//******************************************************************************************************
//******************************************************************************************************
REMOTE_API_DLL int32_t __cdecl ReleaseRemoteServiceModbusStandardInterface()
{
    if (g_pRemoteServerImpl)
    {
        delete g_pRemoteServerImpl;
        g_pRemoteServerImpl = nullptr;
    }
    return 0;
};


