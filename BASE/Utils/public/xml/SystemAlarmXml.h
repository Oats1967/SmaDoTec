//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SystemAlarmXml
///
/// @file   SystemAlarmXml.h
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
#include "BASE/include/AlarmCategoryMap.h"
#include "BASE/include/AlarmErrorMap.h"


namespace base
{
namespace xml
{

class CSystemAlarmXml : public XmlContainer< CAlarmField >
{
	const CAlarmCategoryMap c_FieldCategoryMap;
	const CAlarmErrorMap	c_FieldErrorMap;

	BOOL CheckAlarms();

public:
	CSystemAlarmXml() : c_FieldCategoryMap()
	{}
	CSystemAlarmXml(CAlarmField& _field) : XmlContainer(_field)
	{}

	~CSystemAlarmXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


