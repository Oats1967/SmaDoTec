//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteServer64
///
/// @file   RemoteServer64.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "RemoteServer64.h"
#include "RemoteServerImpl.h"

static CRemoteServerImpl* g_pRemoteServerImpl = nullptr;

 
REMOTESERVER_API_DLL int32_t __cdecl GetRemoteServer64Interface(remote::IRemoteServerInterface** pService)
{
    if (!g_pRemoteServerImpl)
    {
        g_pRemoteServerImpl = new CRemoteServerImpl;
    }
    *pService = g_pRemoteServerImpl;
    return 0;
};


REMOTESERVER_API_DLL int32_t __cdecl ReleaseRemoteServer64Interface()
{
    if (g_pRemoteServerImpl)
    {
        delete g_pRemoteServerImpl;
        g_pRemoteServerImpl = nullptr;
    }
    return 0;
};




