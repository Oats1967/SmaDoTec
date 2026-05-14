//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module GatefilterCalculator
///
/// @file   GatefilterCalculator.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cmath>
#include "BASE/Utils/public/GatefilterCalculator.h"

namespace base
{
namespace utils
{
//**********************************************************************************************************
//**********************************************************************************************************
CGatefilterCalculator::CGatefilterCalculator(void) : m_fNoiseIndex(0.0f)
	, m_fPidGatefilter(0.5f)
	, m_fMassflowFilter(0.1f)
	, m_fSetpoint(0.0f)
	, m_fMassflowGleitend(0.0f)
	, m_sGatefilterStatus(eGatefilter::GATEFILTER_NONE)
	, m_lPlausibelFailed(0)
	, m_MaxPlausibelCounter(GATEFILTER_NMAX / 2)
{
	Init(m_fPidGatefilter, m_fMassflowFilter, 0.0F, 0.0F );
}
//**********************************************************************************************************
//**********************************************************************************************************
CGatefilterCalculator::CGatefilterCalculator(const CGatefilterCalculator& rGate) : base::utils::CFloatRingBuffer<GATEFILTER_RINGUFFERSIZE>(rGate)
{
	m_fNoiseIndex			= rGate.m_fNoiseIndex;
	m_fMassflowGewichtet	= rGate.m_fMassflowGewichtet;
	m_fMassflowGleitend		= rGate.m_fMassflowGleitend;
	m_fMassflowFilter		= rGate.m_fMassflowFilter;
	m_fPidGatefilter		= rGate.m_fPidGatefilter;
	m_fSetpoint				= rGate.m_fSetpoint;
	m_sGatefilterStatus		= rGate.m_sGatefilterStatus;
	m_lPlausibelFailed		= rGate.m_lPlausibelFailed;
	m_MaxPlausibelCounter	= rGate.m_MaxPlausibelCounter;
}
//**********************************************************************************************************
//**********************************************************************************************************
CGatefilterCalculator& CGatefilterCalculator::operator = (const CGatefilterCalculator& rGate)
{
	if (&rGate != this)
	{
		base::utils::CFloatRingBuffer<GATEFILTER_RINGUFFERSIZE>::operator = (rGate);
		m_fNoiseIndex = rGate.m_fNoiseIndex;
		m_fMassflowGewichtet = rGate.m_fMassflowGewichtet;
		m_fMassflowGleitend = rGate.m_fMassflowGleitend;
		m_fMassflowFilter = rGate.m_fMassflowFilter;
		m_fPidGatefilter = rGate.m_fPidGatefilter;
		m_fSetpoint = rGate.m_fSetpoint;
		m_sGatefilterStatus = rGate.m_sGatefilterStatus;
		m_lPlausibelFailed = rGate.m_lPlausibelFailed;
		m_MaxPlausibelCounter = rGate.m_MaxPlausibelCounter;
	}
	return *this;
}
//**********************************************************************************************************
//**********************************************************************************************************
inline eGatefilter CGatefilterCalculator :: GetGatefilterStatus ( const float32_t& fNoiseIndex ) const
{	
	assert(fNoiseIndex >= 0.0f);
	if ( fNoiseIndex >= m_fPidGatefilter )
	{
		return eGatefilter::GATEFILTER_RED;
	}
	assert(m_fPidGatefilter - fNoiseIndex > 0.0f);
	const float32_t fDiff     = _F32(m_fPidGatefilter - fNoiseIndex );
	return ( fDiff < 0.2 *  m_fPidGatefilter ) ? eGatefilter::GATEFILTER_YELLOW : eGatefilter::GATEFILTER_GREEN;
}
//**********************************************************************************************************
//**********************************************************************************************************
void CGatefilterCalculator :: SetMassflow ( const float32_t fMassflow )
{
	Set(fMassflow);
    m_fMassflowGewichtet     = fMassflow;
    m_fMassflowGleitend      = fMassflow;
}
//**********************************************************************************************************
//**********************************************************************************************************
void CGatefilterCalculator :: SetSetpoint ( const float32_t fSetpoint )
{
	m_fSetpoint = fSetpoint;
}
//**********************************************************************************************************
//**********************************************************************************************************
void CGatefilterCalculator :: Init ( const float32_t fPidGatefilter, const float32_t fMassflowFilter, const float32_t setpoint, const float32_t massflow)
{
    m_fSetpoint          =  setpoint;
    m_fPidGatefilter     = fPidGatefilter;
    m_fMassflowFilter    = RANGE(fMassflowFilter, 0.0f, 1.0f);
	m_fNoiseIndex		 = 0.0f;
    m_sGatefilterStatus  = eGatefilter::GATEFILTER_GREEN;
    m_lPlausibelFailed   = 0;

    SetMassflow(massflow);
}
//**********************************************************************************************************
//**********************************************************************************************************
void CGatefilterCalculator :: Open ( void )
{
	Init (m_fPidGatefilter, m_fMassflowFilter, 0.0F, 0.0F);
}
//**********************************************************************************************************
//**********************************************************************************************************
void CGatefilterCalculator :: Close (void)
{
}
//**********************************************************************************************************
//**********************************************************************************************************
CGatefilterCalculator :: ~CGatefilterCalculator ( void )
{
	Close ();
}
//**********************************************************************************************************
//**********************************************************************************************************
float32_t CGatefilterCalculator :: GetMeanShort () const
{
	float32_t fMeanShort = 0.0f;
	// Kurzfristtrend
	if ( m_nCounter > 0 )
	{
		const int32_t c_count = 3;
		const int32_t  eix   = _S32(m_nCounter);
		const int32_t  six   = __max(_S32(m_nCounter) - c_count, 0);
		assert(eix > six);

		float32_t fM = 0.0f;
		for (int32_t i = six; i < eix; i++)
		{
			fM += GetElement(i);
		}
		fMeanShort = fM / (eix - six);
	}
	return fMeanShort;
}
//**********************************************************************************************************
//**********************************************************************************************************
BOOL CGatefilterCalculator :: Calc ( float32_t fMassflow, NOISEINDEX& rRauschenIndex)
{
    const float32_t cMassflowFilterSmooth = m_fMassflowFilter * 0.5f;

	Add(fMassflow);

	// langfristiger Trend
	const float32_t fMeanLong   = GetMean();
	assert(fMeanLong > 0.0f);
 
	// langfristiger Trend
	const float32_t fMeanShort   = GetMeanShort();
	assert(fMeanShort > 0.0f);

	const float32_t fMeanValue = _F32(fabs(fMeanShort - m_fSetpoint)) < _F32(fabs(fMeanLong - m_fSetpoint)) ? fMeanShort : fMeanLong;

		
	// Sortieren
    if ( fMassflow <= 0.1*m_fSetpoint)
    {
        if ( m_fSetpoint > 1e-06)
        {
            m_fNoiseIndex         = m_fPidGatefilter * 10000;
	        m_sGatefilterStatus    = eGatefilter::GATEFILTER_RED;
            m_fMassflowGewichtet  = fMassflow;
            m_fMassflowGleitend   = fMassflow;
        }
        else
        {
            m_fNoiseIndex        = 0.0f;
	        m_sGatefilterStatus    = eGatefilter::GATEFILTER_GREEN;
            m_fMassflowGewichtet   = 0.0f;
            m_fMassflowGleitend    = 0.0f;
        }
    }
    else
    {
        m_fNoiseIndex       = _F32(fabs((fMassflow / m_fSetpoint) - 1.0f));
        m_fMassflowGewichtet  = (1.0f-m_fMassflowFilter)* m_fMassflowGewichtet   + m_fMassflowFilter * fMeanValue;
        if (( fMassflow < (1.0f - m_fPidGatefilter) * m_fSetpoint ) ||
			( fMassflow > (1.0f + m_fPidGatefilter) * m_fSetpoint))
        {
            m_fMassflowGleitend          = (1.0f - cMassflowFilterSmooth) * m_fMassflowGleitend + cMassflowFilterSmooth  * fMeanValue;
			m_sGatefilterStatus			 = eGatefilter::GATEFILTER_RED;
		}
        else
        {
            m_fMassflowGleitend          = (1.0f - cMassflowFilterSmooth)* m_fSetpoint  + cMassflowFilterSmooth * fMeanValue;
	        m_sGatefilterStatus          = GetGatefilterStatus(m_fNoiseIndex);
            float32_t fDiff = _F32(fabs(fMeanValue - m_fSetpoint));
			if (  fDiff < _F32(fabs(m_fMassflowGewichtet - m_fSetpoint)) )
			{
				m_fMassflowGewichtet = fMeanValue;
			}
			if ( fDiff < _F32(fabs(m_fMassflowGleitend - m_fSetpoint)) )
			{
				m_fMassflowGleitend = fMeanValue;
			}
        }
    }

	rRauschenIndex.fNoiseIndex	= m_fNoiseIndex;
	rRauschenIndex.sGatefilter  = m_sGatefilterStatus;
    BOOL bPlausibel = (m_sGatefilterStatus != eGatefilter::GATEFILTER_RED);
    if ( bPlausibel )
	{
		if (m_lPlausibelFailed > 0)
		{
			if ( m_sGatefilterStatus == eGatefilter::GATEFILTER_GREEN)
			{
				m_lPlausibelFailed = __min(m_lPlausibelFailed, 2);
			}
			m_lPlausibelFailed--;
		}
	}
	else
	{
		if (m_lPlausibelFailed < m_MaxPlausibelCounter)
		{
			m_lPlausibelFailed++;
		}
	}
	rRauschenIndex.lPlausibelFailed         = m_lPlausibelFailed;
    rRauschenIndex.fMassflowGewichtet        = m_fMassflowGewichtet;
    rRauschenIndex.fMassflow                 = fMassflow;
    rRauschenIndex.fMassflowGleitend         = m_fMassflowGleitend;
	return bPlausibel;
}
//**********************************************************************************************************
//**********************************************************************************************************
BOOL CGatefilterCalculator :: Push ( const float32_t fMassflow, NOISEINDEX& rRauschenIndex)
{
	BOOL bPlausibel = FALSE;

    auto fDiff = _F32(fabs( fMassflow - m_fSetpoint ));
	if ( fDiff < m_fPidGatefilter * m_fSetpoint )
	{
		Calc(fMassflow, rRauschenIndex);
		bPlausibel = TRUE;
	}
	else
	{
		m_fNoiseIndex		= _F32(fabs((fMassflow / m_fSetpoint) - 1.0f));
	    m_sGatefilterStatus  = GetGatefilterStatus(m_fNoiseIndex);
		rRauschenIndex.lPlausibelFailed  = m_lPlausibelFailed;
		rRauschenIndex.fMassflowGewichtet  = m_fMassflowGewichtet;
		rRauschenIndex.fMassflow =  fMassflow;
		rRauschenIndex.fMassflowGleitend  =  m_fMassflowGewichtet;
		rRauschenIndex.fNoiseIndex	= m_fNoiseIndex;
		rRauschenIndex.sGatefilter	= m_sGatefilterStatus;
		assert(m_sGatefilterStatus != eGatefilter::GATEFILTER_GREEN);
		m_lPlausibelFailed++;
	}
    return bPlausibel;
}

}; // namespace utils
}; // namespace base


