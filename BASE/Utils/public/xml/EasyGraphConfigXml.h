//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EasyGraphConfigXml
///
/// @file   EasyGraphConfigXml.h
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
#include "BASE/include/EasyGraphConfig.h"
#include "BASE/include/LanguageMap.h"


namespace base
{
namespace xml
{

class CEasyGraphConfigXml : public XmlContainer< CEasyGraphConfig>
{
	const CLanguageMap	m_LanguageMap;

public:
	CEasyGraphConfigXml() : XmlContainer()
	{}
	CEasyGraphConfigXml(CEasyGraphConfig& _field) : XmlContainer(_field)
	{}

	~CEasyGraphConfigXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


