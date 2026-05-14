//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ProfiBusDeviceXml
///
/// @file   ProfiBusDeviceXml.h
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

#include <map>
#include <vector>
#include "BASE/Utils/public/xml/XmlContainer.h"
#include "BASE/Utils/public/xml/XmlTypes.h"
#include "BASE/include/ProfiBusConfig.h"
#include "BASE/include/RemoteCategoryMap.h"
#include "BASE/include/RemoteFieldTypeMap.h"

namespace base
{
namespace xml
{
class CProfiBusDeviceXml : public XmlContainer< CProfiBusConfig>
{
    const CRemoteCategoryMap  c_CategoryMap;
    const CRemoteFieldTypeMap c_FieldTypeMap;

private:
    ProfiBusDescriptor SlpitAttributes(const xmlentry& attriblist) const;

public:
    CProfiBusDeviceXml() : XmlContainer(), c_CategoryMap(), c_FieldTypeMap()
    {}

    ~CProfiBusDeviceXml() override = default;

    BOOL Load(const std::string& _szPath2Device) final;
    BOOL Save(const std::string& szfilename = std::string("")) final;
};

};
};

