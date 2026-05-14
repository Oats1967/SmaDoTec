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

#include "DoseServer/include/DoseServerInterface.h"


#ifdef DOSESERVER_EXPORTS
#define DOSESERVER_API_DLL __declspec(dllexport)
#else
#define DOSESERVER_API_DLL __declspec(dllimport)
#endif

DOSESERVER_API_DLL int32_t __cdecl GetDoseServerInterface(IDoseServerInterface** pService);
DOSESERVER_API_DLL int32_t __cdecl ReleaseDoseServerInterface();




