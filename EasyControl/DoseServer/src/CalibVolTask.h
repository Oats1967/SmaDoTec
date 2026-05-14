//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CalibVolTask
///
/// @file   CalibVolTask.h
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

#include "BASE/include/CalibType.h"
#include "BASE/include/Statemachine.h"

enum class eCalibVolSteps : uint32_t
{
	eCalibrationInit = 0,
	eCalibrationRunning,
	eCalibrationError
};


class CDriveControl;
class CAlarmManager;

class CCalibVolTask : public base::CStatemachine< eCalibVolSteps>
{
protected:
	CDriveControl*  m_pDriveCtrl;
	CAlarmManager*  m_pAlarm;

	base::calib::CalibType   m_CalibCfg;
	BOOL			  m_bRunAll;
	uint32_t		  m_StartTime;
	uint32_t		  m_LastTime;
	int32_t			  m_CalibIndex;

private:
	void StepRunning();
	void StepError();
	void SetDrivecommand();
	BOOL CheckErrorState(void);

	CCalibVolTask() = delete;

public:
	explicit CCalibVolTask(const int32_t index) : CStatemachine(index)
		, m_pDriveCtrl{ nullptr }
		, m_pAlarm{ nullptr }
		, m_CalibCfg{ 0 }
		, m_bRunAll(FALSE)
		, m_StartTime{ 0U }
		, m_LastTime{ 0U }
		, m_CalibIndex{ 0 }
	{
	}

	CCalibVolTask(const CCalibVolTask&) = delete;
	CCalibVolTask& operator = (const CCalibVolTask&) = delete;
	virtual ~CCalibVolTask() = default;

	BOOL Start(const uint32_t t) override;
	BOOL Stop() override;
	BOOL Update(const uint32_t t) override;

	void registerDriveCtrl(CDriveControl& encoderCtrl);
	void registerAlarmManager(CAlarmManager& rAlarm);
};
