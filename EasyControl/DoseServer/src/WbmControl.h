//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WbmControl.h
///
/// @file   WbmControl.h
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
#include "BASE/Utils//public/FloatRingBuffer.h"
#include "AdsWbfControl.h"


class CWbmControl : public CBaseControl
{
	static constexpr uint32_t MAXTARACOUNT = 100U;

	loadcell::ILCModuleInterface& m_LoadCell;
	CAdsWbfControl		m_AdsClient;
	CDriveControl		m_DriveCtrl;
	CWeightControl		m_WeightCtrl;
	base::utils::CFloatRingBuffer< MAXTARACOUNT > m_fTaraWeightFifo;

	float32_t		m_fWeight;
	float32_t		m_fBandV;
	BOOL			m_bStartupDelay;
	BOOL			m_bSampleIntervalChanged;
	BOOL			m_bInputFilterChanged;
	float32_t		m_fBeltLoadSetpoint;
	float32_t		m_fMassflowFilter;
	float32_t		m_fBeltLoadFilter;
	float32_t		m_fMassflow;
    float32_t       m_fInputFilter;
	float32_t		m_fBandlast;
	uint32_t		m_tMassflowNext;
	int32_t			m_lMassflowIntervall;
    uint32_t        m_lSampleInterval;
    uint32_t        m_tTaraStop;
	int32_t			m_ControlSteps;



	void StartVol					( void);
	void SetMassflow				( float32_t );
    void TotalizerSet				(const float32_t );


	void InitStart						( void );
	void Init1						( void );
	void Init2						( void );
	void Phase0						( void );
	void Phase1						( void );
	void Phase2						( void );

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
	BOOL GetSampleInterval         ( void );
    void GetPidSampleInterval      ( void );

	float32_t GetVMax					(void);
	void InitMassflowFilter			(void);
	void InitBeltLoadFilter			(void);
	void UpdateMassflowFilter		(void);
	void UpdateBeltLoadFilter		(void);

	void AlarmMonitoring			(void);
	void SetDriveCommand			(const float32_t value);
	void BeltLoadMonitoring			(void);

	BOOL TareStart				( void);
    BOOL TareStop              ( void);
	BOOL TareRun				( void);
	BOOL Control					( void);
	void GetWeight					( void);
	BOOL GetBeltLoad				( void);

protected:
	BOOL GetRelease(void) override;
	void ClearAlarm(const base::eAlarmError lError) override;
	BOOL Stop						( void);
	BOOL Start						( void);
	BOOL GetAnlagenModus			( void ) const;

public:
	CWbmControl(int32_t s, loadcell::ILCModuleInterface& rlc);
	~CWbmControl(void) override = default;

	BOOL Execute	 ( void ) override;
	BOOL InitExecute ( void ) override;
	BOOL ExitExecute ( void ) override;
};



