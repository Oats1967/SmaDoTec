//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module TouchkeyboardXml
///
/// @file   TouchkeyboardXml.h
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
#include "BASE/include/TouchkeyboardConfig.h"


namespace base
{
namespace xml
{

class CTouchkeyboardXml : public XmlContainer< CTouchkeyboardConfig>
{
public:
	CTouchkeyboardXml() : XmlContainer()
	{}
	CTouchkeyboardXml(const CTouchkeyboardConfig& _field) : XmlContainer(_field)
	{}

	~CTouchkeyboardXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


