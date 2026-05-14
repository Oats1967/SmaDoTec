//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleBatcherConfig
///
/// @file   ModuleBatcherConfig.h
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
class CModuleBatcherConfig
{
    BOOL    m_Enable;

public:
    CModuleBatcherConfig() : m_Enable{ FALSE }
    {}

    ~CModuleBatcherConfig() = default;

    SETGET(BOOL, Enable);

    BOOL operator == (const CModuleBatcherConfig& cfg) const;
    BOOL operator != (const CModuleBatcherConfig& cfg) const
    {   return !operator == (cfg);  }
};
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
inline BOOL CModuleBatcherConfig :: operator == (const CModuleBatcherConfig& cfg) const
{
    return  (m_Enable == cfg.m_Enable);
}

};

