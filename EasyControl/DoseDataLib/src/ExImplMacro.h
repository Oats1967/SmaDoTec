//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ExImplMacro.h
///
/// @file   ExImplMacro.h
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


//---------------------------------------------------------------------------------------------------
#define IMPLEMENT_EXPBGET(a)									    \
 int32_t CDose :: EXPBGet##a	( BOOL *b) const					\
{																    \
	assert(b);													    \
	const auto& item = mEGlobal.get();								\
	*b = item.##a;													\
	return DOSIERUNG_OK;										    \
}


#define IMPLEMENT_EXPBSET(a)									    \
 int32_t CDose :: EXPBSet##a	( const BOOL b)						\
{																    \
	auto& item = mEGlobal.get();									\
	if (item.##a != b)												\
	{																\
		std::lock_guard<std::mutex> lock(mEGlobal);					\
		item.##a = b;												\
	}																\
	return DOSIERUNG_OK;										    \
}


#define IMPLEMENT_EXPBRESET(a)									    \
 int32_t CDose :: EXPBReset##a	( void )							\
{	return EXPBSet##a(FALSE); }



//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#define IMPLEMENT_EXINIT(t, a)									\
 int32_t CDose :: EXInit##a	(const t b)							\
{																\
	auto& item = mEGlobal.get();							\
	item.##a.Init(b);										\
	return DOSIERUNG_OK;										\
}

#define IMPLEMENT_EXGET(t, a)									\
 int32_t CDose :: EXGet##a	( t *b) const						\
{																\
	assert(b);													\
	const auto& item = mEGlobal.get();							\
	*b = item.##a.Get();										\
	return DOSIERUNG_OK;										\
}



#define IMPLEMENT_EXPOP(t, a)									\
 int32_t CDose :: EXPop##a	( t *b)	        \
{																\
	assert(b);													\
	std::lock_guard<std::mutex> lock(mEGlobal);					\
	auto& item = mEGlobal.get();								\
	*b = item.##a.Pop();										\
	return DOSIERUNG_OK;										\
}



#define IMPLEMENT_EXGETSTRING(T, a)									\
 int32_t CDose :: EXGet##a	( T& sz) const							\
{																	\
	const auto& item = mEGlobal.get();								\
	sz = item.##a.Get().sz;											\
	return DOSIERUNG_OK;											\
}



#define IMPLEMENT_EXGETCHANGED(a)										\
 int32_t CDose :: EXGetChanged##a	( BOOL *b) const					\
{																		\
	assert(b);															\
	const auto& item = mEGlobal.get();									\
	*b = item.##a.IsChanged();											\
	return DOSIERUNG_OK;												\
}



#define IMPLEMENT_EXSET(t, a)									\
 int32_t CDose :: EXSet##a	( const t b)						\
{																\
	auto& item = mEGlobal.get();								\
	if ( item.##a.Test(b) )										\
	{															\
		std::lock_guard<std::mutex> lock(mEGlobal);				\
		item.##a.Set(b);										\
	}															\
	return DOSIERUNG_OK;										\
}																


#define IMPLEMENT_EXSETSTRING(T, C, a)								\
 int32_t CDose :: EXSet##a	( const T& sz)			\
{																\
	C s(sz);											\
	auto& item = mEGlobal.get();								\
	if ( item.##a.Test(s) )										\
	{															\
		std::lock_guard<std::mutex> lock(mEGlobal);				\
		item.##a.Set(s);										\
	}															\
	return DOSIERUNG_OK;										\
}																



#define IMPLEMENT_EXSETCHANGED( a)										\
 int32_t CDose :: EXSetChanged##a	( const BOOL b)						\
{																		\
	std::lock_guard<std::mutex> lock(mEGlobal);							\
	auto& item = mEGlobal.get();										\
	item.##a.SetChanged(b);												\
	return DOSIERUNG_OK;												\
}


#define IMPLEMENT_INLINE_EX(t, a)							\
		IMPLEMENT_EXINIT(t, a)								\
		IMPLEMENT_EXGET(t, a)								\
		IMPLEMENT_EXPOP(t, a)								\
		IMPLEMENT_EXSET(t, a)								\
		IMPLEMENT_EXGETCHANGED(a)							\
		IMPLEMENT_EXSETCHANGED(a)


#define IMPLEMENT_INLINE_EX_GERNERICSTRING(T, B, a)				\
		IMPLEMENT_EXGETSTRING(T, a)						\
		IMPLEMENT_EXSETSTRING(T, B, a)		\
		IMPLEMENT_EXGETCHANGED(a)								\
		IMPLEMENT_EXSETCHANGED(a)

#define IMPLEMENT_INLINE_EX_STRING(a)								\
		IMPLEMENT_INLINE_EX_GERNERICSTRING(std::string, STRINGCHARS, a)

#define IMPLEMENT_INLINE_EX_TSTRING(a)								\
		IMPLEMENT_INLINE_EX_GERNERICSTRING(std::string, TSTRINGCHARS, a)


#define IMPLEMENT_INLINE_EXPB(a)								\
		IMPLEMENT_EXPBGET(a)									\
		IMPLEMENT_EXPBSET(a)									\
        IMPLEMENT_EXPBRESET(a)


