//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AlarmTypes
///
/// @file   AlarmTypes.h
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
#include <cassert>
#include "BASE/types.h"
#include "BASE/include/DoseType.h"

namespace base
{

enum class eAlarmError : uint32_t
{
    ERROR_DOSE_DRIVECOMMAND_LOW,		    // 0
    ERROR_DOSE_DRIVECOMMAND_HIGH,			// 1
    ERROR_DOSE_MASSFLOW_LOW,				// 2
    ERROR_DOSE_MASSFLOW_HIGH,				// 3
    ERROR_DOSE_DOSEPERFORMANCE_LOW,			// 4
    ERROR_DOSE_DOSEPERFORMANCE_HIGH,		// 5
    ERROR_WBF_BELTLOAD_LOW,					// 6
    ERROR_WBF_BELTLOAD_HIGH,				// 7
    ERROR_DOSE_ENCODER,					    // 8         Drehgeberfehler
    ERROR_DOSE_ENCODER_SPEED_LOW,		    // 9        Drehgeberfehler low
    ERROR_DOSE_ENCODER_SPEED_HIGH,		    // 10        Drehgeberfehler HIGH
    ERROR_DOSE_LC_INVALIDWEIGHT,			// 11
    ERROR_DOSE_LC_MINWEIGHT,                // 12
    ERROR_DOSE_LC_MAXWEIGHT,                // 13
    ERROR_DOSE_LC_OVERLOAD,			        // 14
    ERROR_DOSE_LC_GENERALERROR,				// 15
    ERROR_DOSE_LC_NOISEERROR,				// 16
    ERROR_DOSE_LC_TIMEOUT,					// 17
    ERROR_DOSE_LC_PROTOCOLERROR,		    // 18
    ERROR_DOSE_ALARMSTOP,					// 19
    ERROR_DOSE_EXTALARM,					// 20
    ERROR_DOSE_EXTREFILLALARM,				// 21
    ERROR_DOSE_REFILL_TIMEOUT,              // 22
    ERROR_LWF_REFILLALARMLIMIT,				// 23
    ERROR_ICP_BETRIEB,						// 24
    ERROR_EMPTYFEEDER_LOADCELL,		        // 25
    ERROR_EMPTYFEEDER_RELEASEFAILED,		// 26
    ERROR_EMPTYFEEDER_TIMEOUT,		        // 27
    ERROR_EMPTYFEEDER_GENERAL,		        // 28
    ERROR_CALIB_REFILLACTIVE,		        // 29
    ERROR_CALIB_RELEASEFAILED,		        // 30
    ERROR_CALIB_LOADCELL,                   // 31
    ERROR_CALIB_INVALIDSETUP,               // 32
    ERROR_TARE_RELEASEFAILED,               // 33
    ERROR_TARE_REFILLACTIVE,		        // 34
    ERROR_TARE_LOADCELL,                    // 35
    ERROR_TARE_GENERAL,                     // 36
    ERROR_DOSE_BATCH_TIMEOUT,		        // 37
    ERROR_IFS_INFEEDHOPPER_EMPTY,		    // 38
    ERROR_IFS_INFEEDHOPPER_OVERFLOW,		// 39
    ERROR_DOSE_DRIVEERROR,					// 40
    ERROR_DOSE_MOTORLOAD,					// 41
    ERROR_DOSE_DRIVEERRORPADDLE,			// 42
    ERROR_DOSE_DRIVEERRORREFILLFEEDER,		// 43
    ERROR_LINE_SYNCERROR,                   // 44
    ERROR_LINE_SHUTDOWN,					// 45
    ERROR_LINE_EXTALARM,					// 46
    ERROR_LINE_CUSTOMER_ALARM,			    // 47
    ERROR_LINE_UPS_ALARM,			        // 48
    ERROR_MAX,						        // 49
};
static const int32_t eAlarmErrorMax = _S32(eAlarmError::ERROR_MAX);


enum class eAlarmCategory : uint16_t
{
    categoryAM = 0, // Alarm message     0
    categoryFL,     // Flash light line  1
    categoryS,      // local stop        2
    categorySD,     // lineshutdown line     3
    categoryRA,     // Refill alarm line 3
    categoryMax,
};
static const int32_t eAlarmCategoryMax = _S32(eAlarmCategory::categoryMax);


enum class eAlarmClass : uint16_t 
{ 
    eNOTYP = 0, 
    eWARNTYP, 
    eALARMTYP,
    eALARMTYPMAX
};
static const int32_t eAlarmClassMax = _S32(eAlarmClass::eALARMTYPMAX);


enum class eAlarmLevel : uint16_t
{
    eNone = 0
    , eWarning
    , eAlarm
    , eTotal
};


#define DECLARE_ALARMCATEGORY(field) const std::array<base::eAlarmCategory, base::eAlarmCategoryMax> field = { \
    base::eAlarmCategory::categoryAM,\
    base::eAlarmCategory::categoryFL, \
    base::eAlarmCategory::categoryS,\
    base::eAlarmCategory::categorySD,\
    base::eAlarmCategory::categoryRA,\
};


