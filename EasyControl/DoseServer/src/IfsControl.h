//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IfsControl
///
/// @file   IfsControl.h
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

#include <map>
#include "BaseControl.h"
#include "AdsIfsControl.h"

class CIfsControl : public CBaseControl
{
	using CBaseClass = CBaseControl;

	/*
				|    -> StepHighReached
		High	+
				|	 -> StepMaxReached
		Max  	+
				|	 -> StepMinReached
		Min		+
				|	-> StepLowReached
		Low		+
				|	-> StepEmptyReached

	*/
protected:
	enum class eSubStep : uint32_t
	{
		eInitReached = 0,
		eEmptyReached,
		eLowReached,
		eMinReached,
		eMaxReached,
		eHighReached,
		eMaxOverflowReached,
		eHighOverflowReached,
		eEmptyOverflowReached,
	};

protected:
	typedef void (CIfsControl::*MFP)();

	const std::map <eSubStep, MFP> c_EditMap;
	CAdsIfsControl	m_AdsClient;
	eSubStep		m_eSubStep;


private:
	void Control();

protected:
	virtual BOOL IsHighSwitchSet() const;
	virtual BOOL IsMaxSwitchSet() const;
	virtual BOOL IsMinSwitchSet() const;
	virtual BOOL IsLowSwitchSet() const;

protected:
	virtual void IncreaseSetpoint();
	virtual void ReduceSetpoint();
	virtual BOOL CheckInputs();
	virtual void EnterInitReached();
	virtual void StepInitReached();
	virtual void EnterEmptyReached();
	virtual void StepEmptyReached();
	virtual void EnterLowReached();
	virtual void StepLowReached();
	virtual void EnterMinReached();
	virtual void StepMinReached();
	virtual void EnterMaxReached();
	virtual void StepMaxReached();
	virtual void EnterHighReached();
	virtual void StepHighReached();
	virtual void EnterMaxOverflowReached();
	virtual void StepMaxOverflowReached();
	virtual void EnterHighOverflowReached();
	virtual void StepHighOverflowReached();
	virtual void EnterEmptyOverflowReached();
	virtual void StepEmptyOverflowReached();

	virtual void SetStopSetpoint();
	virtual void SetNormalSetpoint();
	virtual void GetLineSetpoint();
	virtual void SetLineMassflow();

	virtual void Restart() {}

	virtual void OnEmptyReached() = 0;
	virtual void OnLowReached() = 0;
	virtual void OnMinReached() = 0;
	virtual void OnMaxReached() = 0;
	virtual void OnHighReached() = 0;
	virtual void OnMaxOverflowReached() = 0;
	virtual void OnHighOverflowReached() = 0;
	virtual void OnEmptyOverflowReached() = 0;


public:
	CIfsControl(int32_t id);
	~CIfsControl(void) override = default;

	BOOL Execute(void) override;
	BOOL InitExecute(void) override;
	BOOL ExitExecute(void) override;
	BOOL Start(const uint32_t t) override;
	BOOL Stop(void) override;
};
//*************************************************************************************
//*************************************************************************************
inline BOOL CIfsControl::IsHighSwitchSet() const
{
	return m_AdsClient.GetState(CAdsIfsControl::eSensorsHigh);
}
//*************************************************************************************
//*************************************************************************************
inline BOOL CIfsControl::IsMaxSwitchSet() const
{
	return m_AdsClient.GetState(CAdsIfsControl::eSensorsMax);
}
//*************************************************************************************
//*************************************************************************************
inline BOOL CIfsControl::IsMinSwitchSet() const
{
	return m_AdsClient.GetState(CAdsIfsControl::eSensorsMin);
}
//*************************************************************************************
//*************************************************************************************
inline BOOL CIfsControl::IsLowSwitchSet() const
{
	return m_AdsClient.GetState(CAdsIfsControl::eSensorsLow);
}
