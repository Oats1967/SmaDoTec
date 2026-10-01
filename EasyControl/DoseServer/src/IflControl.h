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
		eInit = 0,
		eMinLevel,		// X <= Min-Level
		eMinMaxLevel,   // Min <= X <= Max 
		eMaxLevel,		// X >= Max 
	};


	static constexpr uint32_t IFL_MEASUREBUFFERSIZE = 8U;

	CLwfTareTask			m_TareTask;
	CFeederWeightControl	m_WeightCtrl;
	base::math::CPidControl	m_PidControl;
	base::utils::CWeightPair			m_aLoadCell0;
	base::utils::CWeightPair			m_aLoadCell1;


	BOOL m_bExternalSetpointChanged;
	float32_t m_fWeight;
	float32_t m_fMinLevel;
	float32_t m_fMaxLevel;
	float32_t m_fActSetpoint;
	float32_t m_fActMaxSetpoint;
	float32_t m_fActMinSetpoint;
	uint32_t m_SampleTime;
	uint32_t m_tWeightNext;
	uint32_t m_tNext;
	uint32_t m_tMinStart;
	uint32_t m_tMaxStart;
	eSubSteps m_eSubSteps;

private:
	BOOL Control();
	void CheckAlarm();
	void GetWeight();
	BOOL UpdateWeight(void);
	void InitWeight(void);


	void GetSampleTime();
	void EnterInitLevel();
	void EnterMinLevel();
	void EnterMaxLevel();
	void EnterMinMaxLevel();
	void RunMinLevel();
	void RunMaxLevel();
	void RunMinMaxLevel();
	void RunInitLevel();
	void SetLineSetpoint(float32_t fSetpoint);
	float32_t CalSetpoint(const float32_t fX) const;
	void CalcMinMaxSetpoint(const float32_t fX);


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

