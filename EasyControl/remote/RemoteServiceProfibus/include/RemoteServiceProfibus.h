//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteServiceProfibus
///
/// @file   RemoteServiceProfibus.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "remote/include/RemoteServiceInterface.h"


#ifdef REMOTESERVICEPROFIBUS_EXPORTS
#define REMOTEPROFIBUS_API_DLL __declspec(dllexport)
#else
#define REMOTEPROFIBUS_API_DLL __declspec(dllimport)
#endif

REMOTEPROFIBUS_API_DLL int32_t __cdecl GetRemoteServiceProfibusInterface(remote::IRemoteServiceInterface** pService);
REMOTEPROFIBUS_API_DLL int32_t __cdecl ReleaseRemoteServiceProfibusInterface();



