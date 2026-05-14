//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LogoConfig
///
/// @file   LogoConfig.h
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

#include <array>
#include <vector>
#include <utility>
#include <string>
#include "BASE/Base.h"
#include "BASE/include/FeedingType.h"
#include "BASE/include/LogoLocking.h"
#include "BASE/include/LogoInputSignal.h"
#include "BASE/include/SimpleParser.h"


namespace base
{
    //------------------------------------------------------------------------------------------------
    struct RunControlSection
    {
        uint32_t m_StartDriveCommand = 0;
        uint32_t m_EndDriveCommand = 0;
        uint32_t m_SampleTime = 0;
    };

    //------------------------------------------------------------------------------------------------
    class CRunControl
    {
        std::vector<RunControlSection> m_Interval;
        eLogoInputSignal              m_InputSignal;
        uint32_t                      m_SampleTime;
        BOOL                          m_Enable;

    public:

        CRunControl () :
              m_Interval{}
            , m_InputSignal{ eLogoInputSignal::eInputSignalNone }
            , m_SampleTime{ 2U }
            , m_Enable{ FALSE }
        {}

        CRunControl(CRunControl&& cfg) noexcept :
            m_Interval{ std::move(cfg.m_Interval) }
            , m_InputSignal{ std::move(cfg.m_InputSignal) }
            , m_SampleTime{ std::move(cfg.m_SampleTime) }
            , m_Enable{ std::move(cfg.m_Enable) }
        {}

        CRunControl(const CRunControl&) = default;
        CRunControl& operator = (const CRunControl& cfg) = default;

        SETGET(eLogoInputSignal, InputSignal)
        SETGET(uint32_t, SampleTime)
        SETGET(BOOL, Enable)

        void Add(const RunControlSection& sector);
        uint32_t GetCount() const;

        const RunControlSection& Get(const int32_t index) const;

        uint32_t GetSampleTime(const uint32_t rTime) const;

        BOOL operator == (const CRunControl& cfg) const;

        BOOL operator != (const CRunControl& cfg) const;
    };

    //------------------------------------------------------------------------------------------------
    class CLogoDoseConfig
    {
        CRunControl         m_RunControl;
        CSimpleParser       m_SimpleParser;
        eLogoLocking        m_Locking;

    public:
        CLogoDoseConfig() : m_RunControl{ } 
            , m_SimpleParser{ "DI0" }
            , m_Locking { eLogoLocking::eLockingNone }
        {}

        CLogoDoseConfig(const CLogoDoseConfig&) = default;

        CLogoDoseConfig& operator = (const CLogoDoseConfig& cfg) = default;

        CLogoDoseConfig(CLogoDoseConfig&& cfg) noexcept : 
               m_RunControl{ std::move(cfg.m_RunControl) }
             , m_SimpleParser{ std::move(cfg.m_SimpleParser) }
             , m_Locking{ std::move(cfg.m_Locking) }
        {}

        CRunControl& GetRunControl()
        {  return m_RunControl;     }

        CSimpleParser& GetSimpleParser()
        {   return m_SimpleParser;    }

        SETGET(const CRunControl&, RunControl)
        SETGET(const CSimpleParser&, SimpleParser)
        SETGET(const eLogoLocking, Locking)

        BOOL operator == (const CLogoDoseConfig& cfg) const;
        BOOL operator != (const CLogoDoseConfig& cfg) const
        {  return ! operator == (cfg);   }
    };
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
    inline BOOL CLogoDoseConfig::operator == (const CLogoDoseConfig& cfg) const
    {
        return  /*(m_Priority == cfg.m_Priority) && */
                (m_RunControl == cfg.m_RunControl) &&
                (m_SimpleParser == cfg.m_SimpleParser) &&
                (m_Locking == cfg.m_Locking);
    }
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
    class CLogoConfig
    {
    private:
        std::array <CLogoDoseConfig, GLOBALDOSEMAXCOUNT> m_Dose; // Dosiertyp
        CSimpleParser m_ServiceMode;
        BOOL          m_Modified;

    public:
        CLogoConfig() : m_Dose()
            , m_ServiceMode("")
            , m_Modified{ FALSE }
        {}

        CLogoConfig(const CLogoConfig& cfg) : m_Dose { cfg.m_Dose }
            , m_ServiceMode { cfg.m_ServiceMode }
            , m_Modified{ cfg.m_Modified }
        {}

        CLogoConfig(CLogoConfig&& cfg) noexcept: 
              m_Dose { std::move ( cfg.m_Dose) }
            , m_ServiceMode{ std::move(cfg.m_ServiceMode) }
            , m_Modified{ std::move(cfg.m_Modified) }
        {}

        CLogoConfig& operator = (const CLogoConfig& cfg) = default;

        SETGET(const BOOL, Modified);

        void Set(const int32_t index, const CLogoDoseConfig& _unit);

        const CSimpleParser& GetServiceMode() const
        { return m_ServiceMode;   }
        CSimpleParser& GetServiceMode()
        { return m_ServiceMode;   }
        void SetServiceMode(const CSimpleParser& rMode)
        {   m_ServiceMode = rMode;     }

        const CLogoDoseConfig& Get(const int32_t index) const;
        CLogoDoseConfig& Get(const int32_t index);

        uint32_t GetCount() const
        {  return _U32(m_Dose.size());   }

        BOOL operator == (const CLogoConfig&) const;
        BOOL operator != (const CLogoConfig& rCfg) const
        { return ! operator==(rCfg);    }
    };
};

