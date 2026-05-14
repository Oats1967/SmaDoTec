//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FifoBuffer.h
///
/// @file   FifoBuffer.h
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
#include <mutex>

namespace base
{
namespace utils
{
template <class T, int32_t n>
class CFifoBuffer
{
	T m_aData[n];
	uint32_t m_Write;
	uint32_t m_Read;
	uint32_t m_Counter;
	std::mutex m_Mutex;

public:
	CFifoBuffer(void) : m_Write(0), m_Read(0), m_Counter(0), m_Mutex()
	{}

	//---------------------------------------------------------------------------------------
	virtual ~CFifoBuffer() = default;
	CFifoBuffer(const CFifoBuffer&) = delete;
	CFifoBuffer& operator=(const CFifoBuffer&) = delete;


	bool			IsEmpty(void) const
	{ return (m_Counter <= 0);	}

	bool			GetFirst(T&);
	bool			Add(const T&);
	bool			Add(const T&&);
};



//---------------------------------------------------------------------------------------
template <class T, int32_t n>
inline bool CFifoBuffer<T, n> ::Add(const T& aData)
{
	if (m_Counter < n)
	{
		m_Mutex.lock();
		m_aData[m_Write] = aData;
		m_Write = (m_Write + 1) % n;
		m_Counter++;
		m_Mutex.unlock();
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------------------
template <class T, int32_t n>
inline bool CFifoBuffer<T, n> ::Add(const T&& aData)
{
	if (m_Counter < n)
	{
		m_Mutex.lock();
		m_aData[m_Write] = aData;
		m_Write = (m_Write + 1) % n;
		m_Counter++;
		m_Mutex.unlock();
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------
template <class T, int32_t n>
inline bool CFifoBuffer<T, n> ::GetFirst(T& aData)
{
	if (m_Counter > 0)
	{
		m_Mutex.lock();
		aData = m_aData[m_Read];
		m_Read = (m_Read + 1) % n;
		m_Counter--;
		m_Mutex.unlock();
		return true;
	}
	return false;
}

};
};

