//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module S7SnapXml
///
/// @file   S7SnapXml.h
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

#include "BASE/Utils/public/xml/XmlContainer.h"
#include "BASE/include/DeviceConfig.h"

namespace base
{
namespace xml
{
class CS7SnapXml : public XmlContainer< CDeviceConfig>
{
public:
    CS7SnapXml() : XmlContainer()
    {}
    CS7SnapXml(const CS7SnapXml& _field) : XmlContainer(_field)
    {}
    ~CS7SnapXml() override = default;

    BOOL Load(const std::string& _szPath2Device) final;
    BOOL Save(const std::string& szfilename = std::string("")) final;
};

};
};

