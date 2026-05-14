//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleTotalizerConfig
///
/// @file   ModuleTotalizerConfig.h
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
class CModuleTotalizerConfig
{
    BOOL    m_Enable;

public:
    CModuleTotalizerConfig() : m_Enable{ FALSE }
    {}

    ~CModuleTotalizerConfig() = default;

    SETGET(BOOL, Enable);

    BOOL operator == (const CModuleTotalizerConfig& cfg) const;
    BOOL operator != (const CModuleTotalizerConfig& cfg) const
    {   return !operator == (cfg);  }
};
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
inline BOOL CModuleTotalizerConfig :: operator == (const CModuleTotalizerConfig& cfg) const
{
    return  (m_Enable == cfg.m_Enable);
}

};

