//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwfControl.h
///
/// @file   LwfControl.h
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

#include <map>
#include "BaseControl.h"
#include "RefillControlGrav.h"
#include "DriveControl.h"
#include "AgitatorControl.h"
#include "FeederWeightControl.h"
#include "CalibGravTask.h"
#include "EmptyFeederGravTask.h"
#include "LwfTareTask.h"
#include "MeterControl.h"
#include "MassflowAlarmControl.h"
#include "DosePerformanceAlarmControl.h"
#include "NoiseAlarmControl.h"
#include "AdsMassflowControl.h"
#include "BASE/Utils/public/GatefilterCalculator.h"
#include "BASE/Utils/public/MeasuredBuffer.h"
#include "BASE/math/public/PidControl.h"
#include "BASE/Utils/public/Timer.h"


class CLwfControl : public CBaseControl
{
	using CBaseClass = CBaseControl;

	static constexpr uint32_t LWF_MEASUREBUFFERSIZE = 8U;

	using MassflowControl = base::utils::CMeasuredBuffer<LWF_MEASUREBUFFERSIZE>;

	enum class eControlSteps : uint32_t
	{
		eIdle = 0,
		ePrepareStartupDelay,
		eWaitStartupDelay,
		eVolControl,
		ePrepareDosing,
		eCalcDosefactor,
		eWaitStableMassflow,
		eGravControl,
		eWaitRefillFinished,
		eWaitRefillStableMassflow,
		eWaitFaultStableMassflow,
		eForceVolumetric,
		eMax,
	};


protected:
	typedef void (CLwfControl::* MFP)();
	const std::map <eControlSteps, MFP> c_EditMap;

	base::math::CPidControl				m_PidControl;
	CDriveControl						m_DriveCtrl;
	CAgitatorControl					m_AgitatorControl;
	CFeederWeightControl				m_WeightCtrl;
	CMassflowAlarmControl				m_MassflowAlarmCtrl;
	CDosePerformanceAlarmControl		m_DosePerformanceAlarmCtrl;
	CNoiseAlarmControl					m_NoiseAlarmCtrl;
	CCalibGravTask						m_CalibTask;
	CLwfTareTask						m_TareTask;
	CEmptyFeederGravTask				m_EmptyFeederTask;
	CMeterControl						m_MeterCtrl;
	base::utils::CGatefilterCalculator	m_aPlausibility;
	CRefillControlGrav					m_RefillCtrl;
	CAdsMassflowControl					m_AdsClient;
	MassflowControl						m_MassflowCtrl;
	base::utils::CWeightPair			m_aLoadCell0;
	base::utils::CWeightPair			m_aLoadCell1;

	float32_t	        m_fActualSetpoint;		// Aktueller Sollwert
	float32_t	        m_fMassflow;			// Aktuelle angezeigte Leistung und Verbrauch
	float32_t	        m_fCurrentMassflow;		// Aktuell berechnete Leistung
	float32_t	        m_fMassflowGefiltert;
    float32_t           m_fMassflowGleitend;
	float32_t	        m_fDosePerformance;
	float32_t	        m_fPidPropGainGross;
    float32_t           m_fPidGatefilter;
    float32_t           m_fMassflowFilter;
    float32_t           m_fLastError;


    int32_t            m_lControlCount;
    int32_t            m_lPlausibilityRetryCounter;
	float64_t		   m_dLastDriveCommand;
	float64_t		   m_dActualDoseperformance;

	BOOL			m_bLoadCellInitialized;
	BOOL	        m_bDosePerformanceChanged;
	BOOL	        m_bSetpointChanged;
	BOOL	        m_bPropGrossGainChanged;
	eControlSteps	m_ControlSteps;
#if _DEBUG
	eControlSteps	m_LastControlSteps;
	BOOL            m_bDosefaktorInitialized;
#endif
    BOOL            m_bGatefilterModified;
    BOOL            m_bDoseFaktorOK;

	uint32_t	     m_zt0;
    uint32_t	     m_tNext;
    uint32_t         m_tWeightNext;
	uint32_t	     m_tSampleInterval;

	float32_t		 m_fMinDiff;
	int32_t			 m_lMaxCount;
	BOOL			 m_bGateFilterHit;


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
	void InitPrepareStartupDelay( void);

	//------------------------------------------------------------------------------------
	///  @brief   InitWaitStartupDelay
	///
	///           This method waits until initial parameter delay time has elapsed and jumps to next step
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void InitWaitStartupDelay		( void);
	void InitVolControl				(void);
	void InitPrepareDosing			( void);
	void InitCalcDosefactor			( void);
	void InitWaitStableMassflow	( void);
	void InitGravControl			( void);
	void InitWaitRefillFinished		( void);
	void InitWaitRefillStableMassflow( void);
	void InitWaitFaultStableMassflow( void);
	void InitForceVolumetric( void);
	BOOL CheckGravTransition(void);
	BOOL CheckVolTransition(void);

