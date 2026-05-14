//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecipefileXml
///
/// @file   RecipefileXml.h
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

#include <string>
#include "BASE/types.h"
#include "BASE/include/BooleanMap.h"
#include "BASE/include/DoseTypeMap.h"
#include "BASE/Utils/public/xml/XmlContainer.h"
#include "BASE/Utils/public/RecipeItemList.h"


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

class CRecipefileXml : public XmlContainer< utils::CRecipeItemList >
{
private:
	const CYesNoMap	c_YesNoMap;
	const CDoseTypeMap	c_DoseTypeMap;

private:
	void SetEntry(const int32_t index, const utils::CRecipeItem& cfg, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config);
	utils::CRecipeItem GetEntry(const tinyxml2::XMLElement& doseelement) const;


public:
	CRecipefileXml() : XmlContainer()
	{}
	CRecipefileXml(const utils::CRecipeItemList& _Config) : XmlContainer(_Config)
	{}
	~CRecipefileXml() override = default;

	BOOL Load(const std::string & _szPath2Device) final;
	BOOL Save(const std::string & _szPath2Device = std::string("")) final;
};
};
};


