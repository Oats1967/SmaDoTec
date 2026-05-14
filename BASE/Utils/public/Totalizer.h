//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Totalizer
///
/// @file   Totalizer.h
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
#include <cassert>

namespace base
{
namespace utils
{
//-------------------------------------------------------------------------------
// Totalizer in Kg / h


class CTotalizer
{
	uint32_t   m_t;
	float32_t  m_fV;
	float64_t  m_fS;
    BOOL	   m_bStarted;

public:
	CTotalizer				( void );

	BOOL	Open				( const uint32_t _t, const float32_t _fS0, const float32_t _fV);
	BOOL	Close				( void );
	void	Nullen				(const uint32_t _t );
	BOOL	UpdateMassflow		(const uint32_t _t, const float32_t _fV);
    BOOL    UpdateTime			(const uint32_t _t);

	float64_t GetTotalizer() const
	{ return m_fS;	}
};


//---------------------------------------------------------------------------------------------------------------
inline 	CTotalizer::CTotalizer(void) :        m_t(0)
											, m_fV(0.0f)
											, m_fS(0.0)
											, m_bStarted(FALSE)
{}
//---------------------------------------------------------------------------------------------------------------
inline BOOL 	CTotalizer:: 	Open (const uint32_t _t, const float32_t _fS0, const float32_t _fV)
{
    assert(!m_bStarted);
	m_t             = _t;
	m_fV            = SEC2HOUR(_fV); 
	m_fS            = _fS0;
    m_bStarted      = TRUE;
	return TRUE;
}
//---------------------------------------------------------------------------------------------------------------
inline BOOL 	CTotalizer:: 	Close ( void )
{
    assert(m_bStarted);
	m_t             = 0;
	m_fV            = 0.0f;
	m_fS            = 0.0;
    m_bStarted      = FALSE;
	return TRUE;
}
//---------------------------------------------------------------------------------------------------------------
inline void 	CTotalizer:: 	Nullen (const uint32_t _t )
{
    m_t  = _t;
	m_fS = 0.0;
}
//---------------------------------------------------------------------------------------------------------------
inline 	BOOL CTotalizer::UpdateTime(const uint32_t _t)
{
	assert(m_bStarted);
	assert(_t >= m_t);
	auto result = BOOL(_t > m_t);
	if ( result )
	{
		auto difft = _t - m_t;
		m_fS += _F64(difft * m_fV);
		m_t = _t;
	}
	return result;
}
//---------------------------------------------------------------------------------------------------------------
inline 	BOOL CTotalizer :: UpdateMassflow (const uint32_t _t, const float32_t _fV )
{
	auto result = UpdateTime(_t);
	m_fV = SEC2HOUR(_fV);
	return result;
}

}; // namespace utils
}; // namespace base



