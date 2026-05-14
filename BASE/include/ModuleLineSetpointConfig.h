//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleLineSetpointConfig
///
/// @file   ModuleLineSetpointConfig.h
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
class CModuleLineSetpointConfig
{
    BOOL    m_Enable;
    BOOL    m_LineRelease;
    BOOL    m_LineStart;


public:
    CModuleLineSetpointConfig() : m_Enable{ FALSE }
        , m_LineRelease { FALSE }
        , m_LineStart   { FALSE }
    {}

    ~CModuleLineSetpointConfig() = default;

    SETGET(BOOL, Enable);
    SETGET(BOOL, LineRelease);
    SETGET(BOOL, LineStart);

    BOOL operator == (const CModuleLineSetpointConfig& cfg) const;
    BOOL operator != (const CModuleLineSetpointConfig& cfg) const
    {   return !operator == (cfg);  }
};
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
inline BOOL CModuleLineSetpointConfig :: operator == (const CModuleLineSetpointConfig& cfg) const
{
    return  (m_Enable == cfg.m_Enable) &&
        (m_LineRelease == cfg.m_LineRelease) &&
        (m_LineStart == cfg.m_LineStart);
}

};

