//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IOActionType
///
/// @file   IOActionType.h
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

#include "BASE/types.h"

namespace base
{
	static const int32_t cDoseDigitalInputChannel = 8;
	static const int32_t cDoseDigitalOutputChannel = 4;
	static const int32_t cDoseAnalogInputChannel = 1;
	static const int32_t cDoseAnalogOutputChannel = 3;

	static const int32_t cLineDigitalInputChannel  = 12;
	static const int32_t cLineDigitalOutputChannel = 12;
	static const int32_t cLineAnalogInputChannel = 1;
	static const int32_t cLineAnalogOutputChannel = 2;

	enum class eDoseDigitalInputAction : uint32_t
	{
		eInputActionNone = 0,
		eInputActionStart,				// DI0
		eInputActionGravVol,			// DI1
		eInputActionRelease,			// DI3
		eInputActionRefill,				// Switch Refill / TRUE / FALSE
		eInputActionRefillStop,			// Stop Refill
		eInputActionRefillRequest,		// Start Refill
		eInputActionRefillRelease,		// Switch Release
		eInputActionRefillMaxSensor,	// Max-Sensor refill
		eInputActionRefillMinSensor,	// Min-Sensor refill
		eInputActionRefillExtAlarm,
		eInputActionEmptyFeederStart,
		eInputActionEmptyFeederStop,
		eInputActionExtAlarm,
		eInputActionRunControl,			// RunControl
		eInputActionIfsSwitchMax,
		eInputActionIfsSwitchMin,
		eInputActionIfsSwitchHigh,
		eInputActionIfsSwitchLow,
		eInputActionDriveError,
		eInputActionMotorLoad,
		eInputActionDriveErrorPaddle,
		eInputActionDriveErrorRefillFeeder,
		eInputActionMax
	};

	enum class eDoseDigitalOutputAction : uint32_t
	{
		eOutputActionNone = 0,
		eOutputActionOperate,			// DO0
		eOutputActionAlarm,				// DO1
		eOutputActionRefillRequest,		// DO2
		eOutputActionDriveRelease,		// DO3
		eOutputActionEmptyFeeder,		// Not Used
		eOutputActionStart,				// Not Used
		eOutputActionBeltLoadLimit,		// Not Used
		eOutputActionBatchFinished,		// Not Used
		eOutputActionMax
	};

	enum class eDoseAnalogInputAction : uint32_t
	{
		eInputActionNone = 0,
		eInputActionEncoder,
		eInputActionMassflow, // for PID
		eInputActionMax
	};

	enum class eDoseAnalogOutputAction : uint32_t
	{
		eOutputActionNone = 0,
		eOutputActionDriveCommand,
		eOutputActionAgitator,
		eOutputActionMassflow,
		eOutputActionRefillFeeder,
		eOutputActionMax
	};


	enum class eLineDigitalInputAction : uint32_t
	{
		eInputActionNone = 0,
		eInputActionStart,
		eInputActionShutDown,
		eInputActionSetpointExt,
		eInputActionInvSetpointExt,
		eInputActionRampUp,
		eInputActionRampDown,
		eInputActionMF,
		eInputActionSF1,
		eInputActionSF2,
		eInputActionCustomerAlarm,
		eInputActionExtAlarm,
		eInputActionAlarmQuit1,
		eInputActionAlarmQuit03,
		eInputActionWindowsShutDown,
		eInputActionOperateAuto,
		eInputActionOperateManual,
		eInputActionUPSLow,
		eInputActionMax
	};

	enum class eLineDigitalOutputAction : uint32_t
	{
		eOutputActionNone = 0,
		eOutputActionAlarm0,
		eOutputActionAlarm1,
		eOutputActionAlarm2,
		eOutputActionAlarm3,
		eOutputActionLineRelease,			// Linienfreigabe
		eOutputActionTotalizerPulse,		// Totalizer Impulse
		eOutputActionOperateAuto,			// Autmatic mode
		eOutputActionOperateService,			// Manual ( service) mode
		eOutputActionMax
	};

	enum class eLineAnalogInputAction : uint32_t
	{
		eInputActionNone = 0,
		eInputActionSetpoint,
		eInputActionMax
	};

	enum class eLineAnalogOutputAction : uint32_t
	{
		eOutputActionNone = 0,
		eOutputActionMassflow,
		eOutputActionMax
	};


};
