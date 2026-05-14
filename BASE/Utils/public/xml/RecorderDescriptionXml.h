//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecorderDescriptionXml
///
/// @file   RecorderDescriptionXml.h
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
#include "BASE/Utils/public/xml/XmlTypes.h"
#include "BASE/include/RecorderDescriptionMap.h"


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
class CRecorderDescriptionXml : public XmlContainer< CRecorderDescription>
{
	const CRecorderDescriptionMap c_RecorderDescriptionMap;
	const CRecorderDelimiterMap	  c_RecorderDelimiterMap;

private:
	std::vector< tRecItemDescriptor> SlpitAttributes(const xmlentry& attriblist) const;
	std::vector< tRecItemDescriptor> ReadItem(const tinyxml2::XMLElement& config) const;

	static std::vector< tRecItemDescriptor> GetDefaultHeader(void);
	static std::vector< tRecItemDescriptor> GetDefaultFeeder(void);

public:
	CRecorderDescriptionXml() : XmlContainer()
	{}
	CRecorderDescriptionXml(CRecorderDescription& _field) : XmlContainer(_field)
	{}
	CRecorderDescriptionXml(const CRecorderDescriptionXml& _field) : XmlContainer(_field)
	{}

	~CRecorderDescriptionXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


