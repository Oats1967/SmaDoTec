//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CalibGravTask
///
/// @file   CalibGravTask.h
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
#include "BASE/include/CalibType.h"
#include "BASE/include/Statemachine.h"

enum class eCalibGravSteps : uint32_t
{
	eCalibrationInit = 0,
	eCalibrationRunning,
	eCalibrationError
};


class CDriveControl;
class CWeightControl;
class CAlarmManager;

class CCalibGravTask : public base::CStatemachine< eCalibGravSteps>
{
protected:
	CDriveControl*  m_pDriveCtrl;
	CWeightControl* m_pWeightCtrl;
	CAlarmManager*  m_pAlarm;

	base::calib::CalibType   m_CalibCfg;
	base::utils::CWeightPair m_aStartWeight;
	BOOL			  m_bRunAll;
	uint32_t		  m_StartTime;
	uint32_t		  m_LastTime;
	int32_t			  m_CalibIndex;
	base::LC_PRIORITY m_LCOldPriority;

private:
	void StepRunning();
	void StepError();
	void SetDrivecommand();

	BOOL CheckErrorState(void);

	CCalibGravTask() = delete;

public:
	explicit CCalibGravTask(const int32_t index) : CStatemachine(index)
		, m_pDriveCtrl{ nullptr }
		, m_pWeightCtrl{ nullptr }
		, m_pAlarm{ nullptr }
		, m_CalibCfg{ 0 }
		, m_aStartWeight()
		, m_bRunAll(FALSE)
		, m_StartTime{ 0U }
		, m_LastTime{ 0U }
		, m_CalibIndex{ 0 }
		, m_LCOldPriority { base::LC_PRIORITY::LC_PRIORITY_NORMAL }
	{
	}

	CCalibGravTask(const CCalibGravTask&) = delete;
	CCalibGravTask& operator = (const CCalibGravTask&) = delete;
	virtual ~CCalibGravTask() = default;

	BOOL Start(const uint32_t t) override;
	BOOL Stop() override;
	BOOL Update(const uint32_t t) override;

	void registerDriveCtrl(CDriveControl& encoderCtrl);
	void registerWeightCtrl(CWeightControl& weightCtrl);
	void registerAlarmManager(CAlarmManager& rAlarm);
};
