//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsPidControl
///
/// @file   AdsPidControl.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <cassert>
#include "AdsMassflowControl.h"
#include "AdsDoseSensor.h"


class CAdsPidControl : public CAdsMassflowControl
{
	using CBaseClass = CAdsMassflowControl;

	AdsSensor::CAdsDoseAnalogInput	m_GetMassflow;

public:
	CAdsPidControl(const int32_t index);

	void Init() override;
	BOOL GetMassflow(float32_t& value);
};
//*********************************************************************************************
//*********************************************************************************************
inline void CAdsPidControl::Init()
{
	CBaseClass::Init();
	m_GetMassflow.Init();
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsPidControl::GetMassflow(float32_t& value)
{
	assert(CBaseClass::IsInit());
	return m_GetMassflow.UpdateState(value);
}




