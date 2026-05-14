//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseModules
///
/// @file   DoseModules.h
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

#include "BASE/Base.h"
#include "BASE/include/ModuleRefill.h"


namespace base
{
//--------------------------------------------------------------------------------------------------------------------
class CDoseModules
{
private:
    CModuleRefill       m_ModuleRefill;

public:
    CDoseModules() : m_Enable{ FALSE }
        , m_ModuleRefill{  }
    {}

    virtual ~CDoseModules() = default;

    CDoseModules(const CDoseModules&) = default;
    CDoseModules(CDoseModules&& cfg) noexcept :
        , m_ModuleRefill{ std::move(cfg.m_ModuleRefill) }
    {}

    CDoseModules& operator = (const CDoseModules&) = default;

    BOOL operator == (const CDoseModules& cfg) const
    {
        return (m_ModuleRefill == cfg.m_ModuleRefill);
    }

    BOOL operator != (const CDoseModules& cfg) const
    {
        return !operator == (cfg);
    }
};


};

