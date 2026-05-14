//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleLocalSetpointConfig
///
/// @file   ModuleLocalSetpointConfig.h
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
class CModuleLocalSetpointConfig
{
    BOOL   m_Enable;

public:
    CModuleLocalSetpointConfig() : m_Enable{ FALSE }
    {}

    ~CModuleLocalSetpointConfig() = default;

    SETGET(BOOL, Enable);

    BOOL operator == (const CModuleLocalSetpointConfig& cfg) const;
    BOOL operator != (const CModuleLocalSetpointConfig& cfg) const
    {   return !operator == (cfg);  }
};
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
inline BOOL CModuleLocalSetpointConfig :: operator == (const CModuleLocalSetpointConfig& cfg) const
{
    return (m_Enable == cfg.m_Enable);
}

};

