//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsIcpControl
///
/// @file   AdsIcpControl.cpp
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
#include "AdsIcpControl.h"
#include "AdsClient/include/AdsClient.h"
#include "DoseDataLib/include/DoseData.h"

//*********************************************************************************************
//*********************************************************************************************
CAdsIcpControl::CAdsIcpControl(const int32_t index) : CBaseClass { index}
, m_GetMassflow{ index, AdsClient_DoseGetMassflow }
, m_RefillRequest { index, AdsClient_DoseGetRefillRequest, Dose_DSVSetRefillState }
, m_RefillOutput{ index, AdsClient_DoseSetRefillRequest, Dose_DSVSetIORefillRequestOutput }
{
#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_DSVGetRefillState(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIORefillRequestOutput(m_id, &bTemp);
    assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsIcpControl::Init()
{
    assert(! CBaseClass::IsInit());

    CBaseClass::Init();

	m_GetMassflow.Init();
	m_RefillRequest.Init();
	m_RefillOutput.Init();

#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_DSVGetRefillState(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIORefillRequestOutput(m_id, &bTemp);
    assert(bTemp == FALSE);
#endif
	assert(m_GetMassflow.GetEnable());
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsIcpControl::Exit()
{
    assert(CBaseClass::IsInit());

    m_GetMassflow.Exit();
    m_RefillRequest.Exit();
    m_RefillOutput.Exit();

#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_DSVGetRefillState(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIORefillRequestOutput(m_id, &bTemp);
    assert(bTemp == FALSE);
#endif
    CBaseClass::Exit();
}


