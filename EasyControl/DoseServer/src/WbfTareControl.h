//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwsControl.h
///
/// @file   LwsControl.h
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

#include "DriveControl.h"
#include "WeightControl.h"
#include "BASE/Utils//public/FloatRingBuffer.h"



class CWbfTareTask
{
	static constexpr uint32_t MAXTARACOUNT = 100U;

	enum class eTareSteps : uint32_t 
	{
		eTareInit = 0,
		eTareStart,
		eTareRunning,
		eTareStop,
		eTareError,
	};

	CDriveControl& m_DriveCtrl;
	CWeightControl& m_WeightCtrl;
	base::utils::CFloatRingBuffer< MAXTARACOUNT > m_fTaraWeightFifo;
	int32_t		m_sID;
	eTareSteps	m_step;
	uint32_t	m_t;
	uint32_t    m_StartTime;
	base::utils::CWeightPair m_LastWeight;




private:
	void StepStart();
	void StepRunning();
	void StepStop();
	void StepError();
	BOOL CheckErrorState(void);
	void SetDrivecommand();

	CWbfTareTask() = delete;

public:
	explicit CWbfTareTask(int32_t id, CDriveControl& encoderCtrl, CWeightControl& weightCtrl) : m_DriveCtrl{ encoderCtrl }
		, m_WeightCtrl { weightCtrl }
		, m_fTaraWeightFifo{}
		, m_sID{ id }
		, m_step (eTareSteps ::eTareInit)
		, m_t{ 0 }
		, m_StartTime { 0 }
		, m_LastWeight{ }
	{}

	CWbfTareTask(const CWbfTareTask&) = delete;
	CWbfTareTask& operator = (const CWbfTareTask&) = delete;
	virtual ~CWbfTareTask() = default;

	virtual void Init();
	virtual BOOL Start();
	virtual BOOL Update(const uint32_t t);
	virtual BOOL Stop();
	virtual void Close();
};





