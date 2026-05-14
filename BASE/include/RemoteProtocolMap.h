//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteProtocolMap
///
/// @file   RemoteProtocolMap.h
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

#include "BASE/include/XMLStringEnumMap.h"
#include "BASE/include/RemoteConfig.h"


namespace base
{
    using TRemoteProtocolMap = CXMLStringEnumMap< RemoteServerProtocol >;
    class CRemoteProtocolMap : public TRemoteProtocolMap
    {
    public:
        CRemoteProtocolMap() : TRemoteProtocolMap(
            {
               { RemoteServerProtocol::REMOTE_PROTOCOL_NONE, "PROTOCOL_NONE" } ,
               { RemoteServerProtocol::REMOTE_PROTOCOL_OPENMODBUS, "PROTOCOL_OPENMODBUS" } ,
               { RemoteServerProtocol::REMOTE_PROTOCOL_PROFINET, "PROTOCOL_PROFINET" } ,
               { RemoteServerProtocol::REMOTE_PROTOCOL_S7SNAP, "PROTOCOL_S7SNAP" } ,
               { RemoteServerProtocol::REMOTE_PROTOCOL_S7SIMULATOR, "PROTOCOL_S7SIMULATOR" } ,
               { RemoteServerProtocol::REMOTE_PROTOCOL_PROFIBUS, "PROTOCOL_PROFIBUS" }
            })
        {}
    };
};


