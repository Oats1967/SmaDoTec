//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsRefillControl
///
/// @file   AdsRefillControl.cpp
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
#include "AdsRefillControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/include/AdsClient.h"

//*********************************************************************************************************************
//*********************************************************************************************************************
static int32_t Dose_DSVSetIODriveErrorRefillFeederInputInv(int32_t id, BOOL bValue)
{
	return Dose_DSVSetIODriveErrorRefillFeederInput(id, !bValue);
}

//*********************************************************************************************
//*********************************************************************************************
CAdsRefillControl::CAdsRefillControl(const int32_t index) : CAdsWrapperControl{ index }
, m_RefillStop		{ index, AdsClient_DoseGetRefillStop ,	   Dose_DSVSetIORefillStopInput }
, m_Refill			{ index, AdsClient_DoseGetRefill ,		   Dose_DSVSetIORefillInput }
, m_RefillRequest	{ index, AdsClient_DoseGetRefillRequest ,  Dose_DSVSetIORefillRequestInput }
, m_RefillRelease	{ index, AdsClient_DoseGetRefillRelease ,  Dose_DSVSetIORefillReleaseInput }
, m_RefillExtAlarm  { index, AdsClient_DoseGetRefillExtAlarm , Dose_DSVSetIORefillAlarmInput }
, m_DriveErrorRefillFeeder{ index,  AdsClient_DoseGetDriveErrorRefillFeeder,   Dose_DSVSetIODriveErrorRefillFeederInputInv }
, m_Container{ { &m_RefillStop , &m_Refill, &m_RefillRequest, &m_RefillRelease, &m_RefillExtAlarm, &m_DriveErrorRefillFeeder  } }
{
#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
	BOOL bTemp = FALSE;
	Dose_DSVGetIORefillStopInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIORefillInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIORefillRequestInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIORefillReleaseInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIORefillAlarmInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIODriveErrorRefillFeederInput(m_id, &bTemp);
	assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsRefillControl::Init()
{
	assert(!CBaseClass::IsInit());

	CBaseClass::Init();

	m_Container.Init();

	if (!m_DriveErrorRefillFeeder.GetEnable())
	{
		Dose_DSVInitIODriveErrorRefillFeederInput(m_id, FALSE);
	}

#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
	BOOL bTemp = FALSE;
	Dose_DSVGetIORefillStopInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIORefillInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIORefillRequestInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIORefillReleaseInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIORefillAlarmInput(m_id, &bTemp);
	assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************
//*********************************************************************************************
inline void CAdsRefillControl::Exit()
{
	assert(CBaseClass::IsInit());

	m_Container.Exit();

	Dose_DSVInitIODriveErrorRefillFeederInput(m_id, FALSE);

#if _DEBUG // Wenn assert -> InititializeDoseItem vergessen
	BOOL bTemp = FALSE;
	Dose_DSVGetIORefillStopInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIORefillInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIORefillRequestInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIORefillReleaseInput(m_id, &bTemp);
	assert(bTemp == FALSE);
	Dose_DSVGetIORefillAlarmInput(m_id, &bTemp);
	assert(bTemp == FALSE);
#endif

	CBaseClass::Exit();
}



