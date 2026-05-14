//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteLineProcessStatusMap
///
/// @file   RemoteLineProcessStatusMap.h
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
    using TRemoteLineProcessStatusMap = CXMLStringEnumMap< RemoteLineProcessStatus >;
    class CRemoteLineProcessStatusMap : public TRemoteLineProcessStatusMap
    {
    public:
        CRemoteLineProcessStatusMap() : TRemoteLineProcessStatusMap(
            {
                { RemoteLineProcessStatus::eLineStarted, "LineStart" },
                { RemoteLineProcessStatus::eExtLineSetpoint, "ExtLineSetpoint" },
                { RemoteLineProcessStatus::eExtLineSetpointInv, "ExtLineSetpointInv" } 
            })
            {
                assert(_S32(m_Map.size()) == _S32(RemoteLineProcessStatus::eProcessLineStatusMax));
            }
    };
};
