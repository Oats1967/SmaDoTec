//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EasyGraphSettingsXml
///
/// @file   EasyGraphSettingsXml.h
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
#include "BASE/include/EasyGraphSettings.h"
#include "BASE/include/BooleanMap.h"
#include "BASE/include/MassflowSelectMap.h"
#include "BASE/include/LineCategoryMap.h"


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

class CEasyGraphSettingsXml : public XmlContainer< CEasyGraphSettings>
{
	const CYesNoMap				c_YesNoMap;
	const CMassflowSelectMap	c_MassflowSelectMap;
	const CLineCategoryMap		c_LineCategoryMap;

protected:
	BOOL GetAttrib(const tinyxml2::XMLElement& config, const base::eMassflowSelect select);
	BOOL SetAttrib(tinyxml2::XMLDocument& rDoc, tinyxml2::XMLNode& config, const base::eMassflowSelect select);

public:
	CEasyGraphSettingsXml() : XmlContainer()
	{}
	CEasyGraphSettingsXml(const CEasyGraphSettings& _field) : XmlContainer(_field)
	{}

	~CEasyGraphSettingsXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


