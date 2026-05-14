//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module BaseControl.h
///
/// @file   BaseControl.h
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
#include "AlarmManager.h"
#include "AdsBaseControl.h"
#include "BASE/Utils/public/Totalizer.h"



enum class eOperatingMode : uint32_t
{
	IDLE = 0,
	CALIBRATING,
	TARING,
	EMPTYFEEDER,
	RUNNING
};


class CBaseControl :  public base::CTimerStatemachine<eOperatingMode>
{
protected:
	CAlarmManager			m_aAlarm;
	CAdsBaseControl			m_AdsClient;
	base::utils::CTotalizer m_aTotalizer;
	BOOL					m_bDoseIOStartInputLast;
	BOOL					m_bLineStartedLast;
	BOOL					m_bEmptyFeederIOStartInputLast;
	BOOL					m_bEmptyFeederIOStopInputLast;
	BOOL					m_bIOVolumetricLast;

private:
	void GetLineAlarms	(void);
	BOOL GetPBStop			(void) const;
	BOOL GetPBStart			(void) const;

protected:

	virtual BOOL Init(void);
	virtual BOOL GetRelease(void);
	virtual BOOL GetStart(void);
	virtual BOOL GetValidSetpoint(void);


	virtual BOOL GetClearTotalizer ( void );
	virtual BOOL GetAnlagenModus ( void );
	virtual BOOL GetLineSetpoint ( void ) const;
	virtual BOOL GetDoseSetpoint ( void );
	virtual BOOL GetVolumetric	( void );
	virtual BOOL GetTaring ( void ) const;
	virtual BOOL GetEmptyFeeder (void);
	virtual BOOL GetCalibration	( void ) const;
    virtual void ClearAlarm ( const base::eAlarmError lError);
	virtual void ClearTotalizer ( void );
	virtual BOOL GetClearAlarm	( void );
	virtual void SetAlarmOutput (void);
	virtual void CheckExtAlarm (void);


public:
	CBaseControl ( int32_t s );
	~CBaseControl(void) override = default;

    BOOL InitExecute    ( void ) override;
	BOOL ExitExecute    ( void ) override;
	BOOL Execute		( void )   override;
	BOOL Start			( const uint32_t t = 0) override;
	BOOL Stop			( void ) override;
};




//--------------------------------------------------------------------------------------------
inline void CBaseControl :: ClearAlarm   (const base::eAlarmError lError)
{
	if (lError == base::eAlarmError::ERROR_MAX)
	{
		m_aAlarm.ClearAllAlarms();
	}
	else
	{
		m_aAlarm.ClearAlarm(lError);
	}

}



