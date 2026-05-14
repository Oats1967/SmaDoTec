//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteServerXml
///
/// @file   RemoteServerXml.h
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

#include "BASE/include/RemoteServerConfig.h"
#include "BASE/include/RemoteProtocolMap.h"
#include "BASE/Utils/public/xml/XmlContainer.h"



namespace base
{
namespace xml
{
class CRemoteServerXml :public XmlContainer< CRemoteServerConfig>
{
    CRemoteProtocolMap m_RemoteProtocolMap;

public:
    CRemoteServerXml() : XmlContainer()
    {}

    ~CRemoteServerXml() override = default;

    BOOL Load(const std::string& _szPath2Device) final;
    BOOL Save(const std::string& szfilename = std::string("")) final;
};

};
};

