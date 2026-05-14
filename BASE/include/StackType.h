//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module StackType
///
/// @file   StackType.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <cassert>
#include <limits.h>
#include <cstdlib>
#include <cmath>
#include <intrin.h>
#include "BASE/Utils/public/BitOp.h"

#define CAST2SHORT(a) ((int16_t*)(a))
#define CAST2WORD(a)  ((uint16_t*)(a))
#define CAST2LONG(a)  ((int32_t*)(a))
#define CAST2ULONG(a) ((uint32_t*)(a))
#define CAST2BYTE(a)  ((uint8_t*)(a))
#define CAST2LDWORD(a) ((uint64_t*)(a))


#define BUFFER_UINT64  (*CAST2LDWORD(&m_pbBuffer[m_lOffset]))
#define BUFFER_UINT32  (*CAST2ULONG(&m_pbBuffer[m_lOffset]))
#define BUFFER_UINT16 (*CAST2WORD(&m_pbBuffer[m_lOffset]))
#define BUFFER_UINT8  (*CAST2BYTE(&m_pbBuffer[m_lOffset]))

namespace base
{
class CStackType
{
	private:
		uint8_t* m_pbBuffer;
		int32_t  m_lOffset;
		int32_t  m_lLength;
		BOOL	 m_bBigEndian;

	private:
		/*
		uint32_t long2Dword(int32_t lV) const;
		int32_t Dword2Long(uint32_t uL) const;
		uint16_t Short2Word(int16_t sV) const;
		int16_t Word2Short(uint16_t wV) const;
			*/

		static uint16_t SignedToUnsigned(const int16_t lV);
		static uint32_t SignedToUnsigned(const int32_t lV);
		static uint64_t SignedToUnsigned(const int64_t lV);
		static int16_t UnsignedToSigned(const uint16_t lV);
		static int32_t UnsignedToSigned(const uint32_t lV);
		static int64_t UnsignedToSigned(const uint64_t lV);

		//---------------------------------------------------------------------------------------------------------

			public:
				CStackType(BOOL bigEndian, uint8_t* p, int32_t wL) : m_bBigEndian(bigEndian)
																, m_pbBuffer(p)
																, m_lLength(wL)
																, m_lOffset(0)
				{}

				void pushS64(const int64_t lV);
				void pushS32(const int32_t lV);
				void pushS16(const int16_t lV);
				void pushS8(const int8_t lV);
				void pushU64(const uint64_t lV);
				void pushU32(const uint32_t lV);
				void pushU16(const uint16_t lV);
				void pushU8(const uint8_t lV);

				int64_t  popS64();
				int32_t  popS32();
				int16_t  popS16();
				int8_t   popS8();
				uint64_t popU64();
				uint32_t popU32();
				uint16_t popU16();
				uint8_t  popU8();