#define DECLARE_ALARMERROR(field) const std::array<base::eAlarmError, base::eAlarmErrorMax> field = { \
    base::eAlarmError::ERROR_DOSE_DRIVECOMMAND_LOW, \
    base::eAlarmError::ERROR_DOSE_DRIVECOMMAND_HIGH, \
    base::eAlarmError::ERROR_DOSE_MASSFLOW_LOW, \
    base::eAlarmError::ERROR_DOSE_MASSFLOW_HIGH, \
    base::eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_LOW, \
    base::eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_HIGH, \
    base::eAlarmError::ERROR_WBF_BELTLOAD_LOW, \
    base::eAlarmError::ERROR_WBF_BELTLOAD_HIGH, \
    base::eAlarmError::ERROR_DOSE_ENCODER, \
    base::eAlarmError::ERROR_DOSE_ENCODER_SPEED_LOW, \
    base::eAlarmError::ERROR_DOSE_ENCODER_SPEED_HIGH, \
    base::eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT, \
    base::eAlarmError::ERROR_DOSE_LC_MINWEIGHT, \
    base::eAlarmError::ERROR_DOSE_LC_MAXWEIGHT, \
    base::eAlarmError::ERROR_DOSE_LC_OVERLOAD, \
    base::eAlarmError::ERROR_DOSE_LC_GENERALERROR, \
    base::eAlarmError::ERROR_DOSE_LC_NOISEERROR, \
    base::eAlarmError::ERROR_DOSE_LC_TIMEOUT, \
    base::eAlarmError::ERROR_DOSE_LC_PROTOCOLERROR, \
    base::eAlarmError::ERROR_DOSE_ALARMSTOP, \
    base::eAlarmError::ERROR_DOSE_EXTALARM, \
    base::eAlarmError::ERROR_DOSE_EXTREFILLALARM, \
    base::eAlarmError::ERROR_DOSE_REFILL_TIMEOUT, \
    base::eAlarmError::ERROR_LWF_REFILLALARMLIMIT, \
    base::eAlarmError::ERROR_ICP_BETRIEB, \
    base::eAlarmError::ERROR_EMPTYFEEDER_LOADCELL, \
    base::eAlarmError::ERROR_EMPTYFEEDER_RELEASEFAILED, \
    base::eAlarmError::ERROR_EMPTYFEEDER_TIMEOUT, \
    base::eAlarmError::ERROR_EMPTYFEEDER_GENERAL, \
    base::eAlarmError::ERROR_CALIB_REFILLACTIVE, \
    base::eAlarmError::ERROR_CALIB_RELEASEFAILED, \
    base::eAlarmError::ERROR_CALIB_LOADCELL, \
    base::eAlarmError::ERROR_CALIB_INVALIDSETUP, \
    base::eAlarmError::ERROR_TARE_RELEASEFAILED, \
    base::eAlarmError::ERROR_TARE_REFILLACTIVE, \
    base::eAlarmError::ERROR_TARE_LOADCELL, \
    base::eAlarmError::ERROR_TARE_GENERAL, \
    base::eAlarmError::ERROR_DOSE_BATCH_TIMEOUT, \
    base::eAlarmError::ERROR_IFS_INFEEDHOPPER_EMPTY, \
    base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW, \
    base::eAlarmError::ERROR_DOSE_DRIVEERROR, \
    base::eAlarmError::ERROR_DOSE_MOTORLOAD, \
    base::eAlarmError::ERROR_DOSE_DRIVEERRORPADDLE, \
    base::eAlarmError::ERROR_DOSE_DRIVEERRORREFILLFEEDER, \
    base::eAlarmError::ERROR_LINE_SYNCERROR, \
    base::eAlarmError::ERROR_LINE_SHUTDOWN, \
    base::eAlarmError::ERROR_LINE_EXTALARM,	\
    base::eAlarmError::ERROR_LINE_CUSTOMER_ALARM,	\
    base::eAlarmError::ERROR_LINE_UPS_ALARM,\
};



