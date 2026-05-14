//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WbfTareTask
///
/// @file   WbfTareTask.h
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

#include "BASE/include/LCType.h"
#include "BASE/Utils/public/WeightPair.h"
#include "BASE/include/Statemachine.h"
#include "BASE/Utils//public/FloatRingBuffer.h"


enum class eWbfTareSteps : uint32_t
{
	eTareInit = 0,
	eTareStart,
	eTareRunning,
	eTareStop,
	eTareError,
};


class CWeightControl;
class CAlarmManager;
class CDriveControl;
class CWbfTareTask : public base::CStatemachine< eWbfTareSteps>
{
	static constexpr uint32_t MAXTARACOUNT = 100U;

	CDriveControl* m_pDriveCtrl;
	CWeightControl* m_pWeightCtrl;
	CAlarmManager* m_pAlarm;
	base::utils::CFloatRingBuffer< MAXTARACOUNT > m_fTaraWeightFifo;
	uint32_t    m_StartTime;
	base::utils::CWeightPair m_LastWeight;

private:
	void StepStart();
	void StepRunning();
	void StepStop();
	void StepError();
	BOOL CheckErrorState(void);
	void SetDrivecommand();
	void InternalStop();

	CWbfTareTask() = delete;

public:
	explicit CWbfTareTask(int32_t id)  : CStatemachine(id)
		, m_pDriveCtrl { nullptr	}
		, m_pWeightCtrl { nullptr }
		, m_pAlarm{ nullptr }
		, m_fTaraWeightFifo{}
		, m_StartTime { 0 }
		, m_LastWeight{ }
	{
	}

	CWbfTareTask(const CWbfTareTask&) = delete;
	CWbfTareTask& operator = (const CWbfTareTask&) = delete;
	virtual ~CWbfTareTask() = default;

	BOOL InitExecute() override;
	BOOL Stop() override;
	BOOL Start(const uint32_t t) override;
	BOOL Update(const uint32_t t) override;
	BOOL ExitExecute() override;

	void registerWeightCtrl(CWeightControl& weightCtrl);
	void registerAlarmManager(CAlarmManager& rAlarm);
	void registerDriveCtrl(CDriveControl& rDriveControl);
};





