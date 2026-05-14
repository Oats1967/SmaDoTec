//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module TKBoard64
///
/// @file   TKBoard64.h
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


#ifdef TKBOARD_EXPORTS
#define TKBOARD_API_DLL __declspec(dllexport)
#else
#define TKBOARD_API_DLL __declspec(dllimport)
#endif

TKBOARD_API_DLL int32_t __cdecl GetTKBoard64Interface(ITKBoardInterface** pService);
TKBOARD_API_DLL int32_t __cdecl ReleaseTKBoard64Interface(void);




