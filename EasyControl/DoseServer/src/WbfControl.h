//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WbfControl.h
///
/// @file   WbfControl.h
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

#include <array>
#include "BaseControl.h"
#include "DriveControl.h"
#include "WeightControl.h"
#include "MassflowAlarmControl.h"
#include "WbfTareTask.h"
#include "BASE/Utils/public/Memory.h"
#include "BASE/math/public/PidControl.h"
#include "BASE/Utils/public/WeightPair.h"
#include "AdsWbfControl.h"




class CWbfControl : public CBaseControl
{
	enum class eControlSteps : uint32_t
	{
		eStepStart = 0,
		eStepPrepareWaitDelay,
		eStepPrepareVol,
		eStepVolumetric,
		eStepPrepareGrav,
		eStepGravimetricStart,
		eStepGravimetric,
		eStepForceVolumetric
	};

	loadcell::ILCModuleInterface& m_LoadCell;
	CAdsWbfControl				m_AdsClient;
	base::math::CPidControl		m_PidControl;
	CDriveControl				m_DriveCtrl;
	CWeightControl				m_WeightCtrl;
	CMassflowAlarmControl		m_MassflowAlarmCtrl;
	CWbfTareTask				m_TareCtrl;
	base::utils::CWeightPair    m_aLoadCell0;
	base::utils::CWeightPair    m_aLoadCell1;

	float32_t		m_fWeight;
	float32_t		m_fBandV;
	BOOL			m_bStartupDelay;
	BOOL			m_bLeistungChanged;
	BOOL			m_bBeltLoadSetpointChanged;
	BOOL			m_bInputFilterChanged;
	BOOL			m_bPropGrossGainChanged;
	BOOL			m_bSampleIntervalChanged;
	float32_t		m_fBeltLoadSetpoint;
	float32_t		m_fMassflowFilter;
	float32_t		m_fBeltLoadFilter;
	float32_t		m_fAktLeistung;
	float32_t		m_fMassflow;
	float32_t		m_fActualSetpoint;
	float32_t		m_fLastLeistung;
	float32_t		m_fPidPropGainGross;
	float32_t		m_fInputFilter;
	float32_t		m_fBandlast;
	uint32_t		m_tMassflowNext;
	uint32_t		m_lSampleInterval;
	int32_t			m_lMassflowIntervall;
	eControlSteps	m_ControlSteps;				// Warten


	//------------------------------------------------------------------------------------
	///  @brief   SetDriveCommand
	///
	///           This method ....
	///
	///  @param[in] f , float32_t
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetDriveCommand			(float32_t f);

	//------------------------------------------------------------------------------------
	///  @brief   SetDrehgeber
	///
	///           This method ....
	///
	///  @param[in] fAktBandlast , float32_t
	///  @param[in] fAktLeistung , float32_t
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetDrehgeber				(float32_t , float32_t);

	//------------------------------------------------------------------------------------
	///  @brief   StartGrav
	///
	///           This method starts the gravimetric mode
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void StartGrav					( void);

	//------------------------------------------------------------------------------------
	///  @brief   StartVol
	///
	///           This method starts the volumetric mode
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void StartVol					( void);

	//------------------------------------------------------------------------------------
	///  @brief   InitStart
	///
	///           This method initializes the state machine step 0
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void InitStart			   	( void );

	void InitPrepareWaitDelay		( void );

	//--------------------------------------------------------------------------------
	// voluemtrisch - Phase 2 - 5 Sekunden warten
	//--------------------------------------------------------------------------------
	void InitPrepareVol				( void );
	void InitVolumetric( void );
	//--------------------------------------------------------------------------------
	// gravimetrisch - Phase 4 - Warten auf 5 Sekunden Ueberbrueckung
	//--------------------------------------------------------------------------------
	void InitPrepareGrav( void );
	void InitGravimetricStart( void );
	void InitGravimetric( void );


	//--------------------------------------------------------------------------------
	// volumetrisch  - wegen Minbandlast erreicht
	//--------------------------------------------------------------------------------
	void InitForceVolumetric( void );


	//------------------------------------------------------------------------------------
	///  @brief   StepStart
	///
	///           This method waits until initial time delay has elapsed
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void StepStart					( void );

