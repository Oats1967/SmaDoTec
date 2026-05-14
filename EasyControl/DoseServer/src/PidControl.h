//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module PidControl.h
///
/// @file   PidControl.h
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
#include "MassflowAlarmControl.h"
#include "MeterControl.h"
#include "BASE/math/public/PidControl.h"
#include "AdsPidControl.h"


enum class ePidPropGain : uint32_t 
{ 
	PidPropRough = 0, 
	PidPropFine 
};

class CPidControl : public CBaseControl
{
	CAdsPidControl			 m_AdsClient;
	base::math::CPidControl	 m_PidControl;
	CDriveControl			 m_DriveCtrl;
	CMassflowAlarmControl	 m_MassflowAlarmCtrl;
	CMeterControl			 m_MeterCtrl;

	BOOL				m_bStartupDelay;
	uint32_t			m_lSampleInterval;
	float32_t			m_fLastLeistung;
	float32_t			m_fLastDriveCommand;
	BOOL				m_bLeistungChanged;
	BOOL				m_bMaxSetpointChanged;
	BOOL				m_bDosePerformanceChanged;
	BOOL				m_bSteilheitIswertChanged;
	BOOL				m_bPidPropGainFineChanged;
	BOOL				m_bPropGrossGainChanged;
	BOOL				m_bPidPropGainSwitchGrossFineChanged;
	BOOL				m_bPidIntegralGainChanged;
	BOOL				m_bInputFilterChanged;
	BOOL				m_bSampleIntervalChanged;

	float32_t				m_fMaxSetpoint;
	float32_t				m_fActualDoseperformance;
	float32_t				m_fMassflowFilter;
	float32_t				m_fSteepnessMassflow;
	float32_t				m_fMassflow;
	float32_t				m_fActualSetpoint;
	float32_t				m_fPidPropGainFine;
	float32_t				m_fPidPropGainGross;
	float32_t				m_fPidPropGainSwitchGrossFine;
	float32_t				m_fPidIntegralGain;
	float32_t				m_fInputFilter;

	ePidPropGain			m_ePidPropSwitch;
	uint32_t				m_tRegelNext;
	uint32_t				m_tRampenStart;
	int32_t					m_ControlSteps;


private:
	BOOL GetMaxSetpoint			 ( void );	// TRUE ,wenn geandert sonst FALSE
	BOOL GetSteepnessMassflow	 ( void );	// TRUE ,wenn geandert sonst FALSE
	BOOL GetPidPropGainFine ( void );	// TRUE ,wenn geandert sonst FALSE
	BOOL GetPidPropGainGross ( void );	// TRUE ,wenn geandert sonst FALSE
	BOOL GetPidPropGainSwitchGrossFine	 ( void );	// TRUE ,wenn geandert sonst FALSE
	BOOL GetIntegralVerstaerkung ( void );	// TRUE ,wenn geandert sonst FALSE
	BOOL GetInputFilter		 ( void );	// TRUE ,wenn geandert sonst FALSE
	BOOL GetDosePerformance		 ( void );	// TRUE ,wenn geandert sonst FALSE
	void TotalizerSet(const float32_t fMassflow);
	//------------------------------------------------------------------------------------
	///  @brief   GetSampleInterval
	///
	///           This method ....
	///
	///
	///  @class        CLwfControl
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL GetSampleInterval		 ( void );	// TRUE ,wenn geandert sonst FALSE
  
	void GetMassflow				 ( void );
	void SetDriveCommand		( const float32_t f);
    void SetActualDosePerformance        (const float32_t fQMaxfaktor);


	ePidPropGain			CheckReglerKonstanten ();

	void AlarmMonitoring			( void );
	void CheckVolGrav			( void );
	void CheckRampen			( void );
	void DosiererVolSet			( void );
	void DosiererSet			( float32_t fNomwert );
	void StartVol				( void );
	void StartGrav				( void );
	void StartPID				( void );
	void StartRampe				( void);
	void Phase0					( void );
	void Phase1					( void );
	void Phase2					( void );
	void Phase3					( void );
	void Phase4					( void );
	void Phase5					( void );
	void Phase6					( void );
	void Control				( void );

	void InitStart				( void);
	void Init1					( void);
	void Init2					( void);
	void Init3					( void);
	void Init4					( void);
	void Init5					( void);
	void Init6					( void);

	void UpdateMassflowFilter	(void);
	void SetMassflow			(const float32_t fAktLeistung);

protected:
	BOOL GetDoseSetpoint(void) override;	// TRUE ,wenn geandert sonst FALSE


public:
	CPidControl(int32_t s = 0) : CBaseControl(s)
								, m_AdsClient(s)
								,m_DriveCtrl(s)
								,m_MassflowAlarmCtrl(s)
								,m_MeterCtrl(s)
								,m_tRampenStart(0L)
								, m_ControlSteps{ 0 }
								, m_fActualSetpoint(-1.0f)
								,m_bStartupDelay(FALSE)
								,m_tRegelNext(0L)
								,m_fLastLeistung(0.0f)
								,m_fLastDriveCommand(0.0f)
								,m_bLeistungChanged(FALSE)
								,m_bDosePerformanceChanged(FALSE)
								,m_bMaxSetpointChanged(FALSE)
								,m_fMaxSetpoint(0.0f)
								,m_fActualDoseperformance(0.0f)
								,m_fMassflowFilter(0.0f)
								,m_fSteepnessMassflow(0.0f)
								,m_fMassflow(0.0f)
								,m_bSteilheitIswertChanged(FALSE)
								,m_bPidPropGainFineChanged(FALSE)
								,m_bPropGrossGainChanged(FALSE)
								,m_bPidPropGainSwitchGrossFineChanged(FALSE)
								,m_bPidIntegralGainChanged(FALSE)
								,m_bSampleIntervalChanged(FALSE)
								,m_bInputFilterChanged(FALSE)
								,m_fPidPropGainFine(0.0f)
								,m_fPidPropGainGross(0.0f)
								,m_fPidPropGainSwitchGrossFine(0.0f)
								,m_fPidIntegralGain(0.0f)
								,m_fInputFilter(0.0f)
								,m_ePidPropSwitch(ePidPropGain::PidPropRough)
								,m_lSampleInterval(0L)
	{
		m_DriveCtrl.registerAlarmManager(this->m_aAlarm);
		m_MassflowAlarmCtrl.registerAlarmManager(this->m_aAlarm);
	}

	~CPidControl(void) {}

	BOOL Execute	 ( void ) override;
	BOOL InitExecute ( void ) override;
	BOOL ExitExecute ( void ) override;
	BOOL Stop		 ( void ) override;
	BOOL Start		 (const uint32_t t = 0) override;
};




