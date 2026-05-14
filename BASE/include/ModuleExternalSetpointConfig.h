//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleExternalSetpointConfig
///
/// @file   ModuleExternalSetpointConfig.h
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
class CModuleExternalSetpointConfig
{
    BOOL   m_Enable;
    BOOL   m_ExtInvSetpoint;

public:
    CModuleExternalSetpointConfig() : m_Enable{ FALSE }
    , m_ExtInvSetpoint{ FALSE }
    {}

    ~CModuleExternalSetpointConfig() = default;

    SETGET(BOOL, Enable);
    SETGET(BOOL, ExtInvSetpoint);

    BOOL operator == (const CModuleExternalSetpointConfig& cfg) const;
    BOOL operator != (const CModuleExternalSetpointConfig& cfg) const
    {   return !operator == (cfg);  }
};
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
inline BOOL CModuleExternalSetpointConfig :: operator == (const CModuleExternalSetpointConfig& cfg) const
{
    return
        (m_Enable == cfg.m_Enable) &&
        (m_ExtInvSetpoint == cfg.m_ExtInvSetpoint);
}

};