				int32_t GetCurrentLength()
				{
		 return m_lOffset;
}

void AddOffset(int32_t lOffset);
};

//---------------------------------------------------------------------------------------------------------
inline uint16_t CStackType::SignedToUnsigned(const int16_t lV)
{
	uint16_t uL = std::abs(lV);
	SETBIT(uL, 15, BIT(lV < 0));
	return uL;
}
//---------------------------------------------------------------------------------------------------------
inline uint32_t CStackType::SignedToUnsigned(const int32_t lV)
{
	uint32_t uL = std::abs(lV);
	SETBIT(uL, 31, BIT(lV < 0));
	return uL;
}
//---------------------------------------------------------------------------------------------------------
inline uint64_t CStackType::SignedToUnsigned(const int64_t lV)
{
	uint64_t uL = std::abs(lV);
	SETBIT(uL, 63, BIT(lV < 0));
	return uL;
}
//---------------------------------------------------------------------------------------------------------
inline int16_t CStackType::UnsignedToSigned(uint16_t wV)
{
	uint32_t  uL = _U32(wV);
	int16_t vz = _S16((ISBIT(uL, 15) << 1));
	SETBIT(uL,15,0);			// hoechses Bit loeschen
	int32_t  lV = (1 - vz) * _S32(uL);
	return _S16(lV);
}
//---------------------------------------------------------------------------------------------------------
inline int32_t CStackType::UnsignedToSigned(uint32_t wV)
{
	uint32_t  uL = _U32(wV);
	int16_t vz = _S16((ISBIT(uL, 31) << 1));
	SETBIT(uL, 31, 0);			// hoechses Bit loeschen
	int32_t  lV = _S32((1 - vz) * uL);
	return lV;
}
//---------------------------------------------------------------------------------------------------------
inline int64_t CStackType::UnsignedToSigned(const uint64_t wV)
{
	uint64_t  uL = _U64(wV);
	int16_t vz = _S16((ISBIT(uL, 63) << 1));
	SETBIT(uL, 63, 0);			// hoechses Bit loeschen
	int64_t  lV = _S64((1 - vz) * uL);
	return lV;
}

//---------------------------------------------------------------------------------------------------------
inline void CStackType::AddOffset(const int32_t lOffset)
{
	assert(m_lOffset + lOffset <= m_lLength);
	m_lOffset += lOffset;
}


//---------------------------------------------------------------------------------------------------------
inline void CStackType::pushU64(const uint64_t lV)
{
	assert(m_lOffset + _S32(sizeof(lV)) <= m_lLength);
	BUFFER_UINT64 = SLAVE_TO_MASTER_LDWORD(m_bBigEndian, lV);
	m_lOffset += sizeof(lV);
}
//---------------------------------------------------------------------------------------------------------
inline void CStackType::pushU32(const uint32_t lV)
{
	assert(m_lOffset + _S32(sizeof(lV)) <= m_lLength);
	BUFFER_UINT32 = SLAVE_TO_MASTER_DWORD(m_bBigEndian, lV);
	m_lOffset += sizeof(lV);
}
//---------------------------------------------------------------------------------------------------------
inline void CStackType::pushU16(const uint16_t wV)
{
	assert(m_lOffset + _S32(sizeof(wV)) <= m_lLength);
	BUFFER_UINT16 = SLAVE_TO_MASTER_WORD(m_bBigEndian, wV);
	m_lOffset += sizeof(wV);
}
//---------------------------------------------------------------------------------------------------------
inline void CStackType::pushU8(const uint8_t wV)
{
	assert(m_lOffset + _S32(sizeof(wV)) <= m_lLength);
	BUFFER_UINT8 = wV;
	m_lOffset += sizeof(wV);
}
//---------------------------------------------------------------------------------------------------------
inline void CStackType::pushS64(const int64_t lV)
{
	pushU64(SignedToUnsigned(lV));
}
//---------------------------------------------------------------------------------------------------------
inline void CStackType::pushS32(const int32_t lV)
{
	pushU32(SignedToUnsigned(lV));
}
//---------------------------------------------------------------------------------------------------------
inline void CStackType::pushS16(const int16_t lV)
{
	int16_t sV = lV;
	assert(sV <= SHRT_MAX);
	if (sV > SHRT_MAX)
		sV = SHRT_MAX;
	else if (sV < SHRT_MIN)
		sV = SHRT_MIN;

	pushU16(SignedToUnsigned(sV));
}
//---------------------------------------------------------------------------------------------------------
inline void CStackType::pushS8(const int8_t wV)
{
	pushU32(SignedToUnsigned(wV));
}

//---------------------------------------------------------------------------------------------------------
inline int64_t CStackType::popS64()
{
	int64_t  lV = UnsignedToSigned(MASTER_TO_SLAVE_LDWORD(m_bBigEndian, BUFFER_UINT64));
	m_lOffset += sizeof(lV);
	return lV;
}
//---------------------------------------------------------------------------------------------------------
inline int32_t CStackType::popS32()
{
	int32_t  lV = UnsignedToSigned(MASTER_TO_SLAVE_DWORD(m_bBigEndian, BUFFER_UINT32));
	m_lOffset += sizeof(lV);
	return lV;
}
//---------------------------------------------------------------------------------------------------------
inline int16_t CStackType::popS16()
{
	int16_t  lV = UnsignedToSigned(MASTER_TO_SLAVE_WORD(m_bBigEndian, BUFFER_UINT16));
	m_lOffset += sizeof(lV);
	return lV;
}
//---------------------------------------------------------------------------------------------------------
inline int8_t CStackType::popS8()
{
	int8_t  lV = _S8(BUFFER_UINT8);
	m_lOffset += sizeof(lV);
	return lV;
}


//---------------------------------------------------------------------------------------------------------
inline uint64_t CStackType::popU64()
{
	uint64_t  lV = MASTER_TO_SLAVE_LDWORD(m_bBigEndian, BUFFER_UINT64);
	m_lOffset += sizeof(lV);
	return lV;
}
//---------------------------------------------------------------------------------------------------------
inline uint32_t CStackType::popU32()
{
	uint32_t  lV = MASTER_TO_SLAVE_DWORD(m_bBigEndian, BUFFER_UINT32);
	m_lOffset += sizeof(lV);
	return lV;
}
//---------------------------------------------------------------------------------------------------------
inline uint16_t CStackType::popU16()
{
	uint16_t  lV = MASTER_TO_SLAVE_WORD(m_bBigEndian, BUFFER_UINT16);
	m_lOffset += sizeof(lV);
	return lV;
}
//---------------------------------------------------------------------------------------------------------
inline uint8_t CStackType::popU8()
{
	uint8_t  lV = _U8(BUFFER_UINT8);
	m_lOffset += sizeof(lV);
	return lV;
}

};

