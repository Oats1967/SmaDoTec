//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsBaseControl
///
/// @file   AdsBaseControl.cpp
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
#include "AdsBaseControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/include/AdsClient.h"



//*********************************************************************************************
//*********************************************************************************************
CAdsBaseControl::CAdsBaseControl(const int32_t index) : CBaseClass(index)
, m_AlarmInput  {   index, AdsClient_DoseGetExtAlarm, Dose_DSVSetIOExtAlarmInput }
, m_EmptyFeederStart { index, AdsClient_DoseGetEmptyFeederStart , Dose_DSVSetIOEmptyFeederStartInput }
, m_EmptyFeederStop{ index, AdsClient_DoseGetEmptyFeederStop , Dose_DSVSetIOEmptyFeederStopInput }
, m_GravVol { index, AdsClient_DoseGetGravVol , Dose_DSVSetIOVolumetricInput }
, m_StartInput{ index, AdsClient_DoseGetStart,  Dose_DSVSetIOStartInput }
, m_ReleaseInput{ index, AdsClient_DoseGetRelease,  Dose_DSVSetIOReleaseInput }
, m_AlarmOutput{ index, AdsClient_DoseSetAlarm }
, m_Container{ { &m_AlarmInput, &m_EmptyFeederStart, &m_EmptyFeederStop , &m_GravVol, &m_StartInput, &m_ReleaseInput , &m_AlarmOutput  }  }
{
#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
	BOOL bTemp = FALSE;
	Dose_DSVGetIOExtAlarmInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIOVolumetricInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIOEmptyFeederStopInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIOEmptyFeederStartInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIOStartInput(m_id, &bTemp);
	assert(bTemp == FALSE);
#if 0
	Dose_DSVGetIOReleaseInput(m_id, &bTemp);
	assert(bTemp == FALSE);
#endif
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsBaseControl::Init()
{
	assert(!CBaseClass::IsInit());

	CBaseClass::Init();

	m_Container.Init();

#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
	BOOL bTemp = FALSE;
	Dose_DSVGetIOExtAlarmInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIOVolumetricInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIOEmptyFeederStopInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIOEmptyFeederStartInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIOStartInput(m_id, &bTemp);
	assert(bTemp == FALSE);
#if 0
	Dose_DSVGetIOReleaseInput(m_id, &bTemp);
	assert(bTemp == FALSE);
#endif
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsBaseControl::Exit()
{
	assert(CBaseClass::IsInit());

	m_Container.Exit();

#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
	BOOL bTemp = FALSE;
	Dose_DSVGetIOExtAlarmInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIOVolumetricInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIOEmptyFeederStopInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIOEmptyFeederStartInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIOStartInput(m_id, &bTemp);
	assert(bTemp == FALSE);
#if 0
	Dose_DSVGetIOReleaseInput(m_id, &bTemp);
	assert(bTemp == FALSE);
#endif
#endif
	CBaseClass::Exit();
}



