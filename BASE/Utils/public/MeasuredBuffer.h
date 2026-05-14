//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MeasuredBuffer
///
/// @file   MeasuredBuffer.h
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

#include "BASE/Utils/public/RingBuffer.h"
#include "BASE/math/public/linear.h"
#include "BASE/Utils/public/WeightPair.h"


namespace base
{
namespace utils
{
enum class eTendency : uint16_t
{
	TENDENCY_UNKNOWN = 0,
	TENDENCY_EQUAL,
	TENDENCY_RAISING,
	TENDENCY_FALLING
};
	//--------------------------------------------------------------------------------------------------------------------------------------
template <const int32_t N> 
class CMeasuredBuffer : public CRingBuffer<CWeightPair,N>
{
	float64_t m_dC0;
	float64_t m_dC1;

private:
	eTendency Tendency();
	BOOL	  calc_straight_line();

	float64_t GetC1() const
	{	return m_dC1;	}

public:
	//-------------------------------------------------------------------------
	CMeasuredBuffer(void) :   m_dC0(0.0)
							, m_dC1(0.0)
	{}

	//--------------------------------------------------------------------------
	BOOL GetMassflow(float32_t* pF);
};
//************************************************************************************************************
//************************************************************************************************************
template <const int32_t N>
inline BOOL CMeasuredBuffer<N>::GetMassflow(float32_t* pF)
{
	assert(pF);
	*pF = 0.0f;
	auto tendency = Tendency();
	auto result  = (tendency == eTendency::TENDENCY_FALLING) || (tendency == eTendency::TENDENCY_EQUAL);
	if ( result)
	{
		if (tendency == eTendency::TENDENCY_FALLING)
		{
			assert(m_dC1 < 0.0);
			*pF = _F32(fabs(m_dC1 * 3600.0));
		}
	}
	return result;
}
//************************************************************************************************************
//************************************************************************************************************
template <const int32_t N>
inline eTendency CMeasuredBuffer<N>::Tendency()
{
	eTendency s = eTendency::TENDENCY_UNKNOWN;

	auto result = calc_straight_line();
	if (result)
	{
		if (m_dC1 < 0)
			s = eTendency::TENDENCY_FALLING;
		else if (m_dC1 > 0)
			s = eTendency::TENDENCY_RAISING;
		else
			s = eTendency::TENDENCY_EQUAL;
	}
	return s;
}

//--------------------------------------------------------------------------
template <const int32_t N>
inline BOOL CMeasuredBuffer<N>::calc_straight_line()
{
	const uint32_t count = this->GetCounter();
	assert(count <= N);
	if (count < 3)
	{
		LOGDEBUG("calc_straight_line failed, m_nCounter = " << _S32(this->m_nCounter));
		return FALSE;
	}
	const auto t0 = this->GetElement(0).m_ulT;

	float64_t dX[N];
	float64_t dY[N];
	for (uint32_t k = 0; k < count; k++)
	{
		const auto& data = this->GetElement(k);
#if _DEBUG
		if (k > 0)
		{
			assert(this->GetElement(k - 1).m_ulT < data.m_ulT);
		}
#endif
		assert(data.m_ulT >= t0);
		dX[k] = _F64(data.m_ulT - t0) / 1000.0; // Differenzbildung nicht unbedingt notwendig, hat aber den Vorteil nicht zu grosser Zahlen
		dY[k] = data.m_fWeight;
	}
	(void)GNU::gsl_fit_linear(dX, dY, count, &m_dC0, &m_dC1);
	return TRUE;
}


}; // namespace utils
}; // namespace base



