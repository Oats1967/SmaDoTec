//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module GenericRemoteXml
///
/// @file   GenericRemoteXml.h
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

#include "BASE/Utils/public/xml/XmlTypes.h"
#include "BASE/Utils/public/xml/XmlContainer.h"
#include "BASE/include/RemoteCategoryMap.h"
#include "BASE/include/RemoteFieldTypeMap.h"
#include "BASE/include/RemoteProcessControlMap.h"
#include "BASE/include/RemoteProcessStatusMap.h"
#include "BASE/include/RemoteAlarmStatusMap.h"
#include "BASE/include/RemoteLineProcessControlMap.h"
#include "BASE/include/RemoteLineAlarmStatusMap.h"
#include "BASE/include/RemoteLineProcessStatusMap.h"
#include "BASE/include/RemoteMaskMap.h"




namespace tinyxml2
{
    class XMLElement;
    class XMLDocument;
    class XMLNode;
};

namespace base
{
namespace xml
{
class CGenericRemoteXml : public XmlContainer< CGenericRemoteConfig>
{
    const CRemoteCategoryMap                    c_CategoryMap;
    const CRemoteFieldTypeMap                   c_FieldTypeMap;
    const CRemoteProcessControlMap              c_ProcessControlMap;
    const CRemoteProcessStatusMap               c_ProcessStatusMap;
    const CRemoteAlarmStatusMap                 c_AlarmStatusMap;
    const CRemoteLineProcessControlMap          c_LineProcessControlMap;
    const CRemoteLineAlarmStatusMap             c_LineAlarmStatusMap;
    const CRemoteLineProcessStatusMap           c_LineProcessStatusMap;
    const CRemoteMaskMap                        c_RemoteMaskMap;


private:
    BlockDescriptor SlpitAttributes(const xmlentry& attriblist) const;
    BOOL CheckResults();

    BlockDescriptor ReadItem(const tinyxml2::XMLElement& config) const;
    void ReadBlockItem(const tinyxml2::XMLElement& config, BlockDescriptorVec& blocklist) const;
    void ReadBlockArray(const tinyxml2::XMLElement& config, BlockDescriptorVec& blocklist) const;


    void ReadEntry(const tinyxml2::XMLElement& config, base::ProcessControlDescriptor& field);
    void ReadEntry(const tinyxml2::XMLElement& config, base::ProcessStatusDescriptor& field);
    void ReadEntry(const tinyxml2::XMLElement& config, base::AlarmStatusDescriptor& field);
    void ReadEntry(const tinyxml2::XMLElement& config, base::LineProcessControlDescriptor& field);
    void ReadEntry(const tinyxml2::XMLElement& config, base::LineAlarmStatusDescriptor& field);
    void ReadEntry(const tinyxml2::XMLElement& pcontrol, base::LineProcessStatusDescriptor& field);

    void ReadEntry(const tinyxml2::XMLNode& config, base::MaskDescriptor& field);
    void WriteEntry(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, const base::ProcessControlDescriptor& field);
    void WriteEntry(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, const base::ProcessStatusDescriptor& field);
    void WriteEntry(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, const base::AlarmStatusDescriptor& field);
    void WriteEntry(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, const base::LineProcessControlDescriptor& field);
    void WriteEntry(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, const base::LineAlarmStatusDescriptor& field);
    void WriteEntry(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, const base::LineProcessStatusDescriptor& field);
    void WriteEntry(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, const base::MaskDescriptor& field);


    xmlattributevector ParseElements(const tinyxml2::XMLNode& config);
    void WriteElements(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, const std::string& rName, const xmlattributevector& vec);

public:
    CGenericRemoteXml() : XmlContainer(), c_CategoryMap()
    {}
#if 0
    CGenericRemoteXml(const CGenericRemoteXml& _field) : XmlContainer(_field), c_CategoryMap(_field.c_CategoryMap)
    {}
#endif
    ~CGenericRemoteXml() override = default;

    BOOL Load(const std::string& _szPath2Device) final;
    BOOL Save(const std::string& szfilename = std::string("")) final;
};

};
};

