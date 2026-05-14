//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module GenericRemoteConfig
///
/// @file   GenericRemoteConfig.h
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
#include "BASE/types.h"
#include "GenericRemoteConfig.h"

namespace base
{

struct SimDescriptor
{
    struct FieldEntry
    {
        union FieldValue
        {
            float32_t valF32;
            float64_t valF64;
            uint64_t  valU64;
            uint32_t  valU32;
            uint16_t  valU16;
            uint8_t   valU8;
            int64_t   valS64;
            int32_t   valS32;
            int16_t   valS16;
            int8_t    valS8;
        } value;
        RemoteFieldType type;
    };

    RemoteCategory  m_Category = RemoteCategory::R_CONTROL;
    RemoteFieldType m_FieldType = RemoteFieldType::FLOAT32;
    std::vector< FieldEntry > m_Data;
    uint32_t        m_Elements = 0;                    // Max Number of elements
};

using SimDescriptorVec = std::vector< SimDescriptor >;

class CSimValuesConfig
{
    SimDescriptorVec m_List;

public:
    CSimValuesConfig() : m_List()
    {}
    ~CSimValuesConfig() = default;

    SimDescriptorVec& GetList()
    {   return m_List;   }
    const SimDescriptorVec& GetList() const
    {  return m_List;   }

    void SetList(const SimDescriptorVec& list)
    {   m_List = list;   }
};

};

