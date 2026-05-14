//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteInterfaceTypes
///
/// @file   RemoteInterfaceTypes.h
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
union LCStatus
{
    struct
    {
        uint32_t LCenable : 1;      // 1  : LC is enabled
        uint32_t LCactive : 1;      // 2  : LC is active
    } flags;
    uint32_t allflags = 0;
};


union LineProcessStatus
{
    struct
    {
        uint32_t IOalarmshutdown            : 1;      // 0  : shut down alarm extern
        uint32_t IOWindowsShutDown          : 1;      // 1  : windows shut down
        uint32_t IOExtlineSetpoint          : 1;      // 2  : IO - externer Sollwert
        uint32_t StateExtlineSetpoint       : 1;      // 3  : ext. setpoint active, user modified
        uint32_t IOExtlineInvSetpointEnable : 1;      // 4  : ext. linesetpoint inverse enabled
        uint32_t StateExtLineInvSetpoint    : 1;      // 5  : linesetpoint inverse state
        uint32_t IOrampdown                 : 1;      // 6  : ramp down extern
        uint32_t IOrampup                   : 1;      // 7  : ramp up extern
        uint32_t IOlinestart                : 1;      // 8  : start extern 
        uint32_t linestart                  : 1;      // 9  : start Freigabe durch MBLineStart / MBLineStop
        uint32_t IOmainfeeder               : 1;      // 10  : Main feeder extern
        uint32_t IOsidefeeder1              : 1;      // 11 : Side feeder 1 extern
        uint32_t IOsidefeeder2              : 1;      // 12 : Side feeder 2 extern
        uint32_t IOExtalarm                 : 1;      // 13 : IO Extalarm
        uint32_t IOcustomeralarm            : 1;      // 14 : IO Kundenalarm
        uint32_t IOUPSLowalarm              : 1;      // 15 : IO UPS low
        uint32_t IOSyncError                : 1;      // 16 : IO SyncError
        uint32_t anydosestarted             : 1;      // 17 : Any dosing started              ( line + local )
        uint32_t anylinedosestarted         : 1;      // 18 : Any dosing unit in line started ( only line )
        uint32_t linecontrolstarted         : 1;      // 19 : line started                    ( linecontrol running )
        uint32_t servicemode                : 1;      // 20 : service mode
        uint32_t lineReleaseState           : 1;      // 21 : Line release output set
        uint32_t ModuleExternalSetpoint     : 1;      // 22 : Enable Module external setpoint
        uint32_t ModuleSetpointRamp         : 1;      // 23 : Enable Module setpoint ramp
        uint32_t ModuleLineSetpoint         : 1;      // 24 : Enable Module line setpoint
        uint32_t ModuleLocalSetpoint        : 1;      // 25 : Enable Module Local setpoint
        uint32_t ModuleBatcher              : 1;      // 26 : Enable Module Batcher
        uint32_t ModuleTotalizer            : 1;      // 27 : Enable Module Totalizer
        uint32_t ModuleTotalizerPulse       : 1;      // 28 : Enable Module Totalizer Impulse
        uint32_t LineReleaseEnable          : 1;      // 29 : Enable Line Release
        uint32_t LineStartEnable            : 1;      // 30 : Enable Line Start
        uint32_t IOTotalizerPulseOutput     : 1;      // 31 : IO Totalizerpulse
        uint32_t IOManualOperate            : 1;      // 32 : manual operate mode
        uint32_t IOAutoOperate              : 1;      // 33 : automatic operate mode
    } flags;
    uint64_t allflags = 0;
};

union LineAlarmStatus
{
    struct
    {
        uint32_t IOalarm1quit   : 1;      // quit alarm 1 extern
        uint32_t alarm1quit     : 1;
        uint32_t IOalarm03quit  : 1;      // quit alarm 0-3 extern
        uint32_t alarm03quit    : 1;
        uint32_t alarm0         : 1;      // alarm 0 aktiviert
        uint32_t alarm1         : 1;      // alarm 1 aktiviert
        uint32_t alarm2         : 1;      // alarm 2 aktiviert
        uint32_t alarm3         : 1;      // alarm 3 aktiviert
    } flags;
    uint32_t allflags = 0;
};

