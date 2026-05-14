//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteServer64
///
/// @file   RemoteServer64.h
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

#include "RemoteServerInterface.h"


#ifdef REMOTESERVER_EXPORTS
#define REMOTESERVER_API_DLL __declspec(dllexport)
#else
#define REMOTESERVER_API_DLL __declspec(dllimport)
#endif

REMOTESERVER_API_DLL int32_t __cdecl GetRemoteServer64Interface(remote::IRemoteServerInterface** pService);
REMOTESERVER_API_DLL int32_t __cdecl ReleaseRemoteServer64Interface();




