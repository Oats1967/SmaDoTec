//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IOConfigMap
///
/// @file   IOConfigMap.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <string>
#include "XMLStringEnumMap.h"
#include "BASE/include/IOActionType.h"



namespace base
{
    //****************************************************************************************************************************
    //****************************************************************************************************************************
    using TIODigitalDoseInputMap = CXMLStringEnumMap< eDoseDigitalInputAction >;
    class CIODigitalDoseInputMap : public TIODigitalDoseInputMap
    {
        public:
            CIODigitalDoseInputMap() : TIODigitalDoseInputMap(
            {
                { eDoseDigitalInputAction::eInputActionNone, "NotUsed" } ,
                { eDoseDigitalInputAction::eInputActionStart, "Start" } ,
                { eDoseDigitalInputAction::eInputActionGravVol, "GravVol" } ,
                { eDoseDigitalInputAction::eInputActionRelease, "Release" } ,
                { eDoseDigitalInputAction::eInputActionRefill, "Refill" } ,
                { eDoseDigitalInputAction::eInputActionRefillStop, "RefillStop" } ,
                { eDoseDigitalInputAction::eInputActionRefillRequest, "RefillRequest" } ,
                { eDoseDigitalInputAction::eInputActionRefillRelease, "RefillRelease" } ,
                { eDoseDigitalInputAction::eInputActionRefillMinSensor, "RefillMinSensor" } ,
                { eDoseDigitalInputAction::eInputActionRefillMaxSensor, "RefillMaxSensor" } ,
                { eDoseDigitalInputAction::eInputActionRefillExtAlarm, "RefillExtAlarm" } ,
                { eDoseDigitalInputAction::eInputActionEmptyFeederStart, "EmptyFeederStart" } ,
                { eDoseDigitalInputAction::eInputActionEmptyFeederStop, "EmptyFeederStop" } ,
                { eDoseDigitalInputAction::eInputActionExtAlarm, "ExtAlarm" },
                { eDoseDigitalInputAction::eInputActionRunControl, "RunControl" },
                { eDoseDigitalInputAction::eInputActionIfsSwitchMax, "IfsSwitchMax" },
                { eDoseDigitalInputAction::eInputActionIfsSwitchMin, "IfsSwitchMin" },
                { eDoseDigitalInputAction::eInputActionIfsSwitchHigh, "IfsSwitchHigh" },
                { eDoseDigitalInputAction::eInputActionIfsSwitchLow, "IfsSwitchLow" },
                { eDoseDigitalInputAction::eInputActionDriveError, "DriveError" },
                { eDoseDigitalInputAction::eInputActionMotorLoad, "MotorLoad" },
                { eDoseDigitalInputAction::eInputActionDriveErrorPaddle, "DriveErrorPaddle" },
                { eDoseDigitalInputAction::eInputActionDriveErrorRefillFeeder, "DriveErrorRefillFeeder" }
                })
            {
                assert(_S32(m_Map.size()) == _S32(eDoseDigitalInputAction::eInputActionMax));
            }
    };

    //****************************************************************************************************************************
    //****************************************************************************************************************************
    using TIODigitalDoseOutputMap = CXMLStringEnumMap< eDoseDigitalOutputAction >;
    class CIODigitalDoseOutputMap : public TIODigitalDoseOutputMap
    {
        public:
            CIODigitalDoseOutputMap() : TIODigitalDoseOutputMap(
            {
               { eDoseDigitalOutputAction::eOutputActionNone, "NotUsed" } ,
               { eDoseDigitalOutputAction::eOutputActionOperate, "Operate" } ,
               { eDoseDigitalOutputAction::eOutputActionAlarm, "Alarm" } ,
               { eDoseDigitalOutputAction::eOutputActionRefillRequest, "RefillRequest" } ,
               { eDoseDigitalOutputAction::eOutputActionDriveRelease, "DriveRelease" } ,
               { eDoseDigitalOutputAction::eOutputActionEmptyFeeder, "EmptyFeeder" } ,
               { eDoseDigitalOutputAction::eOutputActionStart, "Start" } ,
               { eDoseDigitalOutputAction::eOutputActionBeltLoadLimit, "BeltLoadLimit" } ,
               { eDoseDigitalOutputAction::eOutputActionBatchFinished, "BatchFinished" } ,
            })
            {
                assert(_S32(m_Map.size()) == _S32(eDoseDigitalOutputAction::eOutputActionMax));
            }
    };
    //****************************************************************************************************************************
    //****************************************************************************************************************************
    using TIOAnalogDoseInputMap = CXMLStringEnumMap< eDoseAnalogInputAction >;
    class CIOAnalogDoseInputMap : public TIOAnalogDoseInputMap
    {
        public:
            CIOAnalogDoseInputMap() :TIOAnalogDoseInputMap(
                {
                   { eDoseAnalogInputAction::eInputActionNone, "NotUsed" } ,
                   { eDoseAnalogInputAction::eInputActionEncoder, "Encoder" },
                   { eDoseAnalogInputAction::eInputActionMassflow, "Massflow" }
                })
            {
                assert(_S32(m_Map.size()) == _S32(eDoseAnalogInputAction::eInputActionMax));
            }
    };

