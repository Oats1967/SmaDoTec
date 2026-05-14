//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteFieldTypemap
///
/// @file   RemoteFieldTypemap.h
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


#include <cassert>
#include "XMLStringEnumMap.h"
#include "BASE/include/GenericRemoteConfig.h"



namespace base
{
    //****************************************************************************************************************************
    //****************************************************************************************************************************
    using TRemoteFieldTypeMap = CXMLStringEnumMap< RemoteFieldType >;
    class CRemoteFieldTypeMap : public TRemoteFieldTypeMap
    {
    public:
        CRemoteFieldTypeMap() : TRemoteFieldTypeMap(
            {
                { RemoteFieldType::UINT8, "UINT8" },
                { RemoteFieldType::UINT16, "UINT16" },
                { RemoteFieldType::UINT32, "UINT32" },
                { RemoteFieldType::UINT64, "UINT64" },
                { RemoteFieldType::INT8, "INT8" },
                { RemoteFieldType::INT16, "INT16" },
                { RemoteFieldType::INT32, "INT32" },
                { RemoteFieldType::INT64, "INT64" },
                { RemoteFieldType::FLOAT32, "FLOAT32" },
                { RemoteFieldType::FLOAT64, "FLOAT64" }
            })
        {
            assert(_S32(m_Map.size()) == _S32(RemoteFieldType::MAX));
        }
    };
};