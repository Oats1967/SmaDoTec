//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IcpControl
///
/// @file   IcpControl.h
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
#include "BASE/Utils/public/FloatRingBuffer.h"
#include "AdsIcpControl.h"



class CIcpControl : public CBaseControl
{
	static constexpr uint32_t ICP_FILTERSIZE = 10;

	CAdsIcpControl			m_AdsClient;
	CDriveControl			m_DriveCtrl;
	CMassflowAlarmControl	m_MassflowAlarmCtrl;
	base::utils::CFloatRingBuffer<ICP_FILTERSIZE> m_MassflowBuffer;

	float32_t			m_fActualSetpoint;
	float32_t			m_fCurrentMassflow;
	float32_t			m_fNominalMassflow;
	float32_t			m_fMaxSetpoint;
	float32_t			m_fSteepnessMassflow;
	BOOL				m_bMaxSetpointChanged;
	BOOL				m_bSteepnessMassflowChanged;
	BOOL				m_bLeistungChanged;
	uint32_t			m_tRegelNext;
	int32_t				m_lDrehgeberIntervall;
	int32_t				m_ControlSteps;

private:
	BOOL GetMaxSetpoint			( void );
	BOOL GetSteepnessMassflow	( void );

	void GetMassflow				( void );

	void Control				( void );
	void SetMassflow			( const float32_t );
	void TotalizerSet			(const float32_t fMassflow);


	void DosiererSet			( const float32_t fNomwert );
	void SetDriveCommand		( const float32_t f);
	void Phase0					( void );
	void Phase1					( void );
	void Phase2					( void );


	void AlarmMonitoring		  ( void );
	BOOL InitStart				  ( void );

protected:
	BOOL Stop					( void ) override;
	BOOL Start					(const uint32_t t)  override;
	BOOL GetDoseSetpoint		( void) override;;


public:
	CIcpControl(int32_t s = 0);
	~CIcpControl(void) override = default;

	BOOL Execute	 ( void ) override;
	BOOL InitExecute ( void ) override;
	BOOL ExitExecute ( void ) override;
};



