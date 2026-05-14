//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module VolControl.h
///
/// @file   VolControl.h
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

#include "BASE/types.h"
#include "BASE/Utils/public/Timer.h"
#include "BaseControl.h"
#include "DriveControl.h"
#include "MeterControl.h"
#include "CalibVolTask.h"
#include "RefillControlVol.h"
#include "EmptyFeederVolTask.h"
#include "AdsMassflowControl.h"


class CVolControl : public CBaseControl
{
	CAdsMassflowControl	m_AdsClient;
	CRefillControlVol	m_RefillCtrl;
	CMeterControl		m_MeterCtrl;
	CDriveControl		m_DriveCtrl;
	CCalibVolTask		m_CalibTask;
	CEmptyFeederVolTask	m_EmptyFeederTask;

	float32_t			m_fActualSetpoint;
	float32_t			m_fMassflow;
	uint32_t			m_tRegelNext;
	int32_t				m_ControlSteps;
	BOOL				m_bSetpointChanged;
	BOOL				m_bDosePerformanceChanged;


private:
	BOOL GetDosePerformance(void);
	void SetActualDosePerformance(const float32_t fQMaxfaktor);
	float32_t CalcDriveCommand(const float32_t fNomwert) const;
	void DosefactorSet(const float32_t fNomwert);
	BOOL GetVolumetric			( void ) override;
	void GetMassflow					( void );
	void SetMassflow				( const float32_t );
	void DoseDriveCommandSet( const float32_t fNomwert );
	void SetDriveCommand			( const float32_t f);
	void Control(void);
	void TotalizerSet(const float32_t fMassflow);

	void Phase0						( void );
	void Phase1						( void );
	void Phase2						( void );

	CVolControl() = delete;

protected:
	BOOL GetDoseSetpoint(void) override;	// TRUE ,wenn geandert sonst FALSE

public:
	CVolControl(int32_t s = 0);
	~CVolControl(void) override = default;

	BOOL Execute	 ( void ) override;
	BOOL InitExecute ( void ) override;
	BOOL ExitExecute ( void ) override;
	BOOL Stop		 ( void ) override;
	BOOL Start		 ( const uint32_t t = 0) override;
};



