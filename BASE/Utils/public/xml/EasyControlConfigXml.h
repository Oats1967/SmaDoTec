//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EasyControlConfigXml
///
/// @file   EasyControlConfigXml.h
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
#include "BASE/include/EasyControlConfig.h"
#include "BASE/include/LanguageMap.h"



namespace base
{
namespace xml
{

class CEasyControlConfigXml : public XmlContainer< CEasyControlConfig>
{
	const CLanguageMap	m_LanguageMap;

public:
	CEasyControlConfigXml() : XmlContainer()
	{}
	CEasyControlConfigXml(CEasyControlConfig& _field) : XmlContainer(_field)
	{}

	~CEasyControlConfigXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


