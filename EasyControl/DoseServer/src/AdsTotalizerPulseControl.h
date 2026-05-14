//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsTotalizerPulseControl
///
/// @file   AdsTotalizerPulseControl.h
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

#include "AdsWrapperControl.h"
#include "AdsLineButton.h"

class CAdsTotalizerPulseControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	AdsSensor::CAdsLineDigitalOutput m_Output;

public:
	CAdsTotalizerPulseControl();
	~CAdsTotalizerPulseControl() = default;

	void SetState(BOOL bState);
	void Init() override;
	void Exit() override;
};
//*********************************************************************************************************************
//*********************************************************************************************************************
inline void CAdsTotalizerPulseControl::SetState(BOOL bState)
{
	assert(CBaseClass::IsInit());
	assert(m_Output.GetEnable());
	m_Output.UpdateState(bState);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline void CAdsTotalizerPulseControl::Init()
{
	assert(! CBaseClass::IsInit());

	CBaseClass::Init();

	m_Output.Init();
	assert(m_Output.GetEnable());
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline void CAdsTotalizerPulseControl::Exit()
{
	assert(CBaseClass::IsInit());

	m_Output.Exit();
	CBaseClass::Exit();
}


