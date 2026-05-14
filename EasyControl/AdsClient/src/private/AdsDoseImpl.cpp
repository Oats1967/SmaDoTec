//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsDoseImpl
///
/// @file   AdsDoseImpl.cpp
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
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "BASE/types.h"
#include "AdsClient/include/AdsTypen.h"
#include "AdsClient/src/private/AdsDoseImpl.h"
#include "AdsClient/src/private/AdsHelper.h"


//**************************************************************************************************************
//**************************************************************************************************************
CAdsDoseImpl::CAdsDoseImpl(const base::CIOFeederConfig& cfg) : m_IOConfig{ cfg }
,m_GlobalItems(0)
, m_PortAddress(0)
{
	m_Handle.fill(0);
}
//**************************************************************************************************************
//**************************************************************************************************************
inline int32_t CAdsDoseImpl::GetDigitalInput(const int32_t index, const base::eDoseDigitalInputAction type, BOOL& value, const BOOL bDefault)
{
	int32_t result = ADS_OK;

	auto channel = m_IOConfig[index].getDI().Get(type);
	if (channel >= 0)
	{
		value = m_DigitalInput.getInput(m_Handle[eINPUTS_DSIO_DI0 + (GLOBALDOSEMAXCOUNT * channel) + index]);
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
inline int32_t CAdsDoseImpl::GetAnalogInput(const int32_t index, const base::eDoseAnalogInputAction type, float32_t& value)
{
	int32_t result = ADS_OK;

	auto channel = m_IOConfig[index].getAI().Get(type);
	if (channel >= 0)
	{
		value = m_AnalogInput.getInput(m_Handle[eINPUTS_DSIO_AI0 + (GLOBALDOSEMAXCOUNT * channel) + index]);
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
inline int32_t CAdsDoseImpl::SetDigitalOutput(const int32_t index, const base::eDoseDigitalOutputAction type, const BOOL value)
{
	int32_t result = ADS_OK;

	auto channel = m_IOConfig[index].getDO().Get(type);
	if (channel >= 0)
	{
		m_DigitalOutput.setOutput(m_Handle[eOUTPUTS_DSIO_DO0 + (GLOBALDOSEMAXCOUNT * channel) + index], value);
	}
	else
	{
		result = ADS_MPM_NOK;
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
inline int32_t CAdsDoseImpl::SetAnalogOutput(const int32_t index, const base::eDoseAnalogOutputAction type, const float32_t value)
{
	int32_t result = ADS_OK;

	auto channel = m_IOConfig[index].getAO().Get(type);
	if (channel >= 0)
	{
		m_AnalogOutput.setOutput(m_Handle[eOUTPUTS_DSIO_AO0 + (GLOBALDOSEMAXCOUNT * channel) + index], value);
	}
	else
	{
		result = ADS_MPM_NOK;
	}
	return result;
}

//**************************************************************************************************************
//**************************************************************************************************************
#ifdef __SIM_RELEASE
SIM_DI(CAdsDoseImpl, Release)
#else
IMPL_DI_TRUE(CAdsDoseImpl, Release, base::eDoseDigitalInputAction::eInputActionRelease);
#endif
IMPL_DI(CAdsDoseImpl, Start, base::eDoseDigitalInputAction::eInputActionStart);
IMPL_DI(CAdsDoseImpl, GravVol, base::eDoseDigitalInputAction::eInputActionGravVol)
IMPL_DI(CAdsDoseImpl, Refill, base::eDoseDigitalInputAction::eInputActionRefill)
IMPL_DI(CAdsDoseImpl, RefillStop, base::eDoseDigitalInputAction::eInputActionRefillStop)
IMPL_DI(CAdsDoseImpl, RefillRequest, base::eDoseDigitalInputAction::eInputActionRefillRequest)
IMPL_DI(CAdsDoseImpl, RefillRelease, base::eDoseDigitalInputAction::eInputActionRefillRelease)
IMPL_DI(CAdsDoseImpl, RefillMinSensor, base::eDoseDigitalInputAction::eInputActionRefillMinSensor)
IMPL_DI(CAdsDoseImpl, RefillMaxSensor, base::eDoseDigitalInputAction::eInputActionRefillMaxSensor)
IMPL_DI(CAdsDoseImpl, RefillExtAlarm, base::eDoseDigitalInputAction::eInputActionRefillExtAlarm)
IMPL_DI(CAdsDoseImpl, EmptyFeederStart, base::eDoseDigitalInputAction::eInputActionEmptyFeederStart)
IMPL_DI(CAdsDoseImpl, EmptyFeederStop, base::eDoseDigitalInputAction::eInputActionEmptyFeederStop)
IMPL_DI(CAdsDoseImpl, ExtAlarm, base::eDoseDigitalInputAction::eInputActionExtAlarm)
IMPL_DI(CAdsDoseImpl, IfsSwitchHigh, base::eDoseDigitalInputAction::eInputActionIfsSwitchHigh)
IMPL_DI(CAdsDoseImpl, IfsSwitchLow, base::eDoseDigitalInputAction::eInputActionIfsSwitchLow)
IMPL_DI(CAdsDoseImpl, IfsSwitchMax, base::eDoseDigitalInputAction::eInputActionIfsSwitchMax)
IMPL_DI(CAdsDoseImpl, IfsSwitchMin, base::eDoseDigitalInputAction::eInputActionIfsSwitchMin)
IMPL_DI(CAdsDoseImpl, DriveError, base::eDoseDigitalInputAction::eInputActionDriveError)
IMPL_DI(CAdsDoseImpl, MotorLoad, base::eDoseDigitalInputAction::eInputActionMotorLoad)
IMPL_DI(CAdsDoseImpl, DriveErrorPaddle, base::eDoseDigitalInputAction::eInputActionDriveErrorPaddle)
IMPL_DI(CAdsDoseImpl, DriveErrorRefillFeeder, base::eDoseDigitalInputAction::eInputActionDriveErrorRefillFeeder)


IMPL_DO(CAdsDoseImpl, Operate, base::eDoseDigitalOutputAction::eOutputActionOperate)
IMPL_DO(CAdsDoseImpl, Alarm, base::eDoseDigitalOutputAction::eOutputActionAlarm)
IMPL_DO(CAdsDoseImpl, RefillRequest, base::eDoseDigitalOutputAction::eOutputActionRefillRequest)
IMPL_DO(CAdsDoseImpl, DriveRelease, base::eDoseDigitalOutputAction::eOutputActionDriveRelease)
IMPL_DO(CAdsDoseImpl, EmptyFeeder, base::eDoseDigitalOutputAction::eOutputActionEmptyFeeder)
IMPL_DO(CAdsDoseImpl, Start, base::eDoseDigitalOutputAction::eOutputActionStart)
IMPL_DO(CAdsDoseImpl, BeltLoadLimit, base::eDoseDigitalOutputAction::eOutputActionBeltLoadLimit)
IMPL_DO(CAdsDoseImpl, BatchFinished, base::eDoseDigitalOutputAction::eOutputActionBatchFinished)

IMPL_AO(CAdsDoseImpl, DriveCommand, base::eDoseAnalogOutputAction::eOutputActionDriveCommand)
IMPL_AO(CAdsDoseImpl, Agitator, base::eDoseAnalogOutputAction::eOutputActionAgitator)
IMPL_AO(CAdsDoseImpl, Massflow, base::eDoseAnalogOutputAction::eOutputActionMassflow)
IMPL_AO(CAdsDoseImpl, RefillFeeder, base::eDoseAnalogOutputAction::eOutputActionRefillFeeder)

IMPL_AI(CAdsDoseImpl, Encoder, base::eDoseAnalogInputAction::eInputActionEncoder)
IMPL_AI(CAdsDoseImpl, Massflow, base::eDoseAnalogInputAction::eInputActionMassflow)

//**************************************************************************************************************
//**************************************************************************************************************
inline int32_t CAdsDoseImpl::GetAnalogInputHandle(const int32_t inputchannel, const int32_t index)
{
	char_t szSymbol[64];

	assert((inputchannel >= 0) && (inputchannel < base::cDoseAnalogInputChannel));
	assert(m_PortAddress > 0);
	sprintf_s(szSymbol, "DSIO.INPUTS.AI%d_%d", index, inputchannel );
	const int32_t hdlIndex = eINPUTS_DSIO_AI0 + (inputchannel * GLOBALDOSEMAXCOUNT) + index;
	m_Handle[hdlIndex] = AddAnalogInput(szSymbol, m_PortAddress);
#ifdef _DEBUG
	if ( ! m_Handle[hdlIndex] )
	{
		LOGDEBUG("Invalid handle " << szSymbol);
	}
#endif
	return (m_Handle[hdlIndex] == 0) ? ADS_NOTOPEN : ADS_OK;
}
//**************************************************************************************************************
//**************************************************************************************************************
inline int32_t CAdsDoseImpl::GetAnalogOutputHandle(const int32_t outputchannel, const int32_t index)
{
	char_t szSymbol[64];

	assert((outputchannel >= 0) && (outputchannel < base::cDoseAnalogOutputChannel));
	assert(m_PortAddress > 0);
	sprintf_s(szSymbol, "DSIO.OUTPUTS.AO%d_%d", index, outputchannel );
	const int32_t hdlIndex = eOUTPUTS_DSIO_AO0 + (outputchannel * GLOBALDOSEMAXCOUNT) + index;
	m_Handle[hdlIndex] = AddAnalogOutput(szSymbol, m_PortAddress);
#ifdef _DEBUG
	if (!m_Handle[hdlIndex])
	{
		LOGDEBUG("Invalid handle " << szSymbol);
	}
#endif
	return (m_Handle[hdlIndex] == 0) ? ADS_NOTOPEN : ADS_OK;
}
//**************************************************************************************************************
//**************************************************************************************************************
inline int32_t CAdsDoseImpl::GetDigitalLogoInputHandle(const int32_t index)
{
	char_t szSymbol[64];

	assert(m_PortAddress > 0);
	sprintf_s(szSymbol, "DSIO.INPUTS.LI%d", index);
	const int32_t hdlIndex = eINPUTS_DSIO_LI0 + index;
	m_Handle[hdlIndex] = AddDigitalInput(szSymbol, m_PortAddress, [this](const uint32_t hdl, BOOL)
		{
			BOOL result = FALSE;
			try
			{
				//LOGDEBUG("Received callback !");
				m_CounterValue[hdl]++;
				result = TRUE;
			}
			catch (...)
			{
				LOGERROR("Handle not found !" << hdl);
			}
			return result;
		});
#ifdef _DEBUG
	if (!m_Handle[hdlIndex])
	{
		LOGDEBUG("Invalid handle " << szSymbol);
	}
#endif
	if (m_Handle[hdlIndex])
	{
		assert(m_CounterValue.find(m_Handle[hdlIndex]) == m_CounterValue.cend());
		m_CounterValue.insert({ m_Handle[hdlIndex], 0 } );
	}
	return (m_Handle[hdlIndex] == 0) ? ADS_NOTOPEN : ADS_OK;
}

//**************************************************************************************************************
//**************************************************************************************************************
inline int32_t CAdsDoseImpl::GetDigitalInputHandle(const int32_t inputchannel, const int32_t index)
{
	char_t szSymbol[64];

	assert((inputchannel >= 0) && (inputchannel < base::cDoseDigitalInputChannel));
	assert(m_PortAddress > 0);
	sprintf_s(szSymbol, "DSIO.INPUTS.DI%d_%d", index, inputchannel );
	const int32_t hdlIndex = eINPUTS_DSIO_DI0 + (inputchannel * GLOBALDOSEMAXCOUNT) + index;
	m_Handle[hdlIndex] = AddDigitalInput(szSymbol, m_PortAddress);
#ifdef _DEBUG
	if (!m_Handle[hdlIndex])
	{
		LOGDEBUG("Invalid handle " << szSymbol);
	}
#endif
	return (m_Handle[hdlIndex] == 0) ? ADS_NOTOPEN : ADS_OK;
}
//**************************************************************************************************************
//**************************************************************************************************************
inline int32_t CAdsDoseImpl::GetDigitalOutputHandle(const int32_t outputchannel, const int32_t index)
{
	char_t szSymbol[64];

	assert((outputchannel >= 0) && (outputchannel < base::cDoseDigitalOutputChannel));
	assert(m_PortAddress > 0);
	sprintf_s(szSymbol, "DSIO.OUTPUTS.DO%d_%d", index, outputchannel );
	const int32_t hdlIndex = eOUTPUTS_DSIO_DO0 + (outputchannel * GLOBALDOSEMAXCOUNT) + index;
	m_Handle[hdlIndex] = AddDigitalOutput(szSymbol, m_PortAddress);
#ifdef _DEBUG
	if (!m_Handle[hdlIndex])
	{
		LOGDEBUG("Invalid handle " << szSymbol);
	}
#endif
	return (m_Handle[hdlIndex] == 0) ? ADS_NOTOPEN : ADS_OK;
}
//**************************************************************************************************************
//**************************************************************************************************************
int32_t CAdsDoseImpl::GetAllDigitalInputHandle(const int32_t index)
{
	int32_t inputchannel = 0;
	const auto& rCfg = m_IOConfig[index].getDI();
	assert(base::cDoseDigitalInputChannel == _S32(rCfg.Count()));
	for (int32_t i = 0; i < _S32(rCfg.Count()); i++)
	{
		auto result = GetDigitalInputHandle(i, index);
		if (result == ADS_OK)
		{
			inputchannel++;
		}
		else
		{
			LOGDEBUG("GetDigitalInputHandle failed !, inputchannel = " << i << " index = " << index);
		}
	}
	return inputchannel;
}
//**************************************************************************************************************
//**************************************************************************************************************
int32_t CAdsDoseImpl::GetAllDigitalOutputHandle(const int32_t index)
{
	int32_t outputchannel = 0;
	const auto& rCfg = m_IOConfig[index].getDO();
	for (int32_t i = 0; i < _S32(rCfg.Count()); i++)
	{
		int32_t result = GetDigitalOutputHandle(i, index);
		if (result == ADS_OK)
		{
			outputchannel++;
		}
		else
		{
			LOGDEBUG("GetDigitalOutputHandle failed !, outputchannel = " << i << " index = " << index);
		}
	}
	return outputchannel;
}
//**************************************************************************************************************
//**************************************************************************************************************
int32_t CAdsDoseImpl::GetAllAnalogInputHandle(const int32_t index)
{
	int32_t inputchannel = 0;
	const auto& rCfg = m_IOConfig[index].getAI();
	for (int32_t i = 0; i < _S32(rCfg.Count()); i++)
	{
		int32_t result = GetAnalogInputHandle(i, index);
		if (result == ADS_OK)
		{
			inputchannel++;
		}
		else
		{
			LOGDEBUG("GetAnalogInputHandle failed !, inputchannel = " << i << " index = " << index);
		}
	}
	return inputchannel;
}
//**************************************************************************************************************
//**************************************************************************************************************
int32_t CAdsDoseImpl::GetAllAnalogOutputHandle(const int32_t index)
{
	int32_t outputchannel = 0;
	const auto& rCfg = m_IOConfig[index].getAO();
	for (int32_t i = 0; i < _S32(rCfg.Count()); i++)
	{
		int32_t result = GetAnalogOutputHandle(i, index);
		if (result == ADS_OK)
		{
			outputchannel++;
		}
		else
		{
			LOGDEBUG("GetAnalogOutputHandle failed !, outputchannel = " << i << " index = " << index);
		}
	}
	return outputchannel;
}
//***********************************************************************************************************************
//***********************************************************************************************************************
int32_t CAdsDoseImpl::GetPulsCounter(const int32_t index, uint32_t& value)
{
	int32_t result = !ADS_OK;

	auto rHandle = m_Handle[_S32(eINPUTS_DSIO_LI0 + index)];
	if (rHandle == 0)
	{
		auto channel = m_IOConfig[index].getDI().Get(base::eDoseDigitalInputAction::eInputActionRunControl);
		// assert(channel >= 0);
		if (channel >= 0)
		{
			rHandle = m_Handle[eINPUTS_DSIO_DI0 + (channel * GLOBALDOSEMAXCOUNT) + index];
		}
	}
	if (rHandle)
	{
		// assert(rHandle != 0);
		assert(m_CounterValue.find(rHandle) != m_CounterValue.cend());
		value = m_CounterValue[rHandle];
		result = ADS_OK;
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
int32_t CAdsDoseImpl::RegisterRunControlInput(const int32_t index)
{
	const auto& rCfg = m_IOConfig[index].getDI();
	auto channel = rCfg.Get(base::eDoseDigitalInputAction::eInputActionRunControl);
	if (channel >= 0)
	{
		const int32_t hdlIndex = eINPUTS_DSIO_DI0 + (channel * GLOBALDOSEMAXCOUNT) + index;
		if (m_Handle[hdlIndex])
		{
			assert(m_CounterValue.find(m_Handle[hdlIndex]) == m_CounterValue.cend());
			m_CounterValue.insert({ m_Handle[hdlIndex], 0 });
			m_DigitalInput.registerCallback(m_Handle[hdlIndex], [this](const uint32_t hdl, BOOL )
				{
					BOOL result = FALSE;
					try
					{
						//LOGDEBUG("Received callback !");
						m_CounterValue[hdl]++;
						result = TRUE;
					}
					catch (...)
					{
						LOGERROR("Handle not found !" << hdl);
					}
					return result;
				});
		}
	}
	return ADS_OK;
}


//**************************************************************************************************************
//**************************************************************************************************************
int32_t CAdsDoseImpl::Init ( void )
{
	for (int32_t i = 0; i < m_GlobalItems; i++)
	{
		SetAlarm(i, FALSE);
	}
	return ADS_OK;
}

//**************************************************************************************************************
//**************************************************************************************************************
int32_t CAdsDoseImpl::Open ( const uint16_t iPortAddress )
{
	assert(m_GlobalItems == 0);
	if (m_PortAddress != 0)
	{
		return ADS_OK;
	}
	m_PortAddress = iPortAddress;
	m_GlobalItems = 0;
	for (int32_t index = 0; index < GLOBALDOSEMAXCOUNT; index++ )
	{
		auto diTemp = GetAllDigitalInputHandle(index);
		BOOL bResult = (diTemp > 0);
		auto doTemp = GetAllDigitalOutputHandle(index);
		bResult		&= (doTemp > 0);
		if ( ! bResult)
		{
			break;
		}
#if _DEBUG
		auto aiTemp = GetAllAnalogInputHandle(index);
		auto aoTemp = GetAllAnalogOutputHandle(index);
		LOGDEBUG("Dose" << index + 1 << " : " << "DI = " << diTemp << " DO = " << doTemp << " AI = " << aiTemp << " AO = " << aoTemp);
#else
		GetAllAnalogInputHandle(index);
		GetAllAnalogOutputHandle(index);
#endif
		GetDigitalLogoInputHandle(index);
		RegisterRunControlInput(index);
		m_GlobalItems++;
	}
	LOGDEBUG("m_GlobalItems = " << m_GlobalItems);
	Init ();
	return ( m_GlobalItems > 0) ? ADS_OK : ADS_MPM_NOK;
}
//**************************************************************************************************************
//**************************************************************************************************************
void CAdsDoseImpl::Close( void )
{
	CAdsControlImpl::Close();
	m_GlobalItems = 0;
	m_PortAddress = 0;
}

