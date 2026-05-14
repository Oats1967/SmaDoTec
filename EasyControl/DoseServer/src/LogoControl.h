//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LogoControl
///
/// @file   LogoControl.h
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

#include <cmath>
#include <string>
#include <array>
#include <map>
#include <utility>
#include "BASE/include/FeedingTypeMap.h"
#include "BASE/include/Statemachine.h"
#include "BASE/include/LogoConfig.h"
#include "BASE/utils/public/FloatRingBuffer.h"
#include "BASE/Base.def"
#include "DoseDataLib/include/DoseData.h"

struct SLogoDose
{
    uint32_t    m_timeStamp = 0;
    uint32_t    m_pulseCounter = 0;
    uint32_t    m_SampleTime = 0;
    BOOL        m_running = FALSE;
    uint32_t    m_changecounter = 0;

    SLogoDose() : m_timeStamp{ 0 }
        , m_pulseCounter{ 0 }
        , m_SampleTime{ 0 }
        , m_running{ FALSE }
        , m_changecounter{ 0 }
    {}

    void Init()
    {
        m_timeStamp = 0;
        m_pulseCounter = 0;
        m_SampleTime = 0;
        m_running = FALSE;
        m_changecounter = 0;
    }
};

class CLogoControl : public base::CTimerStatemachine<>
{
    using CBaseClass = base::CTimerStatemachine<>;

    const base::CFeedingTypeMap c_FeedingTypeMap;
    const std::string c_MF;
    const std::string c_SF1;
    const std::string c_SF2;
    const std::string c_OA; // operate automatic mode
    const std::string c_OM; // operate manual mode
    const std::string c_SM; // ServiceMode set ( active result of string)
    const std::string c_SU; // ServiceMode user set

    base::CLogoConfig                         m_config;
    std::array<SLogoDose, GLOBALDOSEMAXCOUNT> m_LogoDose;
    std::map<const std::string, BOOL> m_refmap;
    base::utils::CFloatRingBuffer<2> m_RingBuffer;

private:
    void RunControlAnalogInput(const int32_t index);
    void SetSampleTime(const int32_t index);
    void RunControlPulseCounter(const int32_t index);
    void RunControlDefault(const int32_t index);
    void RunControl();

    void LockingInit();
    void Locking();
    void ServiceMode();

    CLogoControl(const CLogoControl&) = delete;
    CLogoControl& operator = (const CLogoControl&) = delete;
    CLogoControl() = delete;

public:
    CLogoControl(const base::CLogoConfig& rLogo);
    ~CLogoControl() override = default;

     BOOL InitExecute() override;
     BOOL ExitExecute() override;
     BOOL Execute() override;
};


