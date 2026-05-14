//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module VKBoard64
///
/// @file   VKBoard64.h
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

#include "TKBoardInterface.h"


#ifdef VKBOARD_EXPORTS
#define VKBOARD_API_DLL __declspec(dllexport)
#else
#define VKBOARD_API_DLL __declspec(dllimport)
#endif

VKBOARD_API_DLL int32_t __cdecl GetVKBoard64Interface(ITKBoardInterface** pService);
VKBOARD_API_DLL int32_t __cdecl ReleaseVKBoard64Interface(void);




