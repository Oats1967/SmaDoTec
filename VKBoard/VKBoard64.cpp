//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module VKBoard64
///
/// @file   VKBoard64.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------



#include "VKBoard/private/VKBoardImpl.h"
#include "VKBoard/public/VKBoard64.h"

static CVKBoardImpl* g_pDeviceImpl = nullptr;

VKBOARD_API_DLL int32_t __cdecl GetVKBoard64Interface(ITKBoardInterface** pService)
{
    if (!g_pDeviceImpl)
    {
        g_pDeviceImpl = new CVKBoardImpl;
    }
    *pService = g_pDeviceImpl;
    return 0;
};
//************************************************************************************************
//************************************************************************************************
VKBOARD_API_DLL int32_t __cdecl ReleaseVKBoard64Interface(void)
{
    if (g_pDeviceImpl)
    {
        delete g_pDeviceImpl;
        g_pDeviceImpl = nullptr;
    }
    return 0;
};


