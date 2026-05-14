//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ProductDatabaseXml
///
/// @file   ProductDatabaseXml.h
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

#include "BASE/include/ProductDatabaseMap.h"
#include "BASE/Utils/public/xml/XmlContainer.h"


namespace base
{
namespace xml
{

class CProductDatabaseXml : public XmlContainer< CProductDatabaseMap >
{

public:
	CProductDatabaseXml()
	{}
	CProductDatabaseXml(CProductDatabaseMap& _field) : XmlContainer(_field)
	{}

	~CProductDatabaseXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


