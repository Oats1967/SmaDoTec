//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IflControl
///
/// @file   IflControl.h
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

#include "BASE/math/public/PidControl.h"
#include "BaseControl.h"
#include "FeederWeightControl.h"
#include "LwfTareTask.h"


class CIflControl : public CBaseControl
{
	using CBaseClass = CBaseControl;

	enum class eSubSteps : uint32_t
	{
		eDefault = 0,
		eMaxLevel,
		eMinLevel,
		eNormLevel,
	};

	CLwfTareTask			m_TareTask;
	CFeederWeightControl	m_WeightCtrl;
	base::math::CPidControl	m_PidControl;

	BOOL m_bPidParameterChanged;
	BOOL m_bExternalSetpointChanged;
	BOOL m_bMinReached;
	BOOL m_bMaxReached;
	BOOL m_bMinReachedLast;
	BOOL m_bMaxReachedLast;
	float32_t m_fWeight;
	float32_t m_fMinLevel;
	float32_t m_fMaxLevel;
	float32_t m_fNormLevel;
	float32_t m_fSetpoint;
	float32_t m_fPidPropGainGross;
	uint32_t m_SampleTime;
	uint32_t m_tNext;
	eSubSteps m_eSubSteps;

private:
	BOOL Control();
	void CheckAlarm();
	BOOL GetWeight();
	BOOL GetChangedSampleInterval();
	BOOL GetPidParameter();
	void EnterDefaultLevel();
	void EnterMinLevel();
	void EnterMaxLevel();
	void EnterNormLevel();
	void RunMinLevel();
	void RunMaxLevel();
	void RunNormLevel();
	void RunDefaultLevel();
	void SetLineSetpoint(float32_t fSetpoint);

protected:
	BOOL GetLineSetpoint() const override;

public:
	CIflControl(int32_t id, loadcell::ILCModuleInterface& rlc);
	~CIflControl(void) override = default;

	BOOL Execute			( void ) override;
	BOOL InitExecute		( void ) override;
	BOOL ExitExecute		( void ) override;
	BOOL Start	            ( const uint32_t t ) override;
	BOOL Stop		        ( void ) override;
};

