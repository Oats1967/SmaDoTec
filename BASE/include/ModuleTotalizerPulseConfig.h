//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleTotalizerPulseConfig
///
/// @file   ModuleTotalizerPulseConfig.h
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


namespace base
{
class CModuleTotalizerPulseConfig
{
    BOOL    m_Enable;

public:
    CModuleTotalizerPulseConfig() : m_Enable{ FALSE }
    {}

    ~CModuleTotalizerPulseConfig() = default;

    SETGET(BOOL, Enable);

    BOOL operator == (const CModuleTotalizerPulseConfig& cfg) const;
    BOOL operator != (const CModuleTotalizerPulseConfig& cfg) const
    {   return !operator == (cfg);  }
};
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
inline BOOL CModuleTotalizerPulseConfig :: operator == (const CModuleTotalizerPulseConfig& cfg) const
{
    return  (m_Enable == cfg.m_Enable);
}

};

