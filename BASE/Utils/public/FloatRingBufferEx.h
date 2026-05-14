//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CFloatRingBufferEx
///
/// @file   FloatRingBufferEx.h
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
#include "BASE/Utils/public/FloatRingBuffer.h"

namespace base
{
namespace utils
{
//--------------------------------------------------------------------------------------------------------------------------------------
template < uint32_t N>
class CFloatRingBufferEx : public CFloatRingBuffer < N >
{
public:
	//---------------------------------------------------------------------------------------
	CFloatRingBufferEx(void)
	{}

	//---------------------------------------------------------------------------------------
	~CFloatRingBufferEx() override = default;

	CFloatRingBufferEx(const CFloatRingBufferEx&) = delete;
	CFloatRingBufferEx& operator=(const CFloatRingBufferEx&) = delete;

	//---------------------------------------------------------------------------------------
	void Add(float32_t& _fD) override
	{
		float32_t fD = _fD;

		if (this->m_nCounter >= this->m_nSize)
		{
			if ((fD > this->m_fMin) && (fD - this->m_fMin < __max(4 * this->m_fStreuung, 0.1f * fD)))
			{
				fD = this->m_fMin;
			}
		}
		CFloatRingBuffer<N> ::Add(fD);
	}
};

};
};




