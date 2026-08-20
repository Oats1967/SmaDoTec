//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsLineImpl.h
///
/// @file   AdsLineImpl.h
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
#include "BASE/types.h"
#include "BASE/include/IOConfig.h"
#include "AdsClient/src/private/AdsControlImpl.h"
#include "AdsClient/src/private/AdsLineImplMacro.h"


class CAdsLineImpl : public CAdsControlImpl
{
	enum
	{
		eINPUTS_EXIO_COMMSTATE,
		eINPUTS_EXIO_OPMODE,
		eINPUTS_EXIO_DI0,// digital input
		eINPUTS_EXIO_AI0  = eINPUTS_EXIO_DI0   + base::cLineDigitalInputChannel, // analog input
		eOUTPUTS_EXIO_DO0 = eINPUTS_EXIO_AI0   + base::cLineAnalogInputChannel,
		eOUTPUTS_EXIO_AO0 = eOUTPUTS_EXIO_DO0  + base::cLineDigitalOutputChannel, // analog output
		eMAXHANDLE		  = eOUTPUTS_EXIO_AO0  + base::cLineAnalogOutputChannel + GLOBALEXTRUDERANZAHL,
	};

private:
	base::CIOLineConfig m_LineConfig;
	std::array<uint32_t, eMAXHANDLE> m_Handle;
	uint16_t						 m_PortAddress;

private:
	//-------------------------------------------------------------------------------------
	///  @brief   GetAnalogInputHandle
	///
	///  @param[in] inputchannel [ 0 .. ( kAnalogInputChannel-1) ]
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t GetAnalogInputHandle(const int32_t inputchannel);

	//------------------------------------------------------------------------------------
	///  @brief   GetAnalogOutputHandle
	///
	///  @param[in] outputchannel [ 0 .. ( kAnalogOutputChannel-1) ]
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t GetAnalogOutputHandle(const int32_t outputchannel);

	int32_t GetDigitalInputHandle(const int32_t inputchannel);
	//------------------------------------------------------------------------------------
	///  @brief   AdsLineImplGetDigitalOutputHandle
	///
	///  @param[in] outputchannel [ 0 .. ( kDigitalOutputChannel-1) ]
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t GetDigitalOutputHandle(const int32_t outputchannel);

	//------------------------------------------------------------------------------------
	///  @brief   AdsLineImplGetCommstateHandle
	///
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t GetCommstateHandle(void);

	//------------------------------------------------------------------------------------
	///  @brief   AdsLineImplGetOpModeHandle
	///
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t GetOpModeHandle(void);

	///  @brief   AdsLineImplReleaseCommstateHandle
	///
	///  @param[in] none
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t ReleaseCommstateHandle();
	//------------------------------------------------------------------------------------
	///  @brief   AdsLineImplReleaseOpModeHandle
	///
	///  @param[in] none
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t ReleaseOpModeHandle();
	//------------------------------------------------------------------------------------
	///  @brief   AdsLineImplGetAllDigitalInputHandle
	///
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t GetAllDigitalInputHandle();
	//------------------------------------------------------------------------------------
	///  @brief   AdsLineImplGetAllDigitalOutputHandle
	///
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t GetAllDigitalOutputHandle();
	//------------------------------------------------------------------------------------
	///  @brief   AdsLineImplGetAllAnalogInputHandle
	///
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t GetAllAnalogInputHandle();
	//------------------------------------------------------------------------------------
	///  @brief   AdsLineImplGetAllAnalogOutputHandle
	///
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	int32_t GetAllAnalogOutputHandle();

	int32_t GetDigitalInput(const base::eLineDigitalInputAction type, BOOL& value, const BOOL bDefault = FALSE);
	int32_t GetAnalogInput(const base::eLineAnalogInputAction type, float32_t& value);
	int32_t SetDigitalOutput(const base::eLineDigitalOutputAction type, const BOOL value);
	int32_t SetAnalogOutput(const base::eLineAnalogOutputAction type, const float32_t value);


	CAdsLineImpl() = delete;

public:
	explicit CAdsLineImpl(const base::CIOLineConfig& cfg);
	virtual ~CAdsLineImpl() = default;

	int32_t Open(const uint16_t iPortAddress);
	void Close(void);

	int32_t GetCommunicationState(uint16_t & value);
	int32_t GetOpMode(BOOL & value);

	DECLARE_LINE_DI(Start)
	//DECLARE_LINE_DI(Stop)
	DECLARE_LINE_DI(OperateAuto)
	DECLARE_LINE_DI(OperateManual)
	DECLARE_LINE_DI(ShutDown)
	DECLARE_LINE_DI(WindowsShutDown)
	DECLARE_LINE_DI(SetpointExt)		// SensorlevelMode
	DECLARE_LINE_DI(InvSetpointExt)		// Inverser Setpoint
	DECLARE_LINE_DI(RampUp)
	DECLARE_LINE_DI(RampDown)
	DECLARE_LINE_DI(MF)
	DECLARE_LINE_DI(SF1)
	DECLARE_LINE_DI(SF2)
	DECLARE_LINE_DI(CustomerAlarm)  // Kundenalarm
	DECLARE_LINE_DI(ExtAlarm)  // Kundenalarm
	DECLARE_LINE_DI(AlarmQuit1)
	DECLARE_LINE_DI(AlarmQuit03)
	DECLARE_LINE_DI(UPSLow)


	DECLARE_LINE_DO(Alarm0)
	DECLARE_LINE_DO(Alarm1)
	DECLARE_LINE_DO(Alarm2)
	DECLARE_LINE_DO(Alarm3)
	DECLARE_LINE_DO(TotalizerPulse)
	DECLARE_LINE_DO(Release)
	DECLARE_LINE_DO(OperateAuto)
	DECLARE_LINE_DO(OperateService)
	DECLARE_LINE_DO(WatchDog)

	DECLARE_LINE_AI(Setpoint)
	DECLARE_LINE_AO(Massflow)

};



