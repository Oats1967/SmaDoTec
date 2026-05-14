//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LayoutConfigXml
///
/// @file   LayoutConfigXml.h
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

#include "BASE/include/LayoutConfig.h"
#include "BASE/include/ArchitectureMap.h"
#include "BASE/include/BooleanMap.h"
#include "BASE/Utils/public/xml/XmlContainer.h"


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

class CLayoutConfigXml : public XmlContainer< CLayoutConfig >
{
	const CArchitectureMap		c_ArchitectureMap;
	const CYesNoMap				c_YesNoMap;

public:
	CLayoutConfigXml() : XmlContainer()
	{}
	CLayoutConfigXml(const CLayoutConfig& _Config) : XmlContainer(_Config)
	{}
	~CLayoutConfigXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


