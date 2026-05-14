//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsSetpointControl
///
/// @file   AdsSetpointControl.cpp
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
#include "AdsSetpointControl.h"


//*********************************************************************************************************************
//*********************************************************************************************************************
CAdsSetpointControl::CAdsSetpointControl() : CAdsWrapperControl{ 0 }
, m_Massflow { AdsClient_LineSetMassflow }
{}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CAdsSetpointControl::Init()
{
    CBaseClass::Init();
    m_Massflow.Init();
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CAdsSetpointControl::Exit()
{
    m_Massflow.Exit();
    CBaseClass::Exit();
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CAdsSetpointControl::AdsLineSetMassflow()
{
	assert(CBaseClass::IsInit());

    auto bEnable = m_Massflow.GetEnable();
    if (bEnable)
    {
        float32_t fSumme = 0.0f;
        Dose_EXGetLineMassflow(&fSumme);
        float32_t fMaxLeistung = 0.0f;
        Dose_EXGetMaxLeistung(&fMaxLeistung);
        const float32_t actvalue = (fMaxLeistung) > 0.0f ? __min(fSumme / fMaxLeistung, 1.0f) : 0.0f;
        m_Massflow.UpdateState(actvalue);
    }
    return bEnable;
}
