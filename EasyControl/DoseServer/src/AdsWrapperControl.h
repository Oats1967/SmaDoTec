//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsWrapperControl
///
/// @file   AdsWrapperControl.h
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

#include "BASE/Base.h"

class CAdsWrapperControl
{
protected:
	int32_t			m_id;
	BOOL			m_bInit;

public:
	CAdsWrapperControl(const int32_t index) : m_id{ index }
											, m_bInit{ FALSE }
	{}

	CAdsWrapperControl() : m_id{ 0 }
						, m_bInit{ FALSE }
	{}

	virtual void Init();
	virtual void Exit()
	{	m_bInit = FALSE;	}

	virtual BOOL UpdateState() { return FALSE; }

	BOOL IsInit() const
	{	return m_bInit;	}

	//*********************************************************************************************
	//*********************************************************************************************
	BOOL AdsGetSyncError(BOOL& ) const;
};
