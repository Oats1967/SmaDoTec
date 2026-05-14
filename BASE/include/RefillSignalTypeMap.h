//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RefillSignalTypeMap
///
/// @file   RefillSignalTypeMap.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <string>
#include "BASE/types.h"
#include "BASE/include/XMLStringEnumMap.h"
#include "BASE/include/RefillSignalType.h"

namespace base
{
    using TRefillSignalTypeMap = CXMLStringEnumMap< refillsignal::eRefillSignal>;
    class CRefillSignalTypeMap : public  TRefillSignalTypeMap
    {
    public:
        CRefillSignalTypeMap() : TRefillSignalTypeMap(
            {
                { refillsignal::eRefillSignal::eNormal, "Normal" },
                { refillsignal::eRefillSignal::eCyclic, "Cyclic" },
                { refillsignal::eRefillSignal::eSequenceHIGH, "SequenceHIGH" },
                { refillsignal::eRefillSignal::eSequenceLOW, "SequenceLOW" }
            })
        {
            assert(m_Map.size() == _S32(refillsignal::eRefillSignal::eMax));
        }
    };
};