union eAlarmErrorBits
{
    struct 
    {
        uint32_t doseempty : 1;                         // dose empty               - ERROR_LWF_REFILLALARMLIMIT
        uint32_t refilltimeout : 1;                     // refill timeout           - ERROR_DOSE_REFILL_TIMEOUT
        uint32_t massflowhigh : 1;                      // massflow high            - ERROR_DOSE_MASSFLOW_HIGH
        uint32_t massflowlow : 1;                       // massflow low             - ERROR_DOSE_MASSFLOW_LOW
        uint32_t drivecommandhigh : 1;                  // drive command high       - ERROR_DOSE_DRIVECOMMAND_HIGH
        uint32_t drivecommandlow : 1;                   // drive command low        - ERROR_DOSE_DRIVECOMMAND_LOW
        uint32_t loadcellhigh : 1;                      // Überlast                 - ERROR_DOSE_LC_OVERLOAD
        uint32_t loadcellerror : 1;                     // load cell error          - ERROR_DOSE_LC_GENERALERROR
        uint32_t loadcellnoise : 1;                     // load cell moise          - ERROR_DOSE_LC_NOISEERROR
        uint32_t loadcelltimeout : 1;                   // load cell timeout        - ERROR_DOSE_LC_TIMEOUT
        uint32_t loadcellprotocolerror : 1;             // load cell protocol error - ERROR_DOSE_LC_PROTOCOLERROR
        uint32_t loadcellinvalidweight : 1;             // invalid weight           - ERROR_DOSE_LC_INVALIDWEIGHT
        uint32_t loadcellminweight : 1;                 // min weight reached       - ERROR_DOSE_LC_MINWEIGHT
        uint32_t loadcellmaxweight : 1;                 // max weight reached       - ERROR_DOSE_LC_MAXWEIGHT
        uint32_t doseperformancehigh : 1;               // Doseperformance high     - ERROR_DOSE_DOSEPERFORMANCE_HIGH
        uint32_t doseperformancelow : 1;                // Doseperformance low      - ERROR_DOSE_DOSEPERFORMANCE_LOW
        uint32_t encodererror : 1;                      // encoder error            - ERROR_DOSE_ENCODER
        uint32_t encoderspeedlow : 1;                   // encoder speed low        - ERROR_DOSE_ENCODER_SPEED_LOW
        uint32_t encoderspeedhigh : 1;                  // encoder speed high       - ERROR_DOSE_ENCODER_SPEED_HIGH
        uint32_t extalarm : 1;                          // external alarm           - ERROR_DOSE_EXTALARM
        uint32_t extrefillalarm : 1;                    // external refill alarm    - ERROR_DOSE_EXTREFILLALARM
        uint32_t alarmstop : 1;                         // alarm stop               - ERROR_DOSE_ALARMSTOP
        uint32_t icperror : 1;                          // icpfehler                - ERROR_ICP_BETRIEB
        uint32_t belloadmin : 1;                        // min belt load            - ERROR_WBF_BELTLOAD_LOW
        uint32_t belloadmax : 1;                        // max belt load            - ERROR_WBF_BELTLOAD_HIGH
        uint32_t emptyfeederLoadcell : 1;               // empty feeder loadcell    - ERROR_EMPTYFEEDER_LOADCELL
        uint32_t emptyfeederReleaseFailed : 1;          // empty feeder release     - ERROR_EMPTYFEEDER_RELEASEFAILED
        uint32_t emptyfeederTimeout : 1;                // empty feeder timeout     - ERROR_EMPTYFEEDER_TIMEOUT
        uint32_t emptyfeederGeneralError : 1;           // empty feeder general     - ERROR_EMPTYFEEDER_GENERAL
        uint32_t calibRefillActive : 1;                 // calib. refill active     - ERROR_CALIB_REFILLACTIVE
        uint32_t calibReleaseFailed : 1;                // calib. release failed    - ERROR_CALIB_RELEASEFAILED
        uint32_t calibLoadCell : 1;                     // calib loadcell error     - ERROR_CALIB_LOADCELL
        uint32_t calibInvalidSetup : 1;                 // calib invalid setup      - ERROR_CALIB_GENERAL
        uint32_t tareReleaseFailed : 1;                 // tare release failed      - ERROR_TARE_RELEASEFAILED
        uint32_t tareRefillActive : 1;                  // tare refill active       - ERROR_TARE_REFILLACTIVE
        uint32_t tareLoadCell : 1;                      // tare loadcell error      - ERROR_TARE_LOADCELL
        uint32_t tareGeneralError : 1;                  // tare general error       - ERROR_TARE_GENERAL
        uint32_t batchTimeout : 1;                      // batch timout             - ERROR_DOSE_BATCH_TIMEOUT
        uint32_t ifsInfeedHopperEmpty : 1;              // Infeed hopper empty      - ERROR_IFS_INFEEDHOPPER_EMPTY
        uint32_t ifsInfeedHopperOverflow : 1;           // Infeed hopper overflow   - ERROR_IFS_INFEEDHOPPER_OVERFLOW
        uint32_t driveerroralarm : 1;                   // Drive error alarm        - ERROR_DOSE_DRIVEERROR
        uint32_t motorloadalarm : 1;                    // Motor load alarm         - ERROR_DOSE_MOTORLOAD
        uint32_t driveerrorpaddlealarm : 1;             // Drive error paddle alarm - ERROR_DOSE_DRIVEERRORPADDLE
        uint32_t driveerrorrefillfeederalarm : 1;       // Drive error paddle alarm - ERROR_DOSE_DRIVEERRORREFILLFEEDER
        uint32_t linesyncerror : 1;                     // sync error               - ERROR_LINE_SYNCERROR
        uint32_t lineshutdown : 1;                      // lineshutdown stop        - ERROR_LINE_SHUTDOWN
        uint32_t lineextalarm : 1;                      // ext. Alarm Linie         - ERROR_LINE_EXT_ALARM
        uint32_t linecustomeralarm : 1;                 // Kundenalarm Linie        - ERROR_LINE_CUSTOMER_ALARM
        uint32_t lineupsalarm : 1;                      // UPS alarm Linie          - ERROR_LINE_UPS_ALARM
    } flags;
    uint64_t allflags = 0;

