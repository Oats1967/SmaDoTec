//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RefillTypes
///
/// @file   RefillTypes.h
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

union eRefillBits
{
    struct
    {
        uint32_t IORefillSWRelease      : 1;         // Switch - release
        uint32_t IORefillSWStartStop    : 1;         // Switch - start stop
        uint32_t IORefillSWStart        : 1;         // Switch - start
        uint32_t IORefillSWStop         : 1;         // Switch - Stop

        uint32_t IORefillReleaseStart   : 1;         // Trigger Release Start
        uint32_t IORefillReleaseStop    : 1;         // Trigger Release Stop
        uint32_t IORefillStart          : 1;         // Trigger Start
        uint32_t IORefillStop           : 1;         // Trigger Stop

        uint32_t PBRefillCfg            : 1;         // PB-button start / stop 
        uint32_t PBRefillStart          : 1;         // PB-button start
        uint32_t PBRefillStop           : 1;         // PB-button stop

        uint32_t SWRefillCfg            : 1;         // SW-button start / stop 
        uint32_t SWRefillReleaseStart   : 1;         // SW-start
        uint32_t SWRefillReleaseStop    : 1;         // SW-stop

        uint32_t RefillState            : 1;         // Refill state
        uint32_t ControlEnable          : 1;        
        uint32_t ModuleEnable           : 1;        // Module refill enabled
    } flags;
    uint32_t allflags = 0;
};







