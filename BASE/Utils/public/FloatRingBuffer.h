//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FloatRingBuffer
///
/// @file   FloatRingBuffer.h
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
#include "BASE/Base.h"
#include "BASE/Utils/public/RingBuffer.h"

namespace base
{
namespace utils
{
//--------------------------------------------------------------------------------------------------------------------------------------
template < const uint32_t N>
class CFloatRingBuffer : public CRingBuffer <float32_t, N >
{
protected:
	float32_t  m_fMin;
	float32_t  m_fMax;
	float32_t  m_fMean;

	//---------------------------------------------------------------------------------------
	void CalcAll(float32_t* _fMin, float32_t* _fMax, float32_t* _fMean) const
	{
		assert(_fMin != NULL);
		assert(_fMax != NULL);
		assert(_fMean != NULL);

		if (this->isEmpty())
		{
			*_fMin = *_fMax = *_fMean =  0.0f;
			return;
		}
		auto dMean = _F64(this->m_fData[0]);
		auto  fMin = this->m_fData[0];
		auto  fMax = this->m_fData[0];
		if (this->m_nCounter > 1U)
		{
			for (uint32_t i = 1; i < this->m_nCounter; i++)
			{
				const auto fTemp = this->m_fData[i];
				if (fTemp < fMin)
				{
					fMin = fTemp;
				}
				else if (fTemp > fMax)
				{
					fMax = fTemp;
				}
				dMean += _F64(fTemp);
			}
			dMean /= _F64(this->m_nCounter);
		}
		*_fMin = fMin;
		*_fMax = fMax;
		*_fMean = _F32(dMean);
	}


#if _DEBUG
	//---------------------------------------------------------------------------------------
	void CalcMean(void)
	{
		m_fMean = 0.0f;
		if (this->isEmpty())
		{
			return;
		}

		float64_t dMean = 0.0;
		for (uint32_t i = 0; i < this->m_nCounter; i++)
		{
			dMean += this->m_fData[i];
		}
		m_fMean = float32_t(dMean / this->m_nCounter);
	}


	//---------------------------------------------------------------------------------------
	void CalcMin(void)
	{
		m_fMin = 0.0f;
		if (this->isEmpty())
		{
			return;
		}
		m_fMin = this->m_fData[0];
		for (uint32_t i = 1; i < this->m_nCounter; i++)
		{
			if (this->m_fData[i] < m_fMin)
			{
				m_fMin = this->m_fData[i];
			}
		}
	}
	//---------------------------------------------------------------------------------------
	void CalcMax(void)
	{
		m_fMax = 0.0f;
		if (this->isEmpty())
		{
			return;
		}
		m_fMax = this->m_fData[0];
		for (uint32_t i = 1; i < this->m_nCounter; i++)
		{
			if (this->m_fData[i] > m_fMax)
			{
				m_fMax = this->m_fData[i];
			}
		}
	}
#endif
	//---------------------------------------------------------------------------------------
	float32_t CalcVariance(void)
	{
		if (this->isEmpty())
		{
			return 0.0F;
		}
		float64_t dVariance = 0.0;
		const auto n = this->m_nCounter;
		assert(n > 0);

		for (uint32_t i = 0; i < n; i++)
		{
			auto op = _F64(this->m_fData[i] - m_fMean);
			dVariance += (op * op);
		}
		return _F32(dVariance / n);
	}

	//---------------------------------------------------------------------------------------
	float32_t  CalcStandardDeviation(void)
	{
		return _F32(sqrt(CalcVariance()));
	}


public:
	//---------------------------------------------------------------------------------------
	CFloatRingBuffer(void) : CRingBuffer <float32_t, N >()
		, m_fMin(0.0f)
		, m_fMax(0.0f)
		, m_fMean(0.0f)
	{
		this->m_fData.fill(0.0F);
	}
	//---------------------------------------------------------------------------------------
	~CFloatRingBuffer() override = default;
	//---------------------------------------------------------------------------------------
	CFloatRingBuffer(const CFloatRingBuffer& rData) : CRingBuffer <float32_t, N >(rData)
	{
		m_fMin = rData.m_fMin;
		m_fMax = rData.m_fMax;
		m_fMean = rData.m_fMean;
	}
	//---------------------------------------------------------------------------------------
	CFloatRingBuffer& operator=(const CFloatRingBuffer& rData)
	{
		if (&rData != this)
		{
			CRingBuffer <float32_t, N >::operator = (rData);
			m_fMin = rData.m_fMin;
			m_fMax = rData.m_fMax;
			m_fMean = rData.m_fMean;
		}
		return *this;
	}
	//---------------------------------------------------------------------------------------
	float32_t GetMean(void) const
	{ return m_fMean;	}

	//---------------------------------------------------------------------------------------
	float32_t GetMin(void) const
	{ return m_fMin; }

	//---------------------------------------------------------------------------------------
	float32_t GetMax(void) const
	{	return m_fMax;	}

	//---------------------------------------------------------------------------------------
#if 0
	float32_t GetStreuung(void) const
	{ return m_fStreuung;	}
#endif
	//---------------------------------------------------------------------------------------
	void Add(const float32_t& _fD) override
	{
		CRingBuffer<float32_t, N> ::Add(_fD);

#if _DEBUG
		CalcMin();

		CalcMax();

		CalcMean();

		float32_t fMin, fMax, fMean;
		CalcAll(&fMin, &fMax, &fMean);

		assert(fabs(fMin - m_fMin) < 1e-07);
		assert(fabs(fMax - m_fMax) < 1e-07);
		assert(fabs(fMean - m_fMean) < 1e-07);
#else
		CalcAll(&m_fMin, &m_fMax, &m_fMean);

#endif
	}
	//---------------------------------------------------------------------------------------
	void Set(float32_t f)
	{
		for (uint32_t i = 0; i < this->m_nSize; i++)
		{
			Add(f);
		}
	}

	void Init()
	{
		m_fMin = 0.0f;
		m_fMax = 0.0f;
		m_fMean = 0.0f;
		CRingBuffer <float32_t, N >::Init();
	}

};
};
};



