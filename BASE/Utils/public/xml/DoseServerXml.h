//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseServerXml
///
/// @file   DoseServerXml.h
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
#include "BASE/include/DoseServerConfig.h"
#include "BASE/Utils/public/xml/XmlContainer.h"



namespace base
{
namespace xml
{
class CDoseServerXml : public XmlContainer< CDoseServerConfig>
{
    std::string m_File;

public:
    CDoseServerXml() : XmlContainer()
    {}
    CDoseServerXml(const CDoseServerConfig& _Config) : XmlContainer(_Config)
    {}
    ~CDoseServerXml() override = default;

    BOOL Load(const std::string& _szPath2Device) final;
    BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};

