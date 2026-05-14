//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineGraphConfigXml
///
/// @file   LineGraphConfigXml.h
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
#include "BASE/include/LineGraphConfig.h"


namespace base
{
namespace xml
{

class CLineGraphConfigXml : public XmlContainer< CLineGraphConfig>
{
public:
	CLineGraphConfigXml() : XmlContainer()
	{}
	CLineGraphConfigXml(CLineGraphConfig& _field) : XmlContainer(_field)
	{}
	CLineGraphConfigXml(const CLineGraphConfigXml& _field) : XmlContainer(_field)
	{}

	~CLineGraphConfigXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