    eAlarmErrorBits(const eDoseType type);
    eAlarmErrorBits(uint64_t flags = 0) : allflags{ flags }
    {}

    void SetAll()
    {
        allflags = 0;
        uint32_t count = eAlarmErrorMax;
        while (count--)
        {
            allflags <<= 1U;
            allflags |= 1U;
        }
    }

    static eAlarmErrorBits GetLineAlarmBits ()
    {
        eAlarmErrorBits bits;

        bits.flags.linesyncerror = true;
        bits.flags.lineshutdown = true;
        bits.flags.lineextalarm = true;
        bits.flags.linecustomeralarm = true;
        bits.flags.lineupsalarm = true;
        return bits;
    }

    void SetAlarmErrorBits(const eAlarmError alarm, const BOOL value = TRUE);
    BOOL IsAlarmErrorBits(const eAlarmError alarm) const;
    BOOL IsAlarm() const
    {   return allflags != 0;    }

    BOOL operator == (const eAlarmErrorBits& v) const
    {  return allflags == v.allflags;     }

    BOOL operator != (const eAlarmErrorBits& v) const
    {   return allflags != v.allflags;    }

    eAlarmErrorBits operator & (const eAlarmErrorBits& v) const
    {  return (allflags & v.allflags);    }

    void operator &= (const eAlarmErrorBits& v)
    {  allflags &= v.allflags;    }

    eAlarmErrorBits operator | (const eAlarmErrorBits& v) const
    {   return ( allflags | v.allflags);    }

    void operator |= (const eAlarmErrorBits& v)
    {   allflags |= v.allflags;    }