	//--------------------------------------------------------------------------------
	// volumetrisch - Phase 2 - Warten auf 5 Sekunden Ueberbrueckung
	//--------------------------------------------------------------------------------
	void StepPrepareWaitDelay		( void );
	void StepPrepareVol				( void );
	void StepVolumetric( void );
	//--------------------------------------------------------------------------------
	// gravimetrisch - Phase 4 - Warten auf 5 Sekunden Ueberbrueckung
	//--------------------------------------------------------------------------------
	void StepPrepareGrav( void );
	void StepGravimetricStart( void );
	void StepGravimetric( void );

	//--------------------------------------------------------------------------------
	// volumetrisch  - wegen Minbandlast erreicht
	//--------------------------------------------------------------------------------
	void StepForceVolumetric( void );


	//--------------------------------------------------------------------------------
	// volumetrisch  - wegen Minbandlast erreicht
	//--------------------------------------------------------------------------------
	void StartForceVolumetric     ( void);
    void StartVolumetric          ( void);
    void StartGravimetrisch         ( void);
	//----------------------------------------------------------------------------
	// Leistung starten
	void StartLeistung              ( void );
    void StartLeistungGrav          ( void);

	//--------------------------------------------------------------------------------
	// voluemtrisch - Phase 2 StartLeistungVol
	//--------------------------------------------------------------------------------
	void StartLeistungVol           ( void);


	//----------------------------------------------------------------------------
	// Ueberprueft, ob gravimetrisch oder volumetrisch
	void StartVolGrav               ( void);


	//------------------------------------------------------------------------------------
	///  @brief   GetBeltLoadSetpoint
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL GetBeltLoadSetpoint		( void );

	//------------------------------------------------------------------------------------
	///  @brief   GetPidPropGainGross
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL GetPidPropGainGross	( void );

	//------------------------------------------------------------------------------------
	///  @brief   GetInputFilter
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL GetInputFilter			( void );

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
	BOOL GetSampleInterval			( void );
	//------------------------------------------------------------------------------------
	///  @brief   GetVolumetric
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL GetVolumetric            ( void) override;



	//------------------------------------------------------------------------------------
	///  @brief   GetPidSampleInterval
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void GetPidSampleInterval      ( void );


	//------------------------------------------------------------------------------------
	///  @brief   SetMassflow
	///
	///           This method stores and sets the actual througput to the output
	///
	///  @param[in] fAktLeistung , const float32_t
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetMassflow				(const float32_t );

	void TotalizerSet(const float32_t );



	//------------------------------------------------------------------------------------
	///  @brief   GetVMax
	///
	///           This method returns the velocity of the weight belt feeder
	///
	///  @param[in] none
	///  @return float32_t
	///
	//------------------------------------------------------------------------------------
	float32_t GetVMax				(void) const;

	//------------------------------------------------------------------------------------
	///  @brief   InitMassflowFilter
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void InitMassflowFilter			(void);

	//------------------------------------------------------------------------------------
	///  @brief   InitBeltLoadFilter
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void InitBeltLoadFilter			(void);


	//------------------------------------------------------------------------------------
	///  @brief   UpdateMassflowFilter
	///
	///           This method updates the throughput 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void UpdateMassflowFilter		(void);

	//------------------------------------------------------------------------------------
	///  @brief   UpdateBeltLoadFilter
	///
	///           This method updates the weight belt load filter
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void UpdateBeltLoadFilter		(void);

	//------------------------------------------------------------------------------------
	///  @brief   AlarmMonitoring
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void AlarmMonitoring			(void);


	//------------------------------------------------------------------------------------
	///  @brief   BeltLoadMonitoring
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void BeltLoadMonitoring		(void);

	//------------------------------------------------------------------------------------
	///  @brief   ExtAlarmMonitoring
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void ExtAlarmMonitoring       (void);

	BOOL Control					( void);

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
	///  @brief   GetBeltLoad
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL GetBeltLoad				( void);

protected:
	void ClearAlarm(const base::eAlarmError lError) override;


	//------------------------------------------------------------------------------------
	///  @brief   GetDoseSetpoint
	///
	///           This method return the actual throughput
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL GetDoseSetpoint(void) override;


public:
	CWbfControl(int32_t s, loadcell::ILCModuleInterface& rlc);
	~CWbfControl(void) override = default;

	BOOL Execute	 ( void ) override;
	BOOL InitExecute ( void ) override;
	BOOL ExitExecute ( void ) override;
	BOOL Stop		 ( void ) override;
	BOOL Start		 ( const uint32_t t = 0 ) override;

};