	BOOL CheckTransition(void);

	//------------------------------------------------------------------------------------
	///  @brief   StepWaitStartupDelay
	///
	///           This method waits until initial system delay time has elapsed and jumps to next step
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void StepIdle(void) {}
	void StepPrepareStartupDelay			( void);
	void StepWaitStartupDelay				( void);

	void StepVolControl						( void);
	void StepPrepareDosing					( void);
	void StepCalcDosefactor					( void);
	void StepWaitStableMassflow			( void);
	void StepGravControl					( void);
	void StepWaitRefillFinished				( void);
	void StepWaitRefillStableMassflow	( void);
	void StepWaitFaultStableMassflow		( void);
	void StepForceVolumetric				( void);
    
    void StartWithSetpoint			( void );
    void StartVolumetric			( void );
	void FinishVolumetric			( void );
	void StartDosing				( void );
    void StartVolDosierung          ( void );
    void StartGravDosierung         ( void );
    void StartFault					( void );
    void FinishFault                ( void );
    void StartRefill				( void );
    void FinishRefill				( void );
    void StartForceVolumetric       ( void );
    void FinishForceVolumetric		( void );
	BOOL GetAgitator				( void );
	void StartRefillStableMassflow	(void);
	void FinishRefillStableMassflow(void);


	void CalcDosefactorHandleSmallError(void);
	void CalcDosefactorHandleLargeError(void);

	//------------------------------------------------------------------------------------
	///  @brief   GetDoseSetpoint
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL GetDoseSetpoint( void ) override;

	//------------------------------------------------------------------------------------
	///  @brief   GetDosePerformance
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL GetDosePerformance		( void );

	//------------------------------------------------------------------------------------
	///  @brief   VerifySampleInterval
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL VerifySampleInterval				( const uint32_t );
	void InitSampleInterval					(void);
	BOOL GetChangedSampleInterval(void);

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

	//------------------------------------------------------------------------------------
	///  @brief   GetPidPropGainGross
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL GetPidPropGainGross( void );

	//------------------------------------------------------------------------------------
	///  @brief   GetGatefilter
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL GetGatefilter			( void);

	//------------------------------------------------------------------------------------
	///  @brief   GetMassflowfilter
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL GetMassflowfilter       ( void);

	//------------------------------------------------------------------------------------
	///  @brief   SetActualDosePerformance
	///
	///           This method ....
	///
	///  @param[in] fQMaxfaktor , const float32_t
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetActualDosePerformance           (const float32_t fQMaxfaktor);

	//------------------------------------------------------------------------------------
	///  @brief   SetMassflow
	///
	///           This method ....
	///
	///  @param[in] fAktLeistung , const float32_t
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetMassflow				(const float32_t fAktLeistung);

	//------------------------------------------------------------------------------------
	///  @brief   GetWeight
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void GetWeight					( void);

	//------------------------------------------------------------------------------------
	///  @brief   DoseDriveCommandSet
	///
	///           This method ....
	///
	///  @param[in] fNomwert , const float32_t
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void DoseDriveCommandSet  ( const float32_t );
	void DosefactorSet( const float32_t );
	void TotalizerSet (const float32_t fMassflow);

 
    void InitLeistung                       ( void );
	//------------------------------------------------------------------------------------
	///  @brief   MassflowInit()
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void MassflowInit                        (void);
	//------------------------------------------------------------------------------------
	///  @brief   UpdateMassflow
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL UpdateMassflow                      (void);

	//------------------------------------------------------------------------------------
	///  @brief   UpdateWeightAndMassflow
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL UpdateWeightAndMassflow(void);

	BOOL UpdateWeightAndMassflowAndPlausibility(void);

	//------------------------------------------------------------------------------------
	///  @brief   InitWeight
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void InitWeight                         (void);

	//------------------------------------------------------------------------------------
	///  @brief   UpdateWeight
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL UpdateWeight                       (void);

	//------------------------------------------------------------------------------------
	///  @brief   InitPlausibilityCheck
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void InitPlausibilityCheck		(void);

	//------------------------------------------------------------------------------------
	///  @brief   UpdatePlausibilityCheck
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	void UpdatePlausibilityCheck		(void);


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
	void SetDeltaDriveCommand(const float32_t delta);

protected:
	virtual float32_t CalcDriveCommand(const float32_t ) const
	{ return 0.0F;	}

	virtual void Control(void);

	BOOL GetVolumetric(void)  override;

public:
	CLwfControl(int32_t s, loadcell::ILCModuleInterface& rlc);
	~CLwfControl(void) override = default;

	BOOL Execute	 ( void ) override;
	BOOL InitExecute ( void ) override;
	BOOL ExitExecute ( void ) override;
	BOOL Start		 ( const uint32_t t = 0) override;
	BOOL Stop		 ( void )  override;
};



