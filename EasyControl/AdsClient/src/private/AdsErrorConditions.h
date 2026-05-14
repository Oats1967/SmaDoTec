//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CAdsErrorConditions
///
/// @file   AdsErrorConditions.h
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

#include "BASE/Base.def"

class CAdsErrorConditions
{
protected:
	BOOL	m_SyncError;
	BOOL	m_LineBlock;
	BOOL	m_Open;
	BOOL	m_ComState;

public:
	int32_t CheckConditions() const;
	int32_t CheckLineConditions() const;

	SETGET(BOOL, SyncError)
	SETGET(BOOL, LineBlock)
	SETGET(BOOL, Open)
	SETGET(BOOL, ComState)

	CAdsErrorConditions() :
		m_SyncError{ FALSE }
		, m_LineBlock{ FALSE }
		, m_Open{ FALSE }
		, m_ComState{ FALSE }
	{}
};

//******************************************************************************************************************************
//******************************************************************************************************************************
inline int32_t CAdsErrorConditions::CheckConditions() const
{
	if ( !m_ComState)
		return ADS_COM_ERROR;

	if ( !m_Open)
		return ADS_NOTOPEN;

	if ( m_SyncError )
		return ADS_SYNCERROR;

	return ADS_OK;
}
//******************************************************************************************************************************
//******************************************************************************************************************************
inline int32_t CAdsErrorConditions::CheckLineConditions() const
{
	auto result = CheckConditions();
	if (result == ADS_OK)
	{
		if ( ! m_LineBlock)
		{
			result = ADS_NOTOPEN;
		}
	}
	return result;
}
