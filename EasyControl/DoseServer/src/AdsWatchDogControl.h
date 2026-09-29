//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsWatchDogControl
///
/// @file   AdsWatchDogControl.h
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

class CAdsWatchDogControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

	AdsSensor::CAdsLineDigitalOutput m_Output;

public:
	CAdsWatchDogControl();
	~CAdsWatchDogControl() = default;

	void SetState(BOOL bState);
	BOOL IsEnabled();
	void Init() override;
	void Exit() override;
};
//*********************************************************************************************************************
//*********************************************************************************************************************
inline void CAdsWatchDogControl::SetState(BOOL bState)
{
	assert(CBaseClass::IsInit());
	//assert(m_Output.GetEnable());
	m_Output.UpdateState(bState);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CAdsWatchDogControl::IsEnabled()
{
	assert(CBaseClass::IsInit());
	return m_Output.GetEnable();
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline void CAdsWatchDogControl::Init()
{
	assert(! CBaseClass::IsInit());

	CBaseClass::Init();

	m_Output.Init();
}
//*********************************************************************************************************************
//*********************************************************************************************************************
inline void CAdsWatchDogControl::Exit()
{
	assert(CBaseClass::IsInit());

	m_Output.Exit();
	CBaseClass::Exit();
}


