//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsHelperTypes
///
/// @file   AdsHelperTypes.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "AdsClient/include/AdsTypen.h"

typedef struct __BUSHANDLECTRL
{
    uint32_t	wIndex;
    uint32_t	pStatus;
    uint32_t	pHdlIn;
    uint32_t	pHdlOut;
    uint32_t	pCtrl;
    BOOL	    bEnable;
} BUSHANDLECTRL;

typedef uint64_t LongHandle;

