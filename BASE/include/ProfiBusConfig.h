//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ProfiBusConfig
///
/// @file   ProfiBusConfig.h
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

#include <vector>
#include <string>
#include "BASE/Base.h"
#include "BASE/include/GenericRemoteConfig.h"

namespace base
{
struct ProfiBusDescriptor
{
    uint32_t        m_Elements = 0;                    // Max Number of elements
    RemoteFieldType m_FieldType = RemoteFieldType::INT8;                // float/int etc.
    RemoteDirection m_Direction = RemoteDirection::Read;
    RemoteCategory  m_Category = RemoteCategory::R_CONTROL;
    std::string     m_TaskItem;
};

using ProfiBusDescriptorVec = std::vector< ProfiBusDescriptor >;

class CProfiBusConfig
{
    ProfiBusDescriptorVec   m_List;
    int32_t                 m_TaskId;
    BOOL                    m_ProfiNet;

public:
    CProfiBusConfig() : m_List(), m_TaskId(-1), m_ProfiNet(FALSE)
    {}
    ~CProfiBusConfig() = default;

    ProfiBusDescriptorVec& GetList()
    {   return m_List;   }
    const ProfiBusDescriptorVec& GetList() const
    {  return m_List;   }
    void SetList(const ProfiBusDescriptorVec& list)
    {   m_List = list;   }

    SETGET(int32_t, TaskId)
    SETGET(BOOL, ProfiNet)
};

};

