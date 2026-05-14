//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LCModul.h
///
/// @file   LCModul.h
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

#include <string>
#include "BASE/include/SystemConfig.h"
#include "BASE/include/LCType.h"
#include "BASE/include/LCSubType.h"

namespace loadcell
{

class ILCModuleInterface
{
public:
    ILCModuleInterface() = default;
    virtual ~ILCModuleInterface() = 0 {};
    ILCModuleInterface(const ILCModuleInterface&) = delete;
    ILCModuleInterface& operator=(const ILCModuleInterface&) = delete;


    virtual void SetScaleRange(const int32_t i, const float32_t _fT) = 0;
    virtual float32_t GetScaleRange(const int32_t i) const  = 0;
    virtual void SetTaraWeight(const int32_t i, const float32_t _fT) = 0;
    virtual float32_t GetTaraWeight(const int32_t i) const = 0;
    virtual void SetStatus(const int32_t i, const base::LC_STATUS ) = 0;
    virtual base::LC_STATUS GetStatus(const int32_t i) const = 0;
    virtual void SetCorrectionFactor(const int32_t i, const float32_t _fC) = 0;
    virtual float32_t GetCorrectionFactor(const int32_t i) const = 0;
    virtual void SetPriority(const int32_t n, const base::LC_PRIORITY s) = 0;
    virtual base::LC_PRIORITY GetPriority(const int32_t n) const = 0;
    virtual BOOL SetNoiseControl(const int32_t i, const int16_t sN) = 0;
    virtual void GetErrorCounter(const int32_t i, int16_t*)  = 0;
    virtual BOOL IsActive(const int32_t i) const = 0;
    virtual BOOL IsInitialized(const int32_t i) const = 0;
    virtual BOOL IsConfigured(const int32_t i) const = 0;
    virtual BOOL IsEnabled(const int32_t i) const = 0;
    virtual BOOL IsStartInit(const int32_t i) const = 0;

    virtual base::LC_STATUS GetWeight(const int32_t i, float32_t* f, int16_t* sErrorCounter) const = 0;
    virtual base::LC_STATUS GetWeight(const int32_t n, float32_t* f, uint64_t* pT, int16_t* sErrorCounter) const = 0;
    virtual base::LC_STATUS GetRawWeight(const int32_t n, float32_t* f, uint64_t* pT, int16_t* sErrorCounter) const = 0;
    virtual base::LC_STATUS GetRawWeight(const int32_t i, float32_t* f) const = 0;

    virtual base::LC_ERRORCODE SetAddress(const int32_t i) = 0;
    virtual base::LC_ERRORCODE ClrAddress(const int32_t n) = 0;
    virtual base::LC_ERRORCODE PrepareWriteEEprom(const int32_t i) = 0;
    virtual base::LC_ERRORCODE EnableEEpromWriteProtection(const int32_t n, const BOOL b) = 0;
    virtual base::LC_ERRORCODE RegisterLC(const int32_t n) = 0;
    virtual void SetDetectionType(const int32_t n, base::eLcTypeDetection eTyp) = 0;
    virtual base::eLcTypeDetection GetDetectionType(int32_t n) const = 0;
    virtual void SetType(const int32_t n, base::eLcType eTyp) = 0;
    virtual base::eLcType GetType(int32_t n) const = 0;
    virtual void SetSubType(const int32_t n, base::eLcSubType eTyp) = 0;
    virtual base::eLcSubType GetSubType(int32_t n) const = 0;

    virtual int32_t  GetMaxItems(void) const = 0;

    virtual BOOL IsOpen(void) const = 0;
    virtual base::LC_ERRORCODE Open(const base::CSystemConfig& rCfg) = 0;
    virtual base::LC_ERRORCODE Close(void) = 0;

    virtual void Suspend(void) = 0;
    virtual void Resume(void) = 0;
    virtual void CreateStatus(std::string& status) const = 0;

#ifdef __SIM_WEIGHT_GUI
    virtual void  SetWeight(const int32_t index, const float32_t value) = 0;
#endif

};

};



