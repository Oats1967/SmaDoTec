//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LclControl.h
///
/// @file   LclControl.h
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
#include "LwfTareTask.h"
#include "FeederWeightControl.h"



class CLclControl : public CBaseControl
{
	CFeederWeightControl m_WeightCtrl;
	CLwfTareTask		m_TareTask;
	BOOL				 m_bMinReached;
	BOOL				 m_bMaxReached;
	BOOL				 m_bMinReachedLast;
	BOOL				 m_bMaxReachedLast;


private:
	void CheckAlarm();
	BOOL Control(void);


public:
	CLclControl(int32_t s, loadcell::ILCModuleInterface& rlc);
	~CLclControl(void) override = default;

	BOOL Execute	 ( void ) override;
	BOOL InitExecute ( void ) override;
	BOOL ExitExecute ( void ) override;
	BOOL Stop		 ( void ) override
	{ return FALSE;	}
	BOOL Start		 ( const uint32_t) override
	{ return FALSE;	}
};



