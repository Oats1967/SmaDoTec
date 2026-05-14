//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsRampControl
///
/// @file   AdsRampControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/include/AdsClient.h"
#include "AdsRampControl.h"


//*********************************************************************************************************************
//*********************************************************************************************************************
CAdsRampControl::CAdsRampControl() : CAdsWrapperControl{ 0 }
, m_RampUp { AdsClient_LineGetRampUp , Dose_EXSetIORampUpInput }
, m_RampDown{ AdsClient_LineGetRampDown , Dose_EXSetIORampDownInput }
{
#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_EXGetIORampUpInput(&bTemp);
    assert(bTemp == FALSE);
    Dose_EXGetIORampDownInput(&bTemp);
    assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CAdsRampControl::Init()
{
    CBaseClass::Init();

    m_RampUp.Init();
    m_RampDown.Init();
    Dose_EXInitIORampUpInput(FALSE);
    Dose_EXInitIORampDownInput(FALSE);
}
