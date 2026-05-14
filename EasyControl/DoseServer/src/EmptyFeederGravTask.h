//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EmptyFeederGravTask
///
/// @file   EmptyFeederGravTask.h
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
#include "BASE/Utils/public/FloatRingBuffer.h"
#include "BASE/include/Statemachine.h"
#include "AdsEmptyFeederTaskControl.h"


enum class eEmptyFeederSteps : uint32_t
{
	eEmptyFeederInit = 0,
	eEmptyFeederRunning,
	eEmptyFeederError
};

class CDriveControl;
class CWeightControl;
class CAlarmManager;

class CEmptyFeederGravTask : public base::CStatemachine< eEmptyFeederSteps>
{
	using CBaseClass = base::CStatemachine< eEmptyFeederSteps>;

private:
	CAdsEmptyFeederTaskControl	m_AdsClient;
	base::utils::CFloatRingBuffer<10> m_RingBuffer;
	CAlarmManager*				m_pAlarm;
	CDriveControl*				m_pDriveCtrl;
	CWeightControl*				m_pWeightCtrl;

	base::utils::CWeightPair m_LastWeight;
	uint32_t		  m_LastTime;
	uint32_t		  m_Counter;
	uint32_t		  m_AlarmDriveCommandHigh;
	uint32_t		  m_ElapsedTime;
	BOOL			  m_RefillReleaseState;
	base::LC_PRIORITY m_LCOldPriority;


private:
	void StepRunning();
	void StepError();

	BOOL CheckErrorState(void);
	void CheckDrivecommand();
	void CheckRefillReleaseState() const;
	void StoreRefillReleaseState();
	void RestoreRefillReleaseState() const;
	void StoreLCPriority();
	void RestoreLCPriority() const;
	void InitDrivecommand();
	void InitWeight();
	void SetState(const BOOL bState);
	void StoreAlarms();
	void RestoreAlarms() const;

	CEmptyFeederGravTask() = delete;

public:
	explicit CEmptyFeederGravTask(const int32_t index) : CBaseClass(index)
		, m_AdsClient{ index }
		, m_pDriveCtrl{ nullptr }
		, m_pWeightCtrl{ nullptr }
		, m_pAlarm{ nullptr }
		, m_LastWeight{}
		, m_LastTime{ 0L }
		, m_Counter { 0 }
		, m_AlarmDriveCommandHigh{ 0 }
		, m_ElapsedTime{ 0 }
		, m_RefillReleaseState { FALSE }
		, m_LCOldPriority{ base::LC_PRIORITY::LC_PRIORITY_NORMAL }
	{
	}

	CEmptyFeederGravTask(const CEmptyFeederGravTask&) = delete;
	CEmptyFeederGravTask& operator = (const CEmptyFeederGravTask&) = delete;
	virtual ~CEmptyFeederGravTask() = default;

	BOOL Start(const uint32_t t) override;
	BOOL Stop() override;
	BOOL Update(const uint32_t t) override;
	BOOL InitExecute() override;
	BOOL ExitExecute() override;

	void registerDriveCtrl(CDriveControl& encoderCtrl);
	void registerWeightCtrl(CWeightControl& weightCtrl);
	void registerAlarmManager(CAlarmManager& rAlarm);
};
