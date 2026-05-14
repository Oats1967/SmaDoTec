//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module OpenModbusXml
///
/// @file   OpenModbusXml.cpp
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

#include "BASE/include/ModbusConfig.h"
#include "BASE/Utils/public/xml/XmlContainer.h"



namespace base
{
namespace xml
{

class COpenModbusXml : public XmlContainer< CModbusConfig>
{
public:
    COpenModbusXml() = default;

    COpenModbusXml(const CModbusConfig& _Config) : XmlContainer(_Config)
    {}

    ~COpenModbusXml() override = default;

    BOOL Load(const std::string& _szPath2Device) final;
    BOOL Save(const std::string& szfilename = std::string("")) final;
};
};
};


