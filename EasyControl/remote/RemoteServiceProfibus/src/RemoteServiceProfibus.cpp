//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteServiceProfibus
///
/// @file   RemoteServiceProfibus.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "RemoteServiceProfibus.h"
#include "Profibus.h"

static remote::CProfibus* g_pRemoteServerImpl = nullptr;

REMOTEPROFIBUS_API_DLL int32_t __cdecl GetRemoteServiceProfibusInterface(remote::IRemoteServiceInterface** pService)
{
    if ( ! g_pRemoteServerImpl )
    {
        g_pRemoteServerImpl = new remote::CProfibus;
    }
    *pService = g_pRemoteServerImpl;
    return 0;
}
//******************************************************************************************************
//******************************************************************************************************
REMOTEPROFIBUS_API_DLL int32_t __cdecl ReleaseRemoteServiceProfibusInterface()
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


