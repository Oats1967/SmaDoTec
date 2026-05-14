//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDriveControl
///
/// @file   AdsDriveControl.h
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

#include "AdsDoseButton.h"
#include "AdsDoseSensor.h"
#include "AdsStateControlContainer.h"

class CAdsDriveControl : public CAdsWrapperControl
{
	using CBaseClass = CAdsWrapperControl;

public:

private:
	AdsSensor::CAdsDoseDigitalInput  m_DriveError;
	AdsSensor::CAdsDoseDigitalInput  m_MotorLoad;
	AdsSensor::CAdsDoseDigitalInput  m_DriveErrorPaddle;
	AdsSensor::CAdsDoseDigitalOutput m_DriveRelease;
	AdsSensor::CAdsDoseDigitalOutput m_Start;
	AdsSensor::CAdsDoseDigitalOutput m_Operate;
	AdsSensor::CAdsDoseAnalogOutput  m_DriveCommand;
	AdsSensor::CAdsDoseAnalogInput   m_DecoderInput;
	AdsSensor::CAdsDoseSensor<uint32_t> m_PulseCounter;

	AdsSensor::CAdsStateControlContainer<> m_Container;

public:
	CAdsDriveControl(const int32_t index);
	virtual ~CAdsDriveControl() = default;

	void Init() override;
	void Exit() override;
	BOOL UpdateState() override;
	void SetOperate(BOOL bOperate);
	void SetDriveRelease(BOOL bValue);
	void SetStart(const BOOL bValue);
	void SetDriveCommand(const float32_t fValue);

	BOOL GetDriveError(BOOL& value);
	BOOL GetMotorLoad(BOOL& value);
	BOOL GetDriveErrorPaddle(BOOL& value);
	BOOL GetDriveErrorAfterUpdate(BOOL& value) const;
	BOOL GetMotorLoadAfterUpdate(BOOL& value) const;
	BOOL GetDriveErrorPaddleAfterUpdate(BOOL& value) const;
};
//*********************************************************************************************
//*********************************************************************************************
//************************************************************************************************************
//************************************************************************************************************
inline void CAdsDriveControl::SetDriveRelease(BOOL bValue)
{
	assert(CBaseClass::IsInit());
	m_DriveRelease.UpdateState(bValue);
}
//************************************************************************************************************
//************************************************************************************************************
inline void CAdsDriveControl::SetOperate(BOOL bValue)
{
	assert(CBaseClass::IsInit());
	m_Operate.UpdateState(bValue);
}
//************************************************************************************************************
//************************************************************************************************************
inline void CAdsDriveControl::SetStart(BOOL bValue)
{
	assert(CBaseClass::IsInit());
	m_Start.UpdateState(bValue);
}
//************************************************************************************************************
//************************************************************************************************************
inline void CAdsDriveControl::SetDriveCommand(const float32_t fValue)
{
	assert(CBaseClass::IsInit());
	m_DriveCommand.UpdateState(fValue);
}
//************************************************************************************************************
//************************************************************************************************************
inline BOOL CAdsDriveControl::GetDriveError(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_DriveError.UpdateState(value);
}
//************************************************************************************************************
//************************************************************************************************************
inline BOOL CAdsDriveControl::GetMotorLoad(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_MotorLoad.UpdateState(value);
}
//************************************************************************************************************
//************************************************************************************************************
inline BOOL CAdsDriveControl::GetDriveErrorPaddle(BOOL& value)
{
	assert(CBaseClass::IsInit());
	return m_DriveErrorPaddle.UpdateState(value);
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsDriveControl::UpdateState()
{
	assert(CBaseClass::IsInit());
	return m_Container.UpdateState();
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsDriveControl::GetDriveErrorAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_DriveError.GetEnable();
	value = m_DriveError.GetState();
	return bEnable;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsDriveControl::GetMotorLoadAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_MotorLoad.GetEnable();
	value = m_MotorLoad.GetState();
	return bEnable;
}
//*********************************************************************************************
//*********************************************************************************************
inline BOOL CAdsDriveControl::GetDriveErrorPaddleAfterUpdate(BOOL& value) const
{
	assert(CBaseClass::IsInit());
	auto bEnable = m_DriveErrorPaddle.GetEnable();
	value = m_DriveErrorPaddle.GetState();
	return bEnable;
}

