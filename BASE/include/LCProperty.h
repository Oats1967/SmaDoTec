//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LCProperty
///
/// @file   LCProperty.h
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

#include "BASE/types.h"
#include "BASE/include/LCType.h"
#include "BASE/include/LCTypeDetection.h"

namespace base
{
    struct LCProperty
    {
        BOOL                    bStartInit          = FALSE;                                // legt fest, ob Zelle schon besucht wurde
        BOOL                    bInitialized        = FALSE;                                // legt fest, ob Zelle initialisiert wurde
        base::LC_STATUS	        eStatus             = base::LC_STATUS::LC_STATUS_UNKNOWN;
        base::eLcTypeDetection  eDetectionType      = base::eLcTypeDetection::LC_DET_NONE;
        base::eLcType		    eType               = base::eLcType::LC_EMPTY;
        base::eLcSubType	    eSubType            = base::eLcSubType::eUnknown;
        float32_t			    fVollLast           = 0.0F;
        float32_t			    fWeight             = 0.0F;
        float32_t			    fRawWeight          = 0.0F;
        float32_t			    fTaraWeight         = 0.0F;
        float32_t			    fCorrectionFactor   = 1.0F;
        int32_t				    sErrorCounter       = 0;
        base::LC_PRIORITY	    sPriority           = base::LC_PRIORITY::LC_PRIORITY_LOW;
        uint64_t			    ulTime              = 0u;
    };
};



