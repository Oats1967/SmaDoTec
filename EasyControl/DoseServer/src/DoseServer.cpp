//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseServer
///
/// @file   DoseServer.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "DoseServer/include/DoseServer.h"
#include "DoseServerImpl.h"

static CDoseServerImpl* gp_DoseServerImpl = nullptr;


DOSESERVER_API_DLL int32_t __cdecl GetDoseServerInterface(IDoseServerInterface** pService)
{
    if  (! gp_DoseServerImpl)
    {
        gp_DoseServerImpl = new CDoseServerImpl;
    }
    *pService = gp_DoseServerImpl;
    return 0;
};


DOSESERVER_API_DLL int32_t __cdecl ReleaseDoseServerInterface()
{
    if ( gp_DoseServerImpl )
    {
        delete gp_DoseServerImpl;
        gp_DoseServerImpl = nullptr;
    }
    return 0;
};



