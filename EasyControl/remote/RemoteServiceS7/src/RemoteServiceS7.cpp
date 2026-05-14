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
#include "include/RemoteServiceS7.h"
#include "include/RemoteS7.h"

static remote::CRemoteS7* g_pRemoteServerImpl = nullptr;
 
REMOTE_API_DLL int32_t __cdecl GetRemoteServiceS7Interface(remote::IRemoteServiceInterface** pService)
{
    if ( ! g_pRemoteServerImpl)
    {
        g_pRemoteServerImpl = new remote::CRemoteS7;
    }
    *pService = g_pRemoteServerImpl;
    return 0;
}
//******************************************************************************************************
//******************************************************************************************************
REMOTE_API_DLL int32_t __cdecl ReleaseRemoteServiceS7Interface()
{
    if (g_pRemoteServerImpl)
    {
        delete g_pRemoteServerImpl;
        g_pRemoteServerImpl = nullptr;
    }
    return 0;
}
//******************************************************************************************************
//******************************************************************************************************


