//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDoseImpl.h
///
/// @file   AdsDoseImpl.h
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

#include <array>
#include <map>
#include "BASE/include/IOConfig.h"
#include "AdsClient/src/private/AdsControlImpl.h"
#include "AdsClient/src/private/AdsDoseImplMacro.h"


class CAdsDoseImpl : public CAdsControlImpl
{
	enum
	{
		eINPUTS_DSIO_DI0 = 0,
		eINPUTS_DSIO_AI0 = eINPUTS_DSIO_DI0   + ( base::cDoseDigitalInputChannel  * GLOBALDOSEMAXCOUNT ),
		eOUTPUTS_DSIO_DO0 = eINPUTS_DSIO_AI0  + ( base::cDoseAnalogInputChannel   * GLOBALDOSEMAXCOUNT ),
		eOUTPUTS_DSIO_AO0 = eOUTPUTS_DSIO_DO0 + ( base::cDoseDigitalOutputChannel * GLOBALDOSEMAXCOUNT ),
		eINPUTS_DSIO_LI0  = eOUTPUTS_DSIO_AO0 + ( base::cDoseAnalogOutputChannel  * GLOBALDOSEMAXCOUNT ), 
		eMAXHANDLE = eINPUTS_DSIO_LI0 + GLOBALDOSEMAXCOUNT
	};

private:
	std::array<uint32_t, eMAXHANDLE> m_Handle;
	std::map<uint32_t, uint32_t>     m_CounterValue;
	base::CIOFeederConfig			 m_IOConfig;

	int32_t				m_GlobalItems;
	uint16_t			m_PortAddress;

private:
	int32_t RegisterRunControlInput(const int32_t index);
	int32_t GetAnalogInputHandle(const int32_t inputchannel, const int32_t index);
	int32_t GetAnalogOutputHandle(const int32_t outputchannel, const int32_t index);
	int32_t GetDigitalInputHandle(const int32_t inputchannel, const int32_t index);
	int32_t GetDigitalOutputHandle(const int32_t outputchannel, const int32_t index);
	int32_t GetDigitalLogoInputHandle(const int32_t inputchannel);

	int32_t GetAllDigitalInputHandle(const int32_t index);
	int32_t GetAllDigitalOutputHandle(const int32_t index);
	int32_t GetAllAnalogInputHandle(const int32_t index);
	int32_t GetAllAnalogOutputHandle(const int32_t index);

	int32_t Init(void);

	int32_t GetDigitalInput(const int32_t index, const base::eDoseDigitalInputAction type, BOOL& value, const BOOL bDefault = FALSE);
	int32_t GetAnalogInput(const int32_t index, const base::eDoseAnalogInputAction type, float32_t& value);
	int32_t SetDigitalOutput(const int32_t index, const base::eDoseDigitalOutputAction type, const BOOL value);
	int32_t SetAnalogOutput(const int32_t index, const base::eDoseAnalogOutputAction type, const float32_t value);

	CAdsDoseImpl() = delete;

public:
	explicit CAdsDoseImpl(const base::CIOFeederConfig& cfg);

	virtual ~CAdsDoseImpl() = default;

	//------------------------------------------------------------------------------------
	///  @brief   AdsDoseImplOpen :  This method opens the port for dose items
	/// 
	///  @param[in] iPortAddress, portaddress
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t Open(const uint16_t iPortAddress);

	//------------------------------------------------------------------------------------
	///  @brief   AdsDoseImplClose : This method closes the port for dose items
	///
	///  @param[in] indexone
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	void Close(void) override;

	int32_t GetPulsCounter(const int32_t index, uint32_t& value);

	DECLARE_DI(Release)
	DECLARE_DI(Start);
	DECLARE_DI(GravVol)
	DECLARE_DI(Refill)
	DECLARE_DI(RefillStop)
	DECLARE_DI(RefillRequest)
	DECLARE_DI(RefillRelease)
	DECLARE_DI(RefillMaxSensor)
	DECLARE_DI(RefillMinSensor)
	DECLARE_DI(RefillExtAlarm)
	DECLARE_DI(EmptyFeederStart)
	DECLARE_DI(EmptyFeederStop)
	DECLARE_DI(ExtAlarm)

	DECLARE_DI(IfsSwitchHigh)
	DECLARE_DI(IfsSwitchLow)
	DECLARE_DI(IfsSwitchMax)
	DECLARE_DI(IfsSwitchMin)

	DECLARE_DI(DriveError)
	DECLARE_DI(MotorLoad)
	DECLARE_DI(DriveErrorPaddle)
	DECLARE_DI(DriveErrorRefillFeeder)

	DECLARE_DO(Operate)
	DECLARE_DO(Alarm)
	DECLARE_DO(RefillRequest)
	DECLARE_DO(DriveRelease)
	DECLARE_DO(EmptyFeeder)
	DECLARE_DO(Start)
	DECLARE_DO(BeltLoadLimit)
	DECLARE_DO(BatchFinished)

	DECLARE_AO(DriveCommand)
	DECLARE_AO(Agitator)
	DECLARE_AO(Massflow)
	DECLARE_AO(RefillFeeder)

	DECLARE_AI(Encoder)
	DECLARE_AI(Massflow)
};


