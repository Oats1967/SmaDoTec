//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EmptyFeederVolTask
///
/// @file   EmptyFeederVolTask.h
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

#include "BASE/include/Statemachine.h"
#include "AdsEmptyFeederTaskControl.h"



enum class eEmptyFeederVolSteps : uint32_t
{
	eEmptyFeederInit = 0,
	eEmptyFeederRunning,
	eEmptyFeederError
};

class CDriveControl;
class CAlarmManager;

class CEmptyFeederVolTask : public base::CStatemachine< eEmptyFeederVolSteps>
{
	using CBaseClass = base::CStatemachine< eEmptyFeederVolSteps>;

private:
	CAdsEmptyFeederTaskControl	m_AdsClient;
	CAlarmManager*				m_pAlarm;
	CDriveControl*				m_pDriveCtrl;

	uint32_t		  m_LastTime;
	uint32_t		  m_Counter;
	uint32_t		  m_AlarmDriveCommandHigh;
	uint32_t		  m_ElapsedTime;
	BOOL			  m_RefillReleaseState;


private:
	void StepRunning();
	void StepError();

	BOOL CheckErrorState(void);
	void CheckDrivecommand();
	void CheckRefillReleaseState() const;
	void StoreRefillReleaseState();
	void RestoreRefillReleaseState() const;
	void InitDrivecommand();
	void SetState(const BOOL bState);
	void StoreAlarms();
	void RestoreAlarms() const;

	CEmptyFeederVolTask() = delete;

public:
	explicit CEmptyFeederVolTask(const int32_t index) : CBaseClass(index)
		, m_AdsClient{ index }
		, m_pDriveCtrl{ nullptr }
		, m_pAlarm{ nullptr }
		, m_LastTime{ 0L }
		, m_Counter { 0 }
		, m_AlarmDriveCommandHigh{ 0 }
		, m_ElapsedTime{ 0 }
		, m_RefillReleaseState{ FALSE }
	{
	}

	CEmptyFeederVolTask(const CEmptyFeederVolTask&) = delete;
	CEmptyFeederVolTask& operator = (const CEmptyFeederVolTask&) = delete;
	virtual ~CEmptyFeederVolTask() = default;

	BOOL Start(const uint32_t t) override;
	BOOL Stop() override;
	BOOL Update(const uint32_t t) override;
	BOOL InitExecute() override;
	BOOL ExitExecute() override;

	void registerDriveCtrl(CDriveControl& encoderCtrl);
	void registerAlarmManager(CAlarmManager& rAlarm);
};