    eAlarmErrorBits operator ~ () const
    {return (~allflags);    }

};
//**************************************************************************************************************************************
//**************************************************************************************************************************************
inline eAlarmErrorBits::eAlarmErrorBits(const eDoseType type) : allflags{ 0 }
{
    flags.batchTimeout = __ISBATCHERTYPE(type);

    auto setlinebits = [this]() {
        flags.linesyncerror = true;
        flags.lineshutdown = true;
        flags.lineextalarm = true;
        flags.linecustomeralarm = true;
        flags.lineupsalarm = true;
    };

    setlinebits();

    switch (type)
    {
    case eDoseType::DOSETYPE_LCL:
        flags.loadcellhigh = true;              // Überlast
        flags.loadcellerror = true;             // load cell error
        flags.loadcellmaxweight = true;         // invalid weight
        flags.loadcellminweight = true;         // invalid weight
        flags.loadcellinvalidweight = true;     // invalid weight
        flags.loadcellprotocolerror = true;     // protocol error
        flags.loadcelltimeout = true;           // Timeout loadcell
        flags.extalarm = true;                  // external alarm
        flags.alarmstop = true;                 // alarm stop
        flags.linesyncerror = true;             // sync error
        flags.tareReleaseFailed = true;         // tare release failed
        flags.tareLoadCell = true;              // tare loadcell error
        flags.tareGeneralError = true;          // tare general error
        break;

    case eDoseType::DOSETYPE_IFSO:
    case eDoseType::DOSETYPE_IFSU:
        flags.linesyncerror = true;
        flags.ifsInfeedHopperEmpty = true;      // invalid weight
        flags.ifsInfeedHopperOverflow = true;   // invalid weight
        flags.extalarm = true;                  // external alarm
        flags.alarmstop = true;                 // alarm stop
        break;

    case eDoseType::DOSETYPE_IFL:
        flags.loadcellhigh = true;              // Überlast
        flags.loadcellerror = true;             // load cell error
        flags.loadcellmaxweight = true;     // invalid weight
        flags.loadcellminweight = true;     // invalid weight
        flags.loadcellinvalidweight = true;     // invalid weight
        flags.loadcellprotocolerror = true;         // protocol error
        flags.loadcelltimeout = true;               // Timeout loadcell
        flags.extalarm = true;                  // external alarm
        flags.alarmstop = true;                 // alarm stop
        flags.linesyncerror = true;              // sync error
        flags.tareReleaseFailed = true;         // tare release failed
        flags.tareLoadCell = true;              // tare loadcell error
        flags.tareGeneralError = true;          // tare general error
        break;

    case eDoseType::DOSETYPE_LWS:
    case eDoseType::DOSETYPE_LWP:
    case eDoseType::DOSETYPE_LWV:
    case eDoseType::DOSETYPE_LWB:
    case eDoseType::DOSETYPE_GWB:
        flags.doseempty = true;                     // dose empty
        flags.refilltimeout = true;                 // refill timeout
        flags.massflowhigh = true;                  // massflow high
        flags.massflowlow = true;                   // massflow low
        flags.drivecommandhigh = true;              // drive command high
        flags.drivecommandlow = true;               // drive command low
        flags.loadcellhigh = true;                  // Überlast
        flags.loadcellerror = true;                 // load cell error
        flags.loadcellinvalidweight = true;         // invalid weight
        flags.loadcellnoise = true;                 // load cell moise
        flags.loadcellprotocolerror = true;         // protocol error
        flags.loadcelltimeout = true;               // Timeout loadcell
        flags.doseperformancehigh = true;           // Doseperformance high
        flags.doseperformancelow = true;            // Doseperformance low
        flags.encodererror = true;                  // encoder error
        flags.encoderspeedlow = true;               // encoder speed low
        flags.encoderspeedhigh = true;              // encoder speed high
        flags.extalarm = true;                      // external alarm
        flags.extrefillalarm = true;                // external refill alarm
        flags.alarmstop = true;                     // alarm stop
        flags.emptyfeederLoadcell = true;           // empty feeder loadcell error
        flags.emptyfeederReleaseFailed = true;      // empty feeder release error
        flags.emptyfeederTimeout = true;            // empty feeder timeout
        flags.emptyfeederGeneralError = true;       // empty feeder general error
        flags.calibRefillActive = true;             // calibration refill active
        flags.calibReleaseFailed = true;            // calibration release failed
        flags.calibLoadCell = true;                 // calib loadcell error
        flags.calibInvalidSetup = true;             // calib invalid setup
        flags.tareReleaseFailed = true;             // tare release failed
        flags.tareRefillActive = true;              // tare refill active
        flags.tareLoadCell = true;                  // tare loadcell error
        flags.tareGeneralError = true;              // tare general error
        flags.driveerroralarm = true;               // Drive error alarm
        flags.motorloadalarm = true;                // Motor load alarm
        flags.driveerrorpaddlealarm = true;         // Drive error paddle
        flags.driveerrorrefillfeederalarm = true;   // Drive error refill feeder
        break;


    case eDoseType::DOSETYPE_WBM:
    case eDoseType::DOSETYPE_WBF:
        flags.drivecommandhigh = true;          // drive command high
        flags.drivecommandlow = true;           // drive command low
        flags.massflowhigh = true;              // massflow high
        flags.massflowlow = true;               // massflow low
        flags.encodererror = true;              // encoder error
        flags.encoderspeedlow = true;           // encoder speed low
        flags.encoderspeedhigh = true;          // encoder speed high
        flags.loadcellinvalidweight = true;     // illegal weight
        flags.belloadmin = true;                // min belt load
        flags.belloadmax = true;                // max belt load
        flags.loadcellhigh = true;              // Überlast
        flags.loadcellerror = true;
        flags.loadcellnoise = true;             // load cell moise
        flags.loadcellprotocolerror = true;         // protocol error
        flags.loadcelltimeout = true;               // Timeout loadcell
        flags.alarmstop = true;                 // alarm stop
        flags.extalarm = true;
        flags.driveerroralarm = true;           // Drive error alarm
        flags.motorloadalarm = true;            // Motor load alarm
        flags.driveerrorrefillfeederalarm = true;   // Drive error refill feeder
        break;

    case eDoseType::DOSETYPE_ICB:
    case eDoseType::DOSETYPE_ICP:
        flags.drivecommandhigh = true;          // drive command high
        flags.drivecommandlow = true;           // drive command low
        flags.massflowhigh = true;              // massflow high
        flags.massflowlow = true;               // massflow low
        flags.alarmstop = true;                 // alarm stop
        flags.extalarm = true;
        flags.icperror = true;
        flags.driveerroralarm = true;           // Drive error alarm
        flags.motorloadalarm = true;            // Motor load alarm
        flags.driveerrorrefillfeederalarm = true;   // Drive error refill feeder
        break;

    case eDoseType::DOSETYPE_VOL:
        flags.drivecommandhigh = true;          // drive command high
        flags.drivecommandlow = true;           // drive command low
        flags.alarmstop = true;                 // alarm stop
        flags.extalarm = true;
        flags.encodererror = true;              // encoder error
        flags.encoderspeedlow = true;           // encoder speed low
        flags.encoderspeedhigh = true;          // encoder speed high
        // Wenn refill aktiviert wurde
        flags.extrefillalarm = true;            // external refill alarm
        flags.refilltimeout = true;             // refill timeout
        flags.doseempty = true;                 // dose empty
        flags.driveerroralarm = true;           // Drive error alarm
        flags.motorloadalarm = true;            // Motor load alarm
        flags.driveerrorpaddlealarm = true;     // Drive error paddle
        flags.driveerrorrefillfeederalarm = true;   // Drive error refill feeder
        break;

    case eDoseType::DOSETYPE_PID:
        flags.doseempty = true;                 // dose empty
        flags.refilltimeout = true;             // refill timeout
        flags.massflowhigh = true;              // massflow high
        flags.massflowlow = true;               // massflow low
        flags.drivecommandhigh = true;          // drive command high
        flags.drivecommandlow = true;           // drive command low
        flags.encodererror = true;
        flags.alarmstop = true;                 // alarm stop
        flags.extalarm = true;
        break;

    default:
        break;
    }
}
//**************************************************************************************************************************************
//**************************************************************************************************************************************
inline void eAlarmErrorBits::SetAlarmErrorBits(const eAlarmError alarm, const BOOL value)
{
    switch (alarm)
    {
        case eAlarmError::ERROR_DOSE_REFILL_TIMEOUT: flags.refilltimeout = value;
            break;
        case eAlarmError::ERROR_DOSE_DRIVECOMMAND_LOW: flags.drivecommandlow = value;
            break;
        case eAlarmError::ERROR_DOSE_DRIVECOMMAND_HIGH: flags.drivecommandhigh = value;
            break;
        case eAlarmError::ERROR_DOSE_MASSFLOW_LOW: flags.massflowlow = value;
            break;
        case eAlarmError::ERROR_DOSE_MASSFLOW_HIGH: flags.massflowhigh = value;
            break;
        case eAlarmError::ERROR_DOSE_ENCODER: flags.encodererror = value;
            break;
        case eAlarmError::ERROR_DOSE_ENCODER_SPEED_LOW: flags.encoderspeedlow = value;
            break;
        case eAlarmError::ERROR_DOSE_ENCODER_SPEED_HIGH: flags.encoderspeedhigh = value;
            break;
        case eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT: flags.loadcellinvalidweight = value;
            break;
        case eAlarmError::ERROR_DOSE_LC_MINWEIGHT: flags.loadcellminweight = value;
             break;
        case eAlarmError::ERROR_DOSE_LC_MAXWEIGHT: flags.loadcellmaxweight = value;
            break;
        case eAlarmError::ERROR_DOSE_LC_OVERLOAD:flags.loadcellhigh = value;
            break;
        case eAlarmError::ERROR_DOSE_LC_GENERALERROR:flags.loadcellerror = value;
            break;
        case eAlarmError::ERROR_DOSE_LC_NOISEERROR:flags.loadcellnoise = value;
            break;
        case eAlarmError::ERROR_DOSE_LC_TIMEOUT:flags.loadcelltimeout = value;
            break;
        case eAlarmError::ERROR_DOSE_LC_PROTOCOLERROR:flags.loadcellprotocolerror = value;
            break;
        case eAlarmError::ERROR_DOSE_ALARMSTOP:flags.alarmstop = value;
            break;
        case eAlarmError::ERROR_DOSE_EXTALARM:flags.extalarm = value;
            break;
        case eAlarmError::ERROR_DOSE_EXTREFILLALARM:flags.extrefillalarm = value;
            break;
        case eAlarmError::ERROR_LWF_REFILLALARMLIMIT:flags.doseempty = value;
            break;
        case eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_LOW:flags.doseperformancelow = value;
            break;
        case eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_HIGH:flags.doseperformancehigh = value;
            break;
        case eAlarmError::ERROR_WBF_BELTLOAD_LOW:flags.belloadmin = value;
            break;
        case eAlarmError::ERROR_WBF_BELTLOAD_HIGH:flags.belloadmax = value;
            break;
        case eAlarmError::ERROR_ICP_BETRIEB:flags.icperror = value;
            break;
        case eAlarmError::ERROR_EMPTYFEEDER_LOADCELL:flags.emptyfeederLoadcell = value;
            break;
        case eAlarmError::ERROR_EMPTYFEEDER_RELEASEFAILED:flags.emptyfeederReleaseFailed = value;
            break;
        case eAlarmError::ERROR_EMPTYFEEDER_TIMEOUT:flags.emptyfeederTimeout = value;
            break;
        case eAlarmError::ERROR_EMPTYFEEDER_GENERAL : flags.emptyfeederGeneralError = value;
            break;
        case eAlarmError::ERROR_CALIB_REFILLACTIVE: flags.calibRefillActive = value;
            break;
        case eAlarmError::ERROR_CALIB_RELEASEFAILED: flags.calibReleaseFailed = value;
            break;
        case eAlarmError::ERROR_CALIB_LOADCELL: flags.calibLoadCell = value;
            break;
        case eAlarmError::ERROR_CALIB_INVALIDSETUP: flags.calibInvalidSetup = value;
            break;
        case eAlarmError::ERROR_TARE_RELEASEFAILED: flags.tareReleaseFailed = value;
            break;
        case eAlarmError::ERROR_TARE_REFILLACTIVE: flags.tareRefillActive = value;
            break;
        case eAlarmError::ERROR_TARE_LOADCELL: flags.tareLoadCell = value;
            break;
        case eAlarmError::ERROR_TARE_GENERAL: flags.tareGeneralError = value;
            break;
        case eAlarmError::ERROR_DOSE_BATCH_TIMEOUT: flags.batchTimeout = value;
            break;
        case eAlarmError::ERROR_IFS_INFEEDHOPPER_EMPTY: flags.ifsInfeedHopperEmpty = value;
            break;
        case eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW: flags.ifsInfeedHopperOverflow = value;
            break;
        case eAlarmError::ERROR_DOSE_DRIVEERROR: flags.driveerroralarm = value;
            break;
        case eAlarmError::ERROR_DOSE_MOTORLOAD: flags.motorloadalarm = value;
            break;
        case eAlarmError::ERROR_DOSE_DRIVEERRORPADDLE: flags.driveerrorpaddlealarm = value;
            break;
        case eAlarmError::ERROR_DOSE_DRIVEERRORREFILLFEEDER: flags.driveerrorrefillfeederalarm = value;
            break;
        case eAlarmError::ERROR_LINE_SYNCERROR: flags.linesyncerror = value;
            break;
        case eAlarmError::ERROR_LINE_SHUTDOWN:flags.lineshutdown = value;
            break;
        case eAlarmError::ERROR_LINE_EXTALARM: flags.lineextalarm = value;
            break;
        case eAlarmError::ERROR_LINE_CUSTOMER_ALARM: flags.linecustomeralarm = value;
            break;
        case eAlarmError::ERROR_LINE_UPS_ALARM: flags.lineupsalarm = value;
            break;

        default:
            assert(FALSE);
            break;
    }
}
//**************************************************************************************************************************************
//**************************************************************************************************************************************
inline BOOL eAlarmErrorBits::IsAlarmErrorBits(const eAlarmError alarm) const
{
    switch (alarm)
    {
        case eAlarmError::ERROR_DOSE_REFILL_TIMEOUT: return flags.refilltimeout;
        case eAlarmError::ERROR_DOSE_DRIVECOMMAND_LOW: return flags.drivecommandlow;
        case eAlarmError::ERROR_DOSE_DRIVECOMMAND_HIGH: return flags.drivecommandhigh;
        case eAlarmError::ERROR_DOSE_MASSFLOW_LOW: return flags.massflowlow;
        case eAlarmError::ERROR_DOSE_MASSFLOW_HIGH: return flags.massflowhigh;
        case eAlarmError::ERROR_DOSE_ENCODER: return flags.encodererror;
        case eAlarmError::ERROR_DOSE_ENCODER_SPEED_LOW: return flags.encoderspeedlow;
        case eAlarmError::ERROR_DOSE_ENCODER_SPEED_HIGH: return flags.encoderspeedhigh;
        case eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT: return flags.loadcellinvalidweight;
        case eAlarmError::ERROR_DOSE_LC_OVERLOAD: return flags.loadcellhigh;
        case eAlarmError::ERROR_DOSE_LC_MINWEIGHT: return flags.loadcellminweight;
        case eAlarmError::ERROR_DOSE_LC_MAXWEIGHT: return flags.loadcellmaxweight;
        case eAlarmError::ERROR_DOSE_LC_GENERALERROR:return flags.loadcellerror;
        case eAlarmError::ERROR_DOSE_LC_NOISEERROR:return flags.loadcellnoise;
        case eAlarmError::ERROR_DOSE_LC_TIMEOUT: return flags.loadcelltimeout;
        case eAlarmError::ERROR_DOSE_LC_PROTOCOLERROR:return flags.loadcellprotocolerror;
        case eAlarmError::ERROR_DOSE_ALARMSTOP:return flags.alarmstop;
        case eAlarmError::ERROR_DOSE_EXTALARM:return flags.extalarm;
        case eAlarmError::ERROR_DOSE_EXTREFILLALARM:return flags.extrefillalarm;
        case eAlarmError::ERROR_LWF_REFILLALARMLIMIT: return flags.doseempty;
        case eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_LOW:return flags.doseperformancelow;
        case eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_HIGH:return flags.doseperformancehigh;
        case eAlarmError::ERROR_WBF_BELTLOAD_LOW:return flags.belloadmin;
        case eAlarmError::ERROR_WBF_BELTLOAD_HIGH:return flags.belloadmax;
        case eAlarmError::ERROR_ICP_BETRIEB:return flags.icperror;
        case eAlarmError::ERROR_EMPTYFEEDER_LOADCELL:return flags.emptyfeederLoadcell;
        case eAlarmError::ERROR_EMPTYFEEDER_RELEASEFAILED:return flags.emptyfeederReleaseFailed;
        case eAlarmError::ERROR_EMPTYFEEDER_TIMEOUT:return flags.emptyfeederTimeout;
        case eAlarmError::ERROR_EMPTYFEEDER_GENERAL:return flags.emptyfeederGeneralError;
        case eAlarmError::ERROR_CALIB_REFILLACTIVE: return flags.calibRefillActive;
        case eAlarmError::ERROR_CALIB_RELEASEFAILED: return flags.calibReleaseFailed;
        case eAlarmError::ERROR_CALIB_LOADCELL: return flags.calibLoadCell;
        case eAlarmError::ERROR_CALIB_INVALIDSETUP: return flags.calibInvalidSetup;
        case eAlarmError::ERROR_TARE_RELEASEFAILED: return flags.tareReleaseFailed;
        case eAlarmError::ERROR_TARE_REFILLACTIVE: return flags.tareRefillActive;
        case eAlarmError::ERROR_TARE_LOADCELL: return flags.tareLoadCell;
        case eAlarmError::ERROR_TARE_GENERAL: return flags.tareGeneralError;
        case eAlarmError::ERROR_DOSE_BATCH_TIMEOUT: return flags.batchTimeout;
        case eAlarmError::ERROR_IFS_INFEEDHOPPER_EMPTY: return flags.ifsInfeedHopperEmpty;
        case eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW: return flags.ifsInfeedHopperOverflow;
        case eAlarmError::ERROR_DOSE_DRIVEERROR: return flags.driveerroralarm;
        case eAlarmError::ERROR_DOSE_MOTORLOAD: return flags.motorloadalarm;
        case eAlarmError::ERROR_DOSE_DRIVEERRORPADDLE: return flags.driveerrorpaddlealarm;
        case eAlarmError::ERROR_DOSE_DRIVEERRORREFILLFEEDER: return flags.driveerrorrefillfeederalarm;
        case eAlarmError::ERROR_LINE_SYNCERROR: return flags.linesyncerror;
        case eAlarmError::ERROR_LINE_SHUTDOWN:return flags.lineshutdown;
        case eAlarmError::ERROR_LINE_EXTALARM: return flags.lineextalarm;
        case eAlarmError::ERROR_LINE_CUSTOMER_ALARM: return flags.linecustomeralarm;
        case eAlarmError::ERROR_LINE_UPS_ALARM: return flags.lineupsalarm;

        default:
            assert(FALSE);
            break;
    }
    return FALSE;
}
//**************************************************************************************************************************************
//**************************************************************************************************************************************
struct CAlarmCategories
{
    eAlarmErrorBits ulAlarm[eAlarmCategoryMax];

