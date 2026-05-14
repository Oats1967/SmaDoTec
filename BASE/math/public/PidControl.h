//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module PidControl
///
/// @file   PidControl.h
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

#include <cassert>
#include "BASE/types.h"

namespace base
{
namespace math
{
class CPidControl
{
    float64_t m_e0;
	float64_t m_fu;
	float64_t m_SI;
	float64_t m_u1;
	float64_t m_KP;
	float64_t m_KI;
	float64_t m_KD;
	uint32_t  m_t;

public:
	CPidControl( void );

	void    Open					( const float32_t , const float32_t , const float32_t );
	void    Close() {}
	float32_t  Start				( const uint32_t t, const float32_t);
	float32_t  Update				( const uint32_t t, const float32_t);
};


//--------------------------------------------------------------------------------------------------------------------
inline 	CPidControl::CPidControl(void) :    m_e0(0.0)
	                                    , m_fu(0.0)
	                                    , m_SI(0.0)
                                        , m_u1(0.0)
                                        , m_KP(0.0)
                                        , m_KI(0.0)
                                        , m_KD(0.0)
                                        , m_t(0)
{}




//--------------------------------------------------------------------------------------------------------------------
inline float32_t CPidControl:: 	Start ( const uint32_t t, const float32_t fV)
{
	m_e0		= _F64(fV);
	m_SI		= 0.0;
	m_t		    = t;
	m_fu		= m_KP * m_e0;
    return _F32(m_fu);
}
//--------------------------------------------------------------------------------------------------------------------
inline float32_t CPidControl :: Update    ( const uint32_t t, const float32_t fV)
{
	assert(t > m_t);
    if (t > m_t )
    {
		float64_t dt  = _F64(t - m_t);
		float64_t e1 = _F64(fV);
		m_SI += (e1 + m_e0) / 2.0 * dt;
		m_fu = (m_KP * e1) + (m_KI * m_SI ) + (m_KD * (e1 - m_e0) / dt);
		m_t = t;
		m_e0 = e1;
	}
	return _F32(m_fu);
}


//--------------------------------------------------------------------------------------------------------------------
inline 	void CPidControl :: Open ( const float32_t _KP, const float32_t _KI, const float32_t _KD)
{
	m_KP = _F64(_KP);
	m_KI = _F64(_KI);
	m_KD = _F64(_KD);
}


}; // namespace math
}; // namespace base



