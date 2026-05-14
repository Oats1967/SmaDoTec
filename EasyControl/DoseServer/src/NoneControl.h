//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module NoneControl.h
///
/// @file   NoneControl.h
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

class CNoneControl
{
private:
	int32_t m_sID;

private:
	BOOL InitStart					 ( void );

public:
	CNoneControl(int32_t s = 0) : m_sID(s)
	{}


	//------------------------------------------------------------------------------------
	///  @brief   InitExecute
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL InitExecute(void)
	{ return TRUE;	}

	BOOL ExitExecute(void)
	{
		return TRUE;
	}

	BOOL Execute(void)
	{
		return TRUE;
	}

	void SetID(int16_t s)
	{ m_sID = s;  }

};