    void Set(const eAlarmError _alarm, const eAlarmCategory _category, const BOOL value)
    {   ulAlarm[_S32(_category)].SetAlarmErrorBits(_alarm, value);   }

    BOOL Get(const eAlarmError _alarm, const eAlarmCategory _category) const
    {   return ulAlarm[_S32(_category)].IsAlarmErrorBits(_alarm);    }

    BOOL IsAnyAlarm(const eAlarmCategory _category) const
    {   return ulAlarm[_S32(_category)].IsAlarm();   }

    BOOL IsAnyAlarm(const eAlarmError _alarm) const
    {
        BOOL result = FALSE;
        for (int32_t i = 0; i < eAlarmCategoryMax; i++)
        {
            result |= ulAlarm[i].IsAlarmErrorBits(_alarm);
        }
        return result;
    }

    BOOL Delete()
    {
        BOOL bChanged = FALSE;
        for (int32_t i = 0; i < eAlarmCategoryMax; i++)
        {
            bChanged |= (ulAlarm[i].allflags != 0);
            ulAlarm[i].allflags = 0;
        }
        return bChanged;
    }

    void Reset()
    {
        for (int32_t i = 0; i < eAlarmCategoryMax; i++)
        {
            ulAlarm[i].allflags = 0;
        }
    }

    void SetAll()
    {
        for (int32_t i = 0; i < eAlarmCategoryMax; i++)
        {
            ulAlarm[i].SetAll();
        }
    }
    CAlarmCategories() = default;
};
//**************************************************************************************************************************************
//**************************************************************************************************************************************
class CAlarmField : public CAlarmCategories
{
private:
    BOOL		m_bModified;

public:
    CAlarmField() : CAlarmCategories(), m_bModified(FALSE)
    {}

    ~CAlarmField() = default;
    CAlarmField(const CAlarmField& rAlarm) : CAlarmCategories(rAlarm)
    {  m_bModified = rAlarm.m_bModified;   }

    CAlarmField& operator = (const CAlarmField& rAlarm)
    {
        if (&rAlarm != this)
        {
            CAlarmCategories::operator=(rAlarm);
            m_bModified = rAlarm.m_bModified;
        }
        return *this;
    }

    void SetModified(const BOOL _Modified)
    {  m_bModified = _Modified;   }

    BOOL IsModified(void) const
    {  return m_bModified;    }
};


}; // // namespace base




