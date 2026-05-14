//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineConfig
///
/// @file   LineConfig.h
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
#include "BASE/include/ModuleExternalSetpointConfig.h"
#include "BASE/include/ModuleSetpointRampConfig.h"
#include "BASE/include/ModuleLineSetpointConfig.h"
#include "BASE/include/ModuleLocalSetpointConfig.h"
#include "BASE/include/ModuleBatcherConfig.h"
#include "BASE/include/ModuleTotalizerConfig.h"
#include "BASE/include/ModuleTotalizerPulseConfig.h"

namespace base
{

    class CLineConfig
    {
        CModuleExternalSetpointConfig   m_ModuleExternalSetpointCfg;
        CModuleSetpointRampConfig       m_ModuleSetpointRampCfg;
        CModuleLocalSetpointConfig      m_ModuleLocalSetpointCfg;
        CModuleLineSetpointConfig       m_ModuleLineSetpointCfg;
        CModuleBatcherConfig            m_ModuleBatcherCfg;
        CModuleTotalizerConfig          m_ModuleTotalizerCfg;
        CModuleTotalizerPulseConfig   m_ModuleTotalizerPulseCfg;

    public:
        CLineConfig(void) = default;

        SETGET(const CModuleExternalSetpointConfig&, ModuleExternalSetpointCfg)
        SETGET(const CModuleSetpointRampConfig&, ModuleSetpointRampCfg)
        SETGET(const CModuleLocalSetpointConfig&, ModuleLocalSetpointCfg)
        SETGET(const CModuleLineSetpointConfig&, ModuleLineSetpointCfg)
        SETGET(const CModuleBatcherConfig&, ModuleBatcherCfg)
        SETGET(const CModuleTotalizerConfig&, ModuleTotalizerCfg)
        SETGET(const CModuleTotalizerPulseConfig&, ModuleTotalizerPulseCfg)

        BOOL operator == (const CLineConfig& cfg) const;
        BOOL operator != (const CLineConfig& cfg) const;
    };


    inline BOOL CLineConfig::operator == (const CLineConfig& cfg) const
    {
        return
            (m_ModuleExternalSetpointCfg == cfg.m_ModuleExternalSetpointCfg) &&
            (m_ModuleSetpointRampCfg == cfg.m_ModuleSetpointRampCfg) &&
            (m_ModuleLocalSetpointCfg == cfg.m_ModuleLocalSetpointCfg) &&
            (m_ModuleLineSetpointCfg == cfg.m_ModuleLineSetpointCfg) &&
            (m_ModuleBatcherCfg == cfg.m_ModuleBatcherCfg) &&
            (m_ModuleTotalizerCfg == cfg.m_ModuleTotalizerCfg) &&
            (m_ModuleTotalizerPulseCfg == cfg.m_ModuleTotalizerPulseCfg);
    }

    inline BOOL CLineConfig::operator != (const CLineConfig& cfg) const
    {    return !operator == (cfg);   }
};