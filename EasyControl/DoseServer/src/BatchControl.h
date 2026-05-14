//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module BatchControl
///
/// @file   BatchControl.h
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

#include "BaseControl.h"
#include "DriveControl.h"
#include "MeterControl.h"
#include "BatchTimeAlarmControl.h"


class CRefillControl;
class CAgitatorControl;

class CBatchControl : public CBaseControl
{
	enum class eControlSteps : uint32_t
	{
		eIdle = 0,
		ePrepareStartup,
		eWaitBatchRefill,
		eWaitBatchRefillDelay,
		eWBatchGrossStream,
		eBatchFineStream,
	};


protected:
	CDriveControl						m_DriveCtrl;
	CMeterControl						m_MeterCtrl;
	CBatchTimeAlarmControl				m_BatchTimeAlarmCtrl;
	CRefillControl*						m_pRefillCtrl;
	CAgitatorControl*					m_pAgitatorCtrl;

	float32_t		    m_fLastDriveCommand;
	eControlSteps		m_ControlSteps;
#if _DEBUG
	eControlSteps	m_LastControlSteps;
#endif
    uint32_t	     m_tNext;
    uint32_t         m_tWeightNext;

protected:
	//------------------------------------------------------------------------------------
	///  @brief   InitWaitStartupDelay
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void InitPrepareStartup( void);

	void InitWaitBatchRefill(void);
	void InitStartBatch(void);
	void InitBatchGrossStreamControl(void);
	void InitBatchFineStreamControl(void);
	void InitBatchRefillDelay(void);
	void InitBatchGrossStreamDriveCommand(void);
	void UpdateBatchGrossStreamDriveCommand(void);
	void InitBatchFineStreamDriveCommand(void);
	void UpdateBatchFineStreamDriveCommand(void);
	void InitFinishBatch(void);

	void StepWaitBatchRefill			(void);
	void StepBatchGrossStreamControl(void);
	void StepBatchFineStreamControl(void);
	void StepBatchRefillDelay(void);
	void StepPrepareStartup(void);

	void FinishPrepareStartup(void);
	void FinishBatchRefillDelay(void);
	void RefillUpdate( void);
	void AgitatorUpdate(void);

protected:
	virtual void StartBatch() {}
	virtual void ContinueBatch() {}

	virtual void UpdateSwitchPoint();
	virtual BOOL IsSwitchPointReached() const
	{ return FALSE;	}
	virtual void CalcSwitchPoint() {};
	virtual float32_t UpdateTotalizer()
	{ return 0.0F;	}
	virtual BOOL UpdateWeight(void)
	{	return FALSE;	}

	virtual BOOL IsReturnGrossStream() const
	{ return FALSE;	}

	virtual BOOL IsBatchFinished(void) const
	{return FALSE;	}

protected:
	void ClearAlarm(const base::eAlarmError lError) override;

	//------------------------------------------------------------------------------------
	///  @brief   SetDriveCommand
	///
	///           This method ....
	///
	///  @param[in] f , float32_t
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetDriveCommand(const float32_t value);


protected:
	virtual float32_t CalcDriveCommand(const float32_t ) const
	{ return 0.0F;	}

	virtual void Control(void);

	BOOL GetValidSetpoint(void) override;
	BOOL GetDoseSetpoint(void) override;

	virtual void registerRefillControl(CRefillControl* pRefillCtrl);
	virtual void registerAgitatorControl(CAgitatorControl* pAgitatorCtrl);

public:
	explicit CBatchControl(int32_t s);

	CBatchControl() = delete;
	~CBatchControl(void) override = default;

	BOOL Execute	 ( void ) override;
	BOOL InitExecute ( void ) override;
	BOOL ExitExecute ( void ) override;
	BOOL Start		 ( const uint32_t t = 0) override;
	BOOL Stop		 ( void )  override;
};



