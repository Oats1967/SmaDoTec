//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsTotalizerPulseControl
///
/// @file   AdsTotalizerPulseControl.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "AdsTotalizerPulseControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/include/AdsClient.h"



//*********************************************************************************************************************
//*********************************************************************************************************************
CAdsTotalizerPulseControl::CAdsTotalizerPulseControl() : CBaseClass{ 0 }
, m_Output { AdsClient_LineSetTotalizerPulse , Dose_EXSetIOTotalizerPulseOutput }
{
#if _DEBUG // Wenn assert -> ResetLineItem vergessen
    BOOL bTemp = FALSE;
    Dose_EXGetIOTotalizerPulseOutput(&bTemp);
    assert(bTemp == FALSE);
#endif
}
