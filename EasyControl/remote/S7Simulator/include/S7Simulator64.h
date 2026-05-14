//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteServiceS7
///
/// @file   RemoteServiceS7.h
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

#include "remote/include/DeviceInterface.h"


#ifdef S7SIMULATOR_EXPORTS
#define S7SIMULATOR64_API_DLL __declspec(dllexport)
#else
#define S7SIMULATOR64_API_DLL __declspec(dllimport)
#endif

S7SIMULATOR64_API_DLL int32_t __cdecl GetS7SimulatorInterface(remote::IDeviceInterface** pService);
S7SIMULATOR64_API_DLL int32_t __cdecl ReleaseS7SimulatorInterface(void);