union ProcessStatus
{
    struct
    {
        uint32_t refillActive          : 1;      // 0  : refill active
        uint32_t running               : 1;      // 1  : operating
        uint32_t started               : 1;      // 2  : started
        uint32_t alarmshutdownext      : 1;      // 3  : alarm shutdown extern
        uint32_t alarmactive           : 1;      // 4  : alarm active
        uint32_t gravimetric           : 1;      // 5  : volumetric = 0, grav. = 1
        uint32_t lineMode              : 1;      // 6  : local = 0, line= 1
        uint32_t calibrateActive       : 1;      // 7  : calibration active
        uint32_t refillRelease         : 1;      // 8  : refill release
        uint32_t refillTimeOut         : 1;      // 9  : refill timeout
        uint32_t customeralarm         : 1;      // 10 : customer alarm
        uint32_t vorlagenlarm          : 1;      // 11 : vorlagen alarm
        uint32_t IOUPSLowalarm         : 1;      // 12 : USV low alarm
        uint32_t initialized           : 1;      // 13 : initialized
        uint32_t releaseext            : 1;      // 14 : Externer Freigabe input
        uint32_t startext              : 1;      // 15 : Externer Start input
        uint32_t logostartext          : 1;      // 16 : Externer start input ( see basedosierung ), wird mit logo verwaltet
        uint32_t releasestart          : 1;      // 17 : final release start ( see basedosierung )
        uint32_t release               : 1;      // 18 : final release after release start, check further conditions ( see basedosierung )
        uint32_t volumetricext         : 1;      // 19 : Volumetric external input
        uint32_t tareActive            : 1;      // 20 : tare active
        uint32_t gravvol               : 1;      // 21 : gravimetric mode temporarily in volumetric mode
        uint32_t wbfminbeltloadreached : 1;      // 22 : minimale Bandlast erreicht
        uint32_t agitatorrun           : 1;      // 23 : Rührwerk läuft
        uint32_t emptyfeederActive     : 1;      // 24 : emptyfeeder task active
        uint32_t slaveMode             : 1;      // 25 : dosing unit in slave mode, 0 : master mode
        uint32_t batchStarted          : 1;      // 26 : only batch feeder ! batch started but interrupted if not finished
        uint32_t batchFinished         : 1;      // 27 : only batch feeder ! batch started
        uint32_t refillControlEnable   : 1;      // 28 : refill control enabled
        uint32_t LCEnable              : 1;      // 29 : LC-Feeder enabled
        uint32_t LCActive              : 1;      // 30 : LC-Feeder active
        uint32_t LclMinLevelActive     : 1;      // 31 : only Lcl-Feeder, min level reached
        uint32_t LclMaxLevelActive     : 1;      // 32 : only Lcl-Feeder, max level reached
        uint32_t IfsSwitchHigh         : 1;      // 33 : only Ifs-Feeder, high level reached
        uint32_t IfsSwitchHighEnable   : 1;      // 34 : only Ifs-Feeder, high level reached
        uint32_t IfsSwitchMax          : 1;      // 35 : only Ifs-Feeder, max level reached
        uint32_t IfsSwitchMaxEnable    : 1;      // 36 : only Ifs-Feeder, max level reached
        uint32_t IfsSwitchMin          : 1;      // 37 : only Ifs-Feeder, min level reached
        uint32_t IfsSwitchMinEnable    : 1;      // 38 : only Ifs-Feeder, min level reached
        uint32_t IfsSwitchLow          : 1;      // 39 : only Ifs-Feeder, low level reached
        uint32_t IfsSwitchLowEnable    : 1;      // 40 : only Ifs-Feeder, low level reached
        uint32_t IfsEmptyFeeder        : 1;      // 41 : only Ifs-Feeder, empty feeder reached
        uint32_t IfsOverflowFeeder     : 1;      // 42 : only Ifs-Feeder, overflow feeder
        uint32_t serviceMode           : 1;      // 43 : Service mode

    } flags;
    uint64_t allflags = 0;
};


union RefillStatus
{
    struct
    {
        uint32_t refillrelease              : 1;       // refill release - RefillReleaseState
        uint32_t refillactive               : 1;       // refill active - RefillState
        uint32_t refillstartextI            : 1;       // IO-input refill start
        uint32_t refillstopextI             : 1;       // IO-input refill stop
        uint32_t refillreleaseextI          : 1;       // IO-input refill release
        uint32_t refillalarmextI            : 1;       // IO-input refill alarm
        uint32_t refillstartextO            : 1;       // IO-output request
        uint32_t refillstartenable          : 1;       // RefillStartState
        uint32_t refillstopenable           : 1;       // RefillStopState
        uint32_t refilltimeout              : 1;       // timeout alarm
        uint32_t refillControlEnable        : 1;       // refill control enabled
        uint32_t refillWeightAboveMax       : 1;
        uint32_t refillWeightBelowMin       : 1;
        uint32_t refillFeederEmptyStarted   : 1;
        uint32_t refillFeederEmptyEnded     : 1;
    } flags;
    uint32_t allflags = 0;
};

union DeviceConfig
{
    struct
    {
        uint32_t agitatorEnable : 1;                // agitator enable
        uint32_t refillScrewSpeedEnable : 1;        // refill screw enable
        uint32_t refillControlEnable : 1;           // refill control enabled
        uint32_t moduleRefillEnable : 1;            // Module refill enabled
        uint32_t LCEnable : 1;                      // scale enabled
    } flags;
    uint32_t allflags = 0;
};

};


