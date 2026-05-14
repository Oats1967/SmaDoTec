//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleSetpointRampConfig
///
/// @file   ModuleSetpointRampConfig.h
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
class CModuleSetpointRampConfig
{
    BOOL   m_Enable;

public:
    CModuleSetpointRampConfig() : m_Enable{ FALSE }
    {}

    ~CModuleSetpointRampConfig() = default;

    SETGET(BOOL, Enable);

    BOOL operator == (const CModuleSetpointRampConfig& cfg) const;
    BOOL operator != (const CModuleSetpointRampConfig& cfg) const
    {   return !operator == (cfg);  }
};
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
inline BOOL CModuleSetpointRampConfig :: operator == (const CModuleSetpointRampConfig& cfg) const
{
    return  (m_Enable == cfg.m_Enable);
}

};

