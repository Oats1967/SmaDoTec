//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RingBuffer.h
///
/// @file   RingBuffer.h
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

#include <array>
#include <cassert>
#include <algorithm>
#include "BASE/types.h"
#include "Logger/include/Logger.h"

namespace base
{
namespace utils
{

template < class T, uint32_t N >
class CRingBuffer
{
protected:
	std::array<T, N> m_fData;
	uint32_t    m_nCounter;
	uint32_t    m_nSize;
	uint32_t	m_OldestIx;

private:
	//-----------------------------------------------------------------------------------------
	void rollleft(int32_t left);

public:
	CRingBuffer(void) : m_fData{}, m_nCounter(0), m_nSize(N), m_OldestIx(0)
	{}

	virtual ~CRingBuffer(void) = default;
	CRingBuffer(const CRingBuffer&) = default;
	CRingBuffer& operator=(const CRingBuffer&) = default;

	//-----------------------------------------------------------------------------------------
	virtual BOOL isEmpty(void) const
	{ return (m_nCounter == 0);	}

	//-----------------------------------------------------------------------------------------
	uint32_t GetCounter() const
	{	return m_nCounter;	}

	//-----------------------------------------------------------------------------------------
	void Init();

	//-----------------------------------------------------------------------------------------
	void SetBufferSize(uint32_t nSize = N);


	//-----------------------------------------------------------------------------------------
	uint32_t GetBufferSize(void) const
	{
		return m_nSize;
	}

	//-----------------------------------------------------------------------------------------
	const T& GetElement(uint32_t idx) const
	{
		assert(!isEmpty());
		assert(m_nSize > 0);
		assert(idx < m_nCounter);
		return m_fData[(m_OldestIx + idx) % m_nSize];
	}
	//-----------------------------------------------------------------------------------------
	virtual void Add(const T& fD);
};

//********************************************************************************************************
//********************************************************************************************************
template <class T, uint32_t N >
inline void CRingBuffer<T,N>::SetBufferSize(uint32_t nSize)
{
	//LOGDEBUG(" CRingBuffer :: SetBufferSize =  " << _S32(nSize));
	assert(nSize > 0 && nSize <= N);
	if (nSize != m_nSize)
	{
		int32_t rolleft = 0;
		if (nSize < m_nSize)
		{
			// Puffer wird kleiner
			if (m_nCounter < m_nSize)
			{
				rolleft = __max(_S32(m_nCounter) - _S32(nSize), 0);
			}
			else
			{
				rolleft = (m_OldestIx + nSize) % m_nSize;
			}
		}
		else
		{
			// Puffer wird grösser
			rolleft = m_OldestIx;
		}
		rollleft(rolleft);
		m_OldestIx = 0;
		m_nCounter = __min(m_nCounter, nSize);
#if _DEBUG
		for (int32_t index = _S32(__min(nSize, m_nSize)); index < _S32(__max(nSize, m_nSize)); index++)
		{
			memset(&m_fData[index], 0, sizeof(m_fData[index]));
		}
#endif
		m_nSize = nSize;
	}
}
//********************************************************************************************************
//********************************************************************************************************
template <class T, uint32_t N >
inline void CRingBuffer<T, N>::rollleft(int32_t left)
{
#if _DEBUG
	std::array<T, N> data = m_fData;
	for (int32_t index = 0; index < left; index++)
	{
		T temp = data[0];
		for (uint32_t i = 0; i < m_nSize - 1; i++)
		{
			data[i] = data[i + 1];
		}
		data[m_nSize - 1] = temp;
	}
#endif
	std::rotate(m_fData.begin(), m_fData.begin() + left, m_fData.begin() + m_nSize);
#if _DEBUG
	for (int32_t i = 0; i < _S32(m_nSize); i++)
	{
		assert(m_fData[i] == data[i]);
	}
#endif
}
//********************************************************************************************************
//********************************************************************************************************
template <class T, uint32_t N >
inline void CRingBuffer<T, N>::Add(const T& fD)
{
	assert(m_nSize > 0);

	if (m_nCounter < m_nSize)
	{
		m_OldestIx = 0;
		m_fData[m_nCounter] = fD;
		m_nCounter++;
	}
	else
	{
		m_fData[m_OldestIx] = fD;
		m_OldestIx = (m_OldestIx + 1U) % m_nSize;
	}
}
//********************************************************************************************************
//********************************************************************************************************
template <class T, uint32_t N >
inline void CRingBuffer<T, N>::Init()
{
	m_nCounter = 0;
	m_OldestIx = 0;
	LOGDEBUG(" CRingBuffer :: Init ");
}

};
};


