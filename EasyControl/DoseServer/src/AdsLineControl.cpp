//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsLineControl
///
/// @file   AdsLineControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "AdsLineControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/include/AdsClient.h"


//*********************************************************************************************
//*********************************************************************************************
static int32_t Dose_EXSetIOUPSLowInputInv(BOOL bValue)
{
	return Dose_EXSetIOUPSLowInput( ! bValue);
}
//*********************************************************************************************
//*********************************************************************************************
CAdsLineControl::CAdsLineControl() : CAdsWrapperControl(0)
, m_Alarm{ { AdsSensor::CAdsLineDigitalOutput{ AdsClient_LineSetAlarm0, Dose_EXSetIOAlarm0Output }},
		   { AdsSensor::CAdsLineDigitalOutput{ AdsClient_LineSetAlarm1, Dose_EXSetIOAlarm1Output }},
		   { AdsSensor::CAdsLineDigitalOutput{ AdsClient_LineSetAlarm2, Dose_EXSetIOAlarm2Output }},
		   { AdsSensor::CAdsLineDigitalOutput{ AdsClient_LineSetAlarm3, Dose_EXSetIOAlarm3Output }}
		}
, m_AlarmQuit1 { AdsClient_LineGetAlarmQuit1 , Dose_EXSetIOAlarm1QuitInput }
, m_AlarmQuit03{ AdsClient_LineGetAlarmQuit03 , Dose_EXSetIOAlarm03QuitInput }
, m_Release{ AdsClient_LineSetRelease, Dose_EXSetIOLineReleaseOutput }
, m_LineSetOperateAuto{ AdsClient_LineSetOperateAuto, Dose_EXSetIOOperateAutoOutput }
, m_LineSetOperateService{ AdsClient_LineSetOperateService, Dose_EXSetIOServiceModeOutput }
, m_ExtAlarm { AdsClient_LineGetExtAlarm , Dose_EXSetIOExtAlarmInput }
, m_CustomAlarm { AdsClient_LineGetCustomerAlarm , Dose_EXSetIOCustomerAlarmInput }
, m_ShutDown { AdsClient_LineGetShutDown, Dose_EXSetIOAlarmShutDownInput }
, m_WindowsShutDown{ AdsClient_LineGetWindowsShutDown, Dose_EXSetIOWindowsShutDownInput }
, m_UPSLow{ AdsClient_LineGetUPSLow, Dose_EXSetIOUPSLowInputInv }
, m_LineGetMF{ AdsClient_LineGetMF , Dose_EXSetIOMainFeederInput }
, m_LineGetSF1{ AdsClient_LineGetSF1 , Dose_EXSetIOSideFeeder1Input }
, m_LineGetSF2{ AdsClient_LineGetSF2, Dose_EXSetIOSideFeeder2Input }
, m_LineGetStart{ AdsClient_LineGetStart , Dose_EXSetIOLineStartInput }
, m_LineGetOperateAuto{ AdsClient_LineGetOperateAuto, Dose_EXSetIOOperateAutoInput }
, m_LineGetOperateManual{ AdsClient_LineGetOperateManual, Dose_EXSetIOOperateManualInput }
, m_Container {{ &m_Alarm[0],&m_Alarm[1],&m_Alarm[2],&m_Alarm[3],
				&m_Release,	&m_AlarmQuit1,	&m_AlarmQuit03,	&m_LineSetOperateAuto,& m_LineSetOperateService,
				&m_ExtAlarm, &m_CustomAlarm,	&m_ShutDown, &m_WindowsShutDown,	&m_UPSLow,
				&m_LineGetMF, &m_LineGetSF1, &m_LineGetSF2, &m_LineGetStart, &m_LineGetOperateAuto, &m_LineGetOperateManual	
				}}
{
#if _DEBUG // Wenn assert -> InititializeLineItem vergessen
	BOOL bTemp = FALSE;
	Dose_EXGetIOAlarm0Output(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm1Output(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm2Output(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm3Output(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm1QuitInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm03QuitInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOExtAlarmInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOCustomerAlarmInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarmShutDownInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOLineReleaseOutput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOWindowsShutDownInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarmShutDownInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOUPSLowInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOMainFeederInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOSideFeeder1Input(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOSideFeeder2Input(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOLineStartInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOOperateAutoInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOOperateManualInput(&bTemp);
	assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsLineControl::Init()
{
	assert(!CBaseClass::IsInit());
	CBaseClass::Init();

	m_Container.Init();

	// wegen inverser Funktion
	if ( ! m_UPSLow.GetEnable())
	{
		Dose_EXInitIOUPSLowInput(FALSE); 
	}

#if _DEBUG // Wenn assert -> InititializeLineItem vergessen
	BOOL bTemp = FALSE;
	Dose_EXGetIOAlarm0Output(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm1Output(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm2Output(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm3Output(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm1QuitInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm03QuitInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOExtAlarmInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOCustomerAlarmInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarmShutDownInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOLineReleaseOutput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOWindowsShutDownInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarmShutDownInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOMainFeederInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOSideFeeder1Input(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOSideFeeder2Input(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOLineStartInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOOperateAutoInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOOperateManualInput(&bTemp);
	assert(bTemp == FALSE);
#endif
}
//*********************************************************************************************
//*********************************************************************************************
void CAdsLineControl::Exit()
{
	assert(CBaseClass::IsInit());

	m_Container.Exit();
	Dose_EXInitIOUPSLowInput(FALSE);
	Dose_EXInitIOWindowsShutDownInput(FALSE);

#if _DEBUG // Wenn assert -> InititializeLineItem vergessen
	BOOL bTemp = FALSE;
	Dose_EXGetIOAlarm0Output(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm1Output(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm2Output(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm3Output(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm1QuitInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarm03QuitInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOExtAlarmInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOCustomerAlarmInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarmShutDownInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOLineReleaseOutput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOWindowsShutDownInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOAlarmShutDownInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOUPSLowInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOMainFeederInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOSideFeeder1Input(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOSideFeeder2Input(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOLineStartInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOOperateAutoInput(&bTemp);
	assert(bTemp == FALSE);
	Dose_EXGetIOOperateManualInput(&bTemp);
	assert(bTemp == FALSE);
#endif

	CBaseClass::Exit();
}