    //****************************************************************************************************************************
    //****************************************************************************************************************************
    using TIOAnalogDoseOutputMap = CXMLStringEnumMap< eDoseAnalogOutputAction >;
    class CIOAnalogDoseOutputMap : public TIOAnalogDoseOutputMap
    {
        public:
            CIOAnalogDoseOutputMap() :TIOAnalogDoseOutputMap(
                {
                   { eDoseAnalogOutputAction::eOutputActionNone, "NotUsed" } ,
                   { eDoseAnalogOutputAction::eOutputActionDriveCommand, "DriveCommand" },
                   { eDoseAnalogOutputAction::eOutputActionAgitator, "Agitator" },
                   { eDoseAnalogOutputAction::eOutputActionMassflow, "Massflow" },
                   { eDoseAnalogOutputAction::eOutputActionRefillFeeder, "RefillFeeder" }
                })
            {
                assert(_S32(m_Map.size()) == _S32(eDoseAnalogOutputAction::eOutputActionMax));
            }
    };

    //****************************************************************************************************************************
    //****************************************************************************************************************************
    //****************************************************************************************************************************
    //****************************************************************************************************************************
    //****************************************************************************************************************************
    //****************************************************************************************************************************
    using TIODigitalLineInputMap = CXMLStringEnumMap< eLineDigitalInputAction >;
    class CIODigitalLineInputMap : public TIODigitalLineInputMap
    {
        public:
            CIODigitalLineInputMap() : TIODigitalLineInputMap(
                {
                   { eLineDigitalInputAction::eInputActionNone, "NotUsed" } ,
                   { eLineDigitalInputAction::eInputActionStart, "Start" } ,
                   { eLineDigitalInputAction::eInputActionShutDown, "ShutDown" } ,
                   { eLineDigitalInputAction::eInputActionSetpointExt, "ExtSetpoint" } ,
                   { eLineDigitalInputAction::eInputActionInvSetpointExt, "InvExtSetpoint" } ,
                   { eLineDigitalInputAction::eInputActionRampUp, "SetpointRampUp" } ,
                   { eLineDigitalInputAction::eInputActionRampDown, "SetpointRampDown" } ,
                   { eLineDigitalInputAction::eInputActionMF, "MF" } ,
                   { eLineDigitalInputAction::eInputActionSF1, "SF1" } ,
                   { eLineDigitalInputAction::eInputActionSF2, "SF2" } ,
                   { eLineDigitalInputAction::eInputActionCustomerAlarm, "CustomerAlarm" } ,
                   { eLineDigitalInputAction::eInputActionExtAlarm, "ExtAlarm" },
                   { eLineDigitalInputAction::eInputActionAlarmQuit1, "AlarmQuit1" },
                   { eLineDigitalInputAction::eInputActionAlarmQuit03, "AlarmQuit03" },
                   { eLineDigitalInputAction::eInputActionWindowsShutDown, "WindowsShutDown" },
                   { eLineDigitalInputAction::eInputActionOperateAuto, "OperateAuto" },
                   { eLineDigitalInputAction::eInputActionOperateManual, "OperateManual" },
                   { eLineDigitalInputAction::eInputActionUPSLow, "UPSAlarm"}
                })
            {
                assert(_S32(m_Map.size()) == _S32(eLineDigitalInputAction::eInputActionMax));
            }
    };

    //****************************************************************************************************************************
    //****************************************************************************************************************************
    using TIODigitalLineOutputMap = CXMLStringEnumMap< eLineDigitalOutputAction >;
    class CIODigitalLineOutputMap : public TIODigitalLineOutputMap
    {
        public:
            CIODigitalLineOutputMap() :TIODigitalLineOutputMap(
                {
                   { eLineDigitalOutputAction::eOutputActionNone, "NotUsed" } ,
                   { eLineDigitalOutputAction::eOutputActionAlarm0, "Alarm0" } ,
                   { eLineDigitalOutputAction::eOutputActionAlarm1, "Alarm1" } ,
                   { eLineDigitalOutputAction::eOutputActionAlarm2, "Alarm2" } ,
                   { eLineDigitalOutputAction::eOutputActionAlarm3, "Alarm3" } ,
                   { eLineDigitalOutputAction::eOutputActionLineRelease, "LineRelease" } ,
                   { eLineDigitalOutputAction::eOutputActionTotalizerPulse, "TotalizerPulse" } ,
                   { eLineDigitalOutputAction::eOutputActionOperateAuto, "OperateAuto" },
                   { eLineDigitalOutputAction::eOutputActionOperateService, "OperateService" },
                })
            {
                assert(_S32(m_Map.size()) == _S32(eLineDigitalOutputAction::eOutputActionMax));
            }
    };

    //****************************************************************************************************************************
    //****************************************************************************************************************************
    using TIOAnalogLineInputMap = CXMLStringEnumMap< eLineAnalogInputAction >;
    class CIOAnalogLineInputMap : public TIOAnalogLineInputMap
    {
        public:
            CIOAnalogLineInputMap() : TIOAnalogLineInputMap(
                {
                   { eLineAnalogInputAction::eInputActionNone, "NotUsed" } ,
                   { eLineAnalogInputAction::eInputActionSetpoint, "Setpoint" }
                })
            {
                assert(_S32(m_Map.size()) == _S32(eLineAnalogInputAction::eInputActionMax));
            }
    };

    //****************************************************************************************************************************
    //****************************************************************************************************************************
    using TIOAnalogLineOutputMap = CXMLStringEnumMap< eLineAnalogOutputAction >;
    class CIOAnalogLineOutputMap : public TIOAnalogLineOutputMap
    {
        public:
            CIOAnalogLineOutputMap() : TIOAnalogLineOutputMap(
            {
               { eLineAnalogOutputAction::eOutputActionNone, "NotUsed" } ,
               { eLineAnalogOutputAction::eOutputActionMassflow, "Massflow" },
            })
            {
                assert(_S32(m_Map.size()) == _S32(eLineAnalogOutputAction::eOutputActionMax));
            }
    };



};