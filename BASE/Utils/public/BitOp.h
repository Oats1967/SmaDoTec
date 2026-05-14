//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module BitOp.h
///
/// @file   BitOp.h
///
///
///	@coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
///
//------------------------------------------------------------------------------------
#pragma once


#include "BASE/types.h"


#define	SETNFLAG_U32(b,ulb)		(_U32(b)<<_U8(ulb))
#define	SETNFLAG_U64(b,ulb)		(_U64(b)<<_U8(ulb))
#define	SETFLAG32(ulb)			SETNFLAG_U32(1,ulb)
#define	SETFLAG64(ulb)			SETNFLAG_U64(1,ulb)
#define	DELETEFLAG32(uls, ulb)	((uls)&(~SETFLAG32(ulb)))
#define	DELETEFLAG64(uls, ulb)	((uls)&(~SETFLAG64(ulb)))

#define SBIT_N_U32(a,b)         (_U32((a) != 0) << b)
#define SBIT_N_U16(a,b)         (_U16((a) != 0) << b)
#define SBIT_NEG_U32(a,b)       (_U32((a) == 0) << b)
#define SBIT_NEG_U16(a,b)       (_U16((a) == 0) << b)

#define __HIBYTE(x) _U8(_U8((x)>>8) & 0xff)
#define __LOBYTE(x) _U8(_U8(x)& 0xff)
#define __HIWORD(x) _U16(_U16((x)>>16) & 0xffff)
#define __LOWORD(x) _U16(_U16(x) & 0xffff)

namespace base
{
namespace utils
{
template <typename T>
//------------------------------------------------------------------------
void setBit(T& uls, uint8_t ulb, BIT V)
{
	// Clearbit
	const T ki = ~((static_cast<T>(1U)) << ulb);
	T ri = uls & ki;

	// Setbit
	const T pi = static_cast<T>((static_cast<T>(V != 0)) << ulb);
	uls = ri | pi;
}


//------------------------------------------------------------------------
template <typename T>
//------------------------------------------------------------------------
bool isBitSet(T uls, uint8_t ulb)
{
	return (uls & ((static_cast<T>(1U)) << ulb)) != 0;
}

//------------------------------------------------------------------------
template <typename T>
//------------------------------------------------------------------------
void toggleBit(T& uls, uint8_t ulb)
{
	uls ^= static_cast <T>(1UL << ulb);
}

//------------------------------------------------------------------------
inline uint32_t swapu32(uint32_t p)
{
	return (_U32(__LOWORD(p)) << 16U) | _U32(__HIWORD(p));
}
//------------------------------------------------------------------------
inline uint16_t swapu16(uint16_t p)
{
	return (_U16(__LOBYTE(p)) << 8U) | _U16(__HIBYTE(p));
}

};
};


#define	ISBITSET(a,b)		base::utils::isBitSet(a, _U8(b))
#define	ISBIT(a,b)			ISBITSET(a,b)
#define	ISNBIT(a,b)			(!ISBITSET(a,b))
#define	SETBIT(a,b,c)		base::utils::setBit(a, _U8(b),  BIT(c))
#define	TOGGLEBIT(a,b)		base::utils::toggleBit(a, _U8(b))




//---------------------------------------------------------------------------------------------------------
#define SLAVE_TO_MASTER_WORD(bE,a)  ( bE )?_U16(_byteswap_ushort(a)):(a)
#define MASTER_TO_SLAVE_WORD(bE,a)  SLAVE_TO_MASTER_WORD(bE,a)
#define SLAVE_TO_MASTER_DWORD(bE,a) ( bE )?_U32(_byteswap_ulong(a)):(a)
#define MASTER_TO_SLAVE_DWORD(bE,a) SLAVE_TO_MASTER_DWORD(bE,a)
#define SLAVE_TO_MASTER_LDWORD(bE,a) ( bE )?_U64(_byteswap_uint64(a)):(a)
#define MASTER_TO_SLAVE_LDWORD(bE,a) SLAVE_TO_MASTER_LDWORD(bE,a)


