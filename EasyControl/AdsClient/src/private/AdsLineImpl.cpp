//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsLineImpl.cpp
///         This modules handles all general ADS input/outputs which are related to a line unit
///
/// @file   AdsLineImpl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "AdsClient/src/private/EasyControlVer.h"
#include <climits>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include "BASE/types.h"
#include "AdsClient/src/private/AdsHelper.h"
#include "AdsClient/src/private/AdsLineImpl.h"


#define SCALEFAKTORIN  SHRT_MAX
#define SCALEFAKTOROUT SCALEFAKTORIN


CAdsLineImpl::CAdsLineImpl(const base::CIOLineConfig& cfg) : CAdsControlImpl()
	, m_LineConfig{ cfg }
	, m_PortAddress(0)
{
	m_Handle.fill(0);
}
//**************************************************************************************************************
//**************************************************************************************************************
inline int32_t CAdsLineImpl::GetDigitalInput(const base::eLineDigitalInputAction type, BOOL& value, const BOOL bDefault)
{
	int32_t result = ADS_OK;

	auto channel = m_LineConfig.getDI().Get(type);
	if (channel >= 0)
	{
		value = m_DigitalInput.getInput(m_Handle[eINPUTS_EXIO_DI0 + channel]);
	}
	else
	{
		value = bDefault;
		result = ADS_MPM_NOK;
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
inline int32_t CAdsLineImpl::GetAnalogInput(const base::eLineAnalogInputAction type, float32_t& value)
{
	int32_t result = ADS_OK;

	auto channel = m_LineConfig.getAI().Get(type);
	if (channel >= 0)
	{
		value = m_AnalogInput.getInput(m_Handle[eINPUTS_EXIO_AI0 + channel]);
	}
	else
	{
		value = 0.0F;
		result = ADS_MPM_NOK;
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
inline int32_t CAdsLineImpl::SetDigitalOutput(const base::eLineDigitalOutputAction type, const BOOL value)
{
	int32_t result = ADS_OK;

	auto channel = m_LineConfig.getDO().Get(type);
	if (channel >= 0)
	{
		m_DigitalOutput.setOutput(m_Handle[eOUTPUTS_EXIO_DO0 + channel], value);
	}
	else
	{
		result = ADS_MPM_NOK;
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
inline int32_t CAdsLineImpl::SetAnalogOutput(const base::eLineAnalogOutputAction type, const float32_t value)
{
	int32_t result = ADS_OK;

	auto channel = m_LineConfig.getAO().Get(type);
	if (channel >= 0)
	{
		m_AnalogOutput.setOutput(m_Handle[eOUTPUTS_EXIO_AO0 + channel], value);
	}
	else
	{
		result = ADS_MPM_NOK;
	}
	return result;
}

//**************************************************************************************************************
//**************************************************************************************************************
IMPL_LINE_DI(CAdsLineImpl, UPSLow, base::eLineDigitalInputAction::eInputActionUPSLow)
IMPL_LINE_DI(CAdsLineImpl, OperateAuto, base::eLineDigitalInputAction::eInputActionOperateAuto)
IMPL_LINE_DI(CAdsLineImpl, OperateManual, base::eLineDigitalInputAction::eInputActionOperateManual)
IMPL_LINE_DI(CAdsLineImpl, Start, base::eLineDigitalInputAction::eInputActionStart )
IMPL_LINE_DI(CAdsLineImpl, ShutDown, base::eLineDigitalInputAction::eInputActionShutDown)
IMPL_LINE_DI(CAdsLineImpl, WindowsShutDown, base::eLineDigitalInputAction::eInputActionWindowsShutDown)
IMPL_LINE_DI(CAdsLineImpl,SetpointExt, base::eLineDigitalInputAction::eInputActionSetpointExt)
IMPL_LINE_DI(CAdsLineImpl, InvSetpointExt, base::eLineDigitalInputAction::eInputActionInvSetpointExt)
IMPL_LINE_DI(CAdsLineImpl, RampUp, base::eLineDigitalInputAction::eInputActionRampUp)
IMPL_LINE_DI(CAdsLineImpl,RampDown, base::eLineDigitalInputAction::eInputActionRampDown)
#ifdef __SIM_RELEASE
SIM_LINE_DI(CAdsLineImpl, MF)
#else
IMPL_LINE_DI(CAdsLineImpl, MF, base::eLineDigitalInputAction::eInputActionMF)
#endif
IMPL_LINE_DI(CAdsLineImpl,SF1, base::eLineDigitalInputAction::eInputActionSF1)
IMPL_LINE_DI(CAdsLineImpl,SF2, base::eLineDigitalInputAction::eInputActionSF2)
IMPL_LINE_DI(CAdsLineImpl,CustomerAlarm, base::eLineDigitalInputAction::eInputActionCustomerAlarm)
IMPL_LINE_DI(CAdsLineImpl,ExtAlarm, base::eLineDigitalInputAction::eInputActionExtAlarm)
IMPL_LINE_DI(CAdsLineImpl,AlarmQuit1, base::eLineDigitalInputAction::eInputActionAlarmQuit1)
IMPL_LINE_DI(CAdsLineImpl,AlarmQuit03, base::eLineDigitalInputAction::eInputActionAlarmQuit03)


IMPL_LINE_DO(CAdsLineImpl,Alarm0, base::eLineDigitalOutputAction::eOutputActionAlarm0)
IMPL_LINE_DO(CAdsLineImpl,Alarm1, base::eLineDigitalOutputAction::eOutputActionAlarm1)
IMPL_LINE_DO(CAdsLineImpl,Alarm2, base::eLineDigitalOutputAction::eOutputActionAlarm2)
IMPL_LINE_DO(CAdsLineImpl,Alarm3, base::eLineDigitalOutputAction::eOutputActionAlarm3)
IMPL_LINE_DO(CAdsLineImpl, Release, base::eLineDigitalOutputAction::eOutputActionLineRelease)
IMPL_LINE_DO(CAdsLineImpl, TotalizerPulse, base::eLineDigitalOutputAction::eOutputActionTotalizerPulse)
IMPL_LINE_DO(CAdsLineImpl, OperateAuto, base::eLineDigitalOutputAction::eOutputActionOperateAuto)
IMPL_LINE_DO(CAdsLineImpl, OperateService, base::eLineDigitalOutputAction::eOutputActionOperateService)

IMPL_LINE_AI(CAdsLineImpl,Setpoint, base::eLineAnalogInputAction::eInputActionSetpoint)
IMPL_LINE_AO(CAdsLineImpl,Massflow, base::eLineAnalogOutputAction::eOutputActionMassflow)

//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::GetAnalogInputHandle(const int32_t inputchannel)
{
	char_t szSymbol[64];

	LOGDEBUG("GetAnalogInputHandle, index = " << inputchannel);
	assert((inputchannel >= 0) && (inputchannel < base::cLineAnalogInputChannel));
	assert(m_PortAddress > 0);
	sprintf_s(szSymbol, "EXIO.INPUTS.AI%d", inputchannel);
	const int32_t hdlIndex = eINPUTS_EXIO_AI0 + inputchannel;
	m_Handle[hdlIndex] = AddAnalogInput(szSymbol, m_PortAddress);
#ifdef _DEBUG
	if (!m_Handle[hdlIndex])
	{
		LOGDEBUG("Invalid handle " << szSymbol);
	}
#endif
	return (m_Handle[hdlIndex] != 0) ? ADS_OK : ADS_NOTOPEN;
}
//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::GetAnalogOutputHandle(const int32_t outputchannel)
{
	char_t szSymbol[64];

	LOGDEBUG("GetAnalogOutputHandle, index = " << outputchannel);
	assert((outputchannel >= 0) && (outputchannel < base::cLineAnalogOutputChannel));
	assert(m_PortAddress > 0);
	sprintf_s(szSymbol, "EXIO.OUTPUTS.AO%d", outputchannel);
	const int32_t hdlIndex = eOUTPUTS_EXIO_AO0 + outputchannel;
	m_Handle[hdlIndex] = AddAnalogOutput(szSymbol, m_PortAddress);
#ifdef _DEBUG
	if (!m_Handle[hdlIndex])
	{ 
		LOGDEBUG("Invalid handle " << szSymbol);
	}
#endif
	return (m_Handle[hdlIndex] != 0) ? ADS_OK : ADS_NOTOPEN;
}
//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::GetDigitalInputHandle(const int32_t inputchannel)
{
	char_t szSymbol[64];

	LOGDEBUG("GetDigitalInputHandle, index = "<< inputchannel);
	assert((inputchannel >= 0) && (inputchannel < base::cLineDigitalInputChannel));
	assert(m_PortAddress > 0);
	sprintf_s(szSymbol, "EXIO.INPUTS.DI%d", inputchannel);
	const int32_t hdlIndex = eINPUTS_EXIO_DI0 + inputchannel;
	m_Handle[hdlIndex] = AddDigitalInput(szSymbol, m_PortAddress);
#ifdef _DEBUG
	if (!m_Handle[hdlIndex])
	{
		LOGDEBUG("Invalid handle " << szSymbol);
	}
#endif
	return (m_Handle[hdlIndex] != 0) ? ADS_OK : ADS_NOTOPEN;
}
//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::GetDigitalOutputHandle(const int32_t outputchannel)
{
	char_t szSymbol[64];

	LOGDEBUG("GetDigitalOutputHandle, index = " << outputchannel);
	assert(m_PortAddress > 0);
	assert((outputchannel >= 0) && (outputchannel < base::cLineDigitalOutputChannel));
	sprintf_s(szSymbol, "EXIO.OUTPUTS.DO%d", outputchannel);
	const int32_t hdlIndex = eOUTPUTS_EXIO_DO0 + outputchannel;
	m_Handle[hdlIndex] = AddDigitalOutput(szSymbol, m_PortAddress);
#ifdef _DEBUG
	if (!m_Handle[hdlIndex])
	{
		LOGDEBUG("Invalid handle" << szSymbol);
	}
#endif
	return (m_Handle[hdlIndex] != 0) ? ADS_OK : ADS_NOTOPEN;
}
//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::GetCommstateHandle(void)
{
	char_t szSymbol[64];

	LOGDEBUG("GetCommstateHandle !");

	assert(m_PortAddress > 0);
	sprintf_s(szSymbol, "EXIO.INPUTS.COMMSTATE");
	int32_t result = AdsHelperGetHandle(m_PortAddress, szSymbol, m_Handle[eINPUTS_EXIO_COMMSTATE]);
	if (result != ADS_OK)
	{
		LOGERROR("CommstateHandle NOT found !" << ", Errorcode = 0x" << std::hex << result);
	}
	return result;
}
//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::GetOpModeHandle(void)
{
	char_t szSymbol[64];

	LOGDEBUG("GetOpModeHandle !");

	assert(m_PortAddress > 0);
	sprintf_s(szSymbol, "EXIO.INPUTS.OPMODE");
	int32_t result = AdsHelperGetHandle(m_PortAddress, szSymbol, m_Handle[eINPUTS_EXIO_OPMODE]);
	if (result != ADS_OK)
	{
		LOGERROR("OpModeHandle NOT found !" << ", Errorcode = 0x" << std::hex << result);
	}
	return result;
}
//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::ReleaseCommstateHandle()
{	
	LOGINFO("ReleaseCommstateHandle");
	assert(m_PortAddress > 0);
	int32_t result = AdsHelperReleaseHandle(m_PortAddress, m_Handle[eINPUTS_EXIO_COMMSTATE]);
	if (result != ADS_OK)
	{
		LOGERROR("ReleaseCommstateHandle failed !, Errorcode = 0x" << std::hex << result);
	}
	m_Handle[eINPUTS_EXIO_COMMSTATE] = 0;
	return result;
}
//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::ReleaseOpModeHandle()
{
	LOGINFO("ReleaseOpModeHandle");
	assert(m_PortAddress > 0);
	int32_t result = AdsHelperReleaseHandle(m_PortAddress, m_Handle[eINPUTS_EXIO_OPMODE]);
	if (result != ADS_OK)
	{
		LOGERROR("ReleaseOpModeHandle failed !, Errorcode = 0x" << std::hex << result);
	}
	m_Handle[eINPUTS_EXIO_OPMODE] = 0;
	return result;
}
//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::GetAllDigitalInputHandle()
{
	int32_t inputchannel = 0;

	const auto& rCfg = m_LineConfig.getDI();
	assert(base::cLineDigitalInputChannel == _S32(rCfg.Count()));
	for (int32_t i = 0; i < _S32(rCfg.Count()); i++)
	{
		auto result = GetDigitalInputHandle(i);
		if (result == ADS_OK)
		{
			inputchannel++;
		}
		else
		{
			LOGDEBUG("GetDigitalInputHandle failed !, inputchannel = " << i);
		}
	}
	return inputchannel;
}
//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::GetAllDigitalOutputHandle()
{
	int32_t outputchannel = 0;

	const auto& rCfg = m_LineConfig.getDO();
	assert(base::cLineDigitalOutputChannel == _S32(rCfg.Count()));
	for (int32_t i = 0; i < _S32(rCfg.Count()); i++)
	{
		auto result = GetDigitalOutputHandle(i);
		if (result == ADS_OK)
		{
			outputchannel++;
		}
		else
		{
			LOGDEBUG("GetDigitalOutputHandle failed !, outputchannel = " << i);
		}
	}
	return outputchannel;
}
//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::GetAllAnalogInputHandle()
{
	int32_t inputchannel = 0;

	const auto& rCfg = m_LineConfig.getAI();
	assert(base::cLineAnalogInputChannel == _S32(rCfg.Count()));
	for (int32_t i = 0; i < _S32(rCfg.Count()); i++)
	{
		auto result = GetAnalogInputHandle(i);
		if (result == ADS_OK)
		{
			inputchannel++;
		}
		else
		{
			LOGDEBUG("GetAnalogInputHandle failed !, inputchannel = " << i);
		}
	}
	return inputchannel;
}
//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::GetAllAnalogOutputHandle()
{
	int32_t outputchannel = 0;
	const auto& rCfg = m_LineConfig.getAO();
	assert(base::cLineAnalogOutputChannel == _S32(rCfg.Count()));
	for (int32_t i = 0; i < _S32(rCfg.Count()); i++)
	{
		auto result = GetAnalogOutputHandle(i);
		if (result == ADS_OK)
		{
			outputchannel++;
		}
		else
		{
			LOGDEBUG("GetAnalogOutputHandle failed !, outputchannel = " << i);
		}
	}
	return outputchannel;
}
//****************************************************************************************************************************
//****************************************************************************************************************************
int32_t CAdsLineImpl::GetCommunicationState(uint16_t& value)
{
	int32_t result = AdsHelperStatusInput(m_PortAddress, m_Handle[eINPUTS_EXIO_COMMSTATE], value);
	if (result != ADS_OK)
	{
		LOGERROR("GetCommunicationState failed ! !");
	}
	return result;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t CAdsLineImpl::GetOpMode(BOOL& value)
{
	uint16_t temp = 0;
	int32_t result = AdsHelperStatusInput(m_PortAddress, m_Handle[eINPUTS_EXIO_OPMODE], temp);
	if (result != ADS_OK)
	{
		LOGERROR("GetOpMode failed ! !");
	}
	value = temp == 8;
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
int32_t CAdsLineImpl::Open(const uint16_t iPortAddress)
{
	LOGDEBUG("Line Open !");

	if (m_PortAddress != 0)
	{
		return ADS_OK;
	}
	m_Handle.fill(0);
	m_PortAddress = iPortAddress;
	{
		auto result = GetCommstateHandle();
		if (result == ADS_OK)
		{
			result = GetOpModeHandle();
		}
	}
	auto i1 = GetAllDigitalInputHandle();
	auto i2 = GetAllDigitalOutputHandle();
	auto i3 = GetAllAnalogInputHandle();
	auto i4 = GetAllAnalogOutputHandle();
	auto i5 = __max(__max(i1, i2), __max(i3, i4));
	return ( i5 > 0 )? ADS_OK : ADS_NOTOPEN;
}
//**************************************************************************************************************
//**************************************************************************************************************
void CAdsLineImpl::Close(void)
{
	CAdsControlImpl::Close();
	(void)ReleaseCommstateHandle();
	(void)ReleaseOpModeHandle();
	m_PortAddress = 0U;
}
