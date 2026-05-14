//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteServiceModbusStandard
///
/// @file   RemoteServiceModbusStandard.h
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


#ifdef REMOTESERVICEMODBUSSTANDARD_EXPORTS
#define REMOTE_API_DLL __declspec(dllexport)
#else
#define REMOTE_API_DLL __declspec(dllimport)
#endif

REMOTE_API_DLL int32_t __cdecl GetRemoteServiceModbusStandardInterface(remote::IRemoteServiceInterface** pService);
REMOTE_API_DLL int32_t __cdecl ReleaseRemoteServiceModbusStandardInterface();



