//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module TKBoard64
///
/// @file   TKBoard64.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------



#include "TKBoardImpl.h"
#include "TKBoard64.h"

static CTKBoardImpl* g_pDeviceImpl = nullptr;

TKBOARD_API_DLL int32_t __cdecl GetTKBoard64Interface(ITKBoardInterface** pService)
{
    if (!g_pDeviceImpl)
    {
        g_pDeviceImpl = new CTKBoardImpl;
    }
    *pService = g_pDeviceImpl;
    return 0;
};
//************************************************************************************************
//************************************************************************************************
TKBOARD_API_DLL int32_t __cdecl ReleaseTKBoard64Interface(void)
{
    if (g_pDeviceImpl)
    {
        delete g_pDeviceImpl;
        g_pDeviceImpl = nullptr;
    }
    return 0;
};


