//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LCImplMacro.h
///
/// @file   LCImplMacro.h
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


					



//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#define IMPLEMENT_LCINIT(t, a)										\
 int32_t CDose :: LCInit##a	(const uint32_t LCAddress, const t b)	\
{																	\
	assert(LCAddress != INVALID_LC_LINKADDRESS);					\
	auto& item = mLCGlobal[LCAddress].get();								\
	item.##a.Init(b);												\
	return DOSIERUNG_OK;											\
}

#define IMPLEMENT_LCGET(t, a)									\
 int32_t CDose :: LCGet##a	(const uint32_t LCAddress, t *b) const		\
{																\
	assert(LCAddress != INVALID_LC_LINKADDRESS);					\
	assert(b);													\
	const auto& item = mLCGlobal[LCAddress].get();							\
	*b = item.##a.Get();										\
	return DOSIERUNG_OK;										\
}

#define IMPLEMENT_LCSET(t, a)									\
 int32_t CDose :: LCSet##a	( const uint32_t LCAddress, const t b)						\
{																\
	assert(LCAddress != INVALID_LC_LINKADDRESS);					\
	auto& item = mLCGlobal[LCAddress].get();								\
	if ( item.##a.Test(b) )										\
	{															\
		std::lock_guard<std::mutex> lock(mLCGlobal[LCAddress]);				\
		item.##a.Set(b);										\
	}															\
	return DOSIERUNG_OK;										\
}																


#define IMPLEMENT_INLINE_LC(t, a)							\
		IMPLEMENT_LCINIT(t, a)								\
		IMPLEMENT_LCGET(t, a)								\
		IMPLEMENT_LCSET(t, a)

