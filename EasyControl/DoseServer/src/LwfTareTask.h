//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwfTareTask
///
/// @file   LwfTareTask.h
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

enum class eLwfTareSteps : uint32_t
{
	eTareInit = 0,
	eTareRunning,
	eTareError
};

class CWeightControl;
class CAlarmManager;
class CLwfTareTask : public base::CStatemachine< eLwfTareSteps>
{
	CWeightControl* m_pWeightCtrl;
	CAlarmManager* m_pAlarm;
	base::LC_PRIORITY m_LCOldPriority;
	base::utils::CWeightPair m_L0;

private:
	void StepRunning();
	void StepError();
	BOOL CheckErrorState(void);

	CLwfTareTask() = delete;

public:
	explicit CLwfTareTask(int32_t id) : CStatemachine(id)
		, m_pWeightCtrl{ nullptr }
		, m_pAlarm{ nullptr }
		, m_LCOldPriority{ base::LC_PRIORITY ::LC_PRIORITY_NORMAL}
		, m_L0{}
	{
	}

	CLwfTareTask(const CLwfTareTask&) = delete;
	CLwfTareTask& operator = (const CLwfTareTask&) = delete;
	virtual ~CLwfTareTask() = default;

	BOOL Stop() override;
	BOOL Start(const uint32_t t) override;
	BOOL Update(const uint32_t t) override;

	void registerWeightCtrl(CWeightControl& weightCtrl);
	void registerAlarmManager(CAlarmManager& rAlarm);

};





