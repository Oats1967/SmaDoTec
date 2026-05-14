//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsMassflowControl
///
/// @file   AdsMassflowControl.h
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
#include "AdsWrapperControl.h"
#include "AdsDoseMassflowOutput.h"


class CAdsMassflowControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	AdsSensor::CAdsDoseMassflowOutput   m_SetMassflow;

public:
	CAdsMassflowControl(const int32_t index) : CAdsWrapperControl{ index }
		, m_SetMassflow{ index }
	{}

	void Init() override;
	void Exit() override;
	BOOL SetMassflow(const float32_t);
};
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsMassflowControl::SetMassflow(const float32_t value)
{
	assert(CBaseClass::IsInit());
	return m_SetMassflow.SetMassflow(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline void CAdsMassflowControl::Init()
{
	assert(!CBaseClass::IsInit());
	m_SetMassflow.Init();
	CBaseClass::Init();
}
//*********************************************************************************************
//*********************************************************************************************
inline void CAdsMassflowControl::Exit()
{
	assert(CBaseClass::IsInit());
	m_SetMassflow.Exit();
	CBaseClass::Exit();
}


