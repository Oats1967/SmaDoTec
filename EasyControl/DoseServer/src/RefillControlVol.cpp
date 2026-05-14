//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RefillControlVol
///
/// @file   RefillControlVol.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "DoseDataLib/include/DoseData.h"
#include "RefillControlVol.h"


//**************************************************************************************************
//**************************************************************************************************
CRefillControlVol::CRefillControlVol(int32_t id) : CBaseClass(id)
, m_FeederEmptyStart{ 0 }
, m_FeederEmptyEnd{ 0 }
, m_AdsClient{ id }
, m_t0{ 0 }
, m_tEF{ 0 }
, m_DurationFeederEmpty{ 0 }
, m_DurationFeederEmptyExit{ 0 }
{
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillControlVol::InitExecute(void)
{
	auto result = CBaseClass::InitExecute();
	if (result)
	{
		m_AdsClient.Init();
		Dose_DSVPopRefillFeederEmptyStart(m_sID, &m_FeederEmptyStart);
		m_FeederEmptyStart = RANGE(m_FeederEmptyStart, 1U, c_MaxFeederEmptyStart);
	}
	return result;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillControlVol::ExitExecute(void)
{
	auto result = CBaseClass::IsInit();
	if (result)
	{
		m_AdsClient.Exit();
		result = CBaseClass::ExitExecute();
	}
	return result;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillControlVol::CheckFeederEmpty()
{
	if (IsWeightBelowMin())
	{
		if (m_ControlBits.flags.bLastMinState)
		{
			m_ControlBits.flags.bLastMinState = false;
			m_tEF = m_st;
			assert(! m_ControlBits.flags.bEmptyFeeder);
		}
		else
		{
			m_ControlBits.flags.bEmptyFeeder = (m_st >= m_tEF + m_FeederEmptyStart);
		}
	}
	else
	{
		m_ControlBits.flags.bEmptyFeeder = false;
		m_ControlBits.flags.bLastMinState = true;
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillControlVol :: Update (const uint32_t t)
{
	assert(IsInit());
	auto result = CBaseClass::Update(t);
	if ( result)
	{
		m_AdsClient.Update(t);

		BOOL bChanged = FALSE;
		Dose_DSVGetChangedRefillFeederEmptyStart(m_sID, &bChanged);
		if (bChanged)
		{
			Dose_DSVPopRefillFeederEmptyStart(m_sID, &m_FeederEmptyStart);
			m_FeederEmptyStart = RANGE(m_FeederEmptyStart, 1U, c_MaxFeederEmptyStart);
		}
		CheckFeederEmpty();
	}
	return result;
}





