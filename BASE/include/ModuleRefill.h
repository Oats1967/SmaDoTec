//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleRefill
///
/// @file   ModuleRefill.h
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

#include <cassert>
#include <array>
#include <vector>
#include <utility>
#include "BASE/Base.h"
#include "BASE/include/LCType.h"
#include "BASE/include/RefillSignalType.h"


namespace base
{
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
class CRefillSignalConfig
{
public:
    using RefillSignalVec = std::vector < std::pair<BOOL, uint32_t>>;

public:
    RefillSignalVec m_RefillSignal;
    base:: refillsignal::eRefillSignal m_eRefillSignal;

public:
    CRefillSignalConfig() : m_RefillSignal{}
        , m_eRefillSignal { base::refillsignal::eRefillSignal ::eNormal }
    {}
    virtual ~CRefillSignalConfig() = default;

    CRefillSignalConfig(const CRefillSignalConfig&) = default;
    CRefillSignalConfig(CRefillSignalConfig&& cfg) noexcept :
        m_RefillSignal { std::move(cfg.m_RefillSignal) }
        , m_eRefillSignal{ std::move(cfg.m_eRefillSignal) }
    {}

    CRefillSignalConfig& operator = (const CRefillSignalConfig&) = default;

    BOOL operator == (const CRefillSignalConfig& cfg) const
    {   return (m_RefillSignal == cfg.m_RefillSignal) && (m_eRefillSignal == cfg.m_eRefillSignal);    }

    BOOL operator != (const CRefillSignalConfig& cfg) const
    {    return !operator == (cfg);   }
};

//--------------------------------------------------------------------------------------------------------------------
class CModuleRefill
{
private:
    BOOL                m_Enable;
    int32_t             m_FeederID;
    CRefillSignalConfig m_RefillSignal;

public:
    CModuleRefill() : m_Enable{ FALSE }
        ,  m_FeederID{ 0 }
        , m_RefillSignal()
    {}

    virtual ~CModuleRefill() = default;

    CModuleRefill(const CModuleRefill&) = default;
    CModuleRefill(CModuleRefill&& cfg) noexcept :
          m_Enable{ std::move(cfg.m_Enable) }
        , m_FeederID{ std::move(cfg.m_FeederID) }
        , m_RefillSignal{ std::move(cfg.m_RefillSignal) }
    {}

    CModuleRefill& operator = (const CModuleRefill&) = default;

    BOOL operator == (const CModuleRefill& cfg) const
    {
        return (m_Enable == cfg.m_Enable) && 
               (m_FeederID == cfg.m_FeederID) && 
               (m_RefillSignal == cfg.m_RefillSignal);
    }

    BOOL operator != (const CModuleRefill& cfg) const
    {
        return !operator == (cfg);
    }

    SETGET(BOOL, Enable)
    SETGET(int32_t, FeederID)
    SETGET(const CRefillSignalConfig&, RefillSignal)
};


};

