//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ECImplMacro.h
///
/// @file   ECImplMacro.h
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
#define IMPLEMENT_EC_GET(t, a)									\
 int32_t CDose :: ECGet##a	( const int32_t n, t *b) const		\
{																\
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);					\
	assert(b);													\
	const auto& item = mDGlobal[n].get();						\
	*b = item.##a.Get();										\
	return DOSIERUNG_OK;										\
}									

#define IMPLEMENT_EC_POP(t, a)									\
 int32_t CDose :: ECPop##a	( const int32_t n, t *b)		    \
{																\
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);					\
	assert(b);													\
	std::lock_guard<std::mutex> lock(mDGlobal[n]);				\
	auto& item = mDGlobal[n].get();								\
	*b = item.##a.Pop();										\
	return DOSIERUNG_OK;										\
}									


#define IMPLEMENT_EC_GETSTRING( T, a)										\
int32_t CDose :: ECGet##a	( const int32_t n, T& sz) const	\
{																		\
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);							\
	const auto& item = mDGlobal[n].get();								\
	const auto& s = item.##a.Get();										\
	sz = s.sz;															\
	return DOSIERUNG_OK;												\
}	



#define IMPLEMENT_EC_GETCHANGED(a)								\
 int32_t CDose :: ECGetChanged##a	( const int32_t n, BOOL *b) const	\
{																\
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);					\
	assert(b);													\
	const auto& item = mDGlobal[n].get();						\
	*b = item.##a.IsChanged();									\
	return DOSIERUNG_OK;										\
}																

#define IMPLEMENT_EC_SET(t, a)									\
 int32_t CDose :: ECSet##a	( const int32_t n, const t b)		\
{																\
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);					\
	auto& item  = mDGlobal[n].get();							\
	if (item.##a.Test(b))										\
	{															\
		std::lock_guard<std::mutex> lock(mDGlobal[n]);			\
		item.##a.Set(b);										\
    }															\
	return DOSIERUNG_OK;										\
}																

#define IMPLEMENT_EC_INIT(t, a)									\
 int32_t CDose :: ECInit##a	( const int32_t n, const t b)		\
{																\
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);					\
	std::lock_guard<std::mutex> lock(mDGlobal[n]);				\
	auto& item = mDGlobal[n].get();								\
	item.##a.Init(b);											\
	return DOSIERUNG_OK;										\
}																

#define IMPLEMENT_EC_SETSTRING(T, C, a)								\
 int32_t CDose :: ECSet##a	( const int32_t n, const T& rsz)		\
{																	\
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);						\
	C s(rsz);														\
	auto& item  = mDGlobal[n].get();								\
	if (item.##a.Test(s))											\
	{																\
		std::lock_guard<std::mutex> lock(mDGlobal[n]);				\
		item.##a.Set(s);											\
	}																\
	return DOSIERUNG_OK;											\
}																


#define IMPLEMENT_EC_INITSTRING(C, a)								\
 int32_t CDose :: ECInitString##a	( const int32_t n)	            \
{																	\
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);						\
	C s;															\
	std::lock_guard<std::mutex> lock(mDGlobal[n]);					\
	auto& item = mDGlobal[n].get();									\
	item.##a.Init(s);												\
	return DOSIERUNG_OK;											\
}																

#define IMPLEMENT_EC_SETCHANGED(a)								        \
 int32_t CDose :: ECSetChanged##a	( const int32_t n, const BOOL b)	\
{																        \
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);					        \
	std::lock_guard<std::mutex> lock(mDGlobal[n]);						\
	auto& item = mDGlobal[n].get();										\
	item.##a.SetChanged(b);												\
	return DOSIERUNG_OK;										        \
}																


//---------------------------------------------------------------------------------------------------

#define IMPLEMENT_EC_GERNERICSTRING(T, C, a)  \
	IMPLEMENT_EC_GETSTRING(T, a)  \
	IMPLEMENT_EC_SETSTRING(T, C, a)   \
	IMPLEMENT_EC_INITSTRING(C, a)  \
	IMPLEMENT_EC_SETCHANGED(a)  \
	IMPLEMENT_EC_GETCHANGED(a)

#define IMPLEMENT_EC_TSTRING(a)  \
	IMPLEMENT_EC_GERNERICSTRING(std::string, TSTRINGCHARS, a)

#define IMPLEMENT_EC_STRING(a)  \
	IMPLEMENT_EC_GERNERICSTRING(std::string, STRINGCHARS, a)

//-----------------------------------------------------------------------------------------------------------------------

#define IMPLEMENT_EC(t, a) \
	IMPLEMENT_EC_GET(t, a) \
	IMPLEMENT_EC_SET(t, a) \
	IMPLEMENT_EC_POP(t, a) \
	IMPLEMENT_EC_INIT(t, a) \
	IMPLEMENT_EC_GETCHANGED(a) \
	IMPLEMENT_EC_SETCHANGED(a)										


//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
#define IMPLEMENT_EC_PBGET(a)									    \
int32_t CDose :: ECPBGet##a	( const int32_t n, BOOL *b) const		\
{																    \
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);					    \
	assert(b);														\
	const auto& item = mDGlobal[n].get();							\
	*b = item.##a;													\
	return DOSIERUNG_OK;										    \
}									


#define IMPLEMENT_EC_PBSET(a)									    \
int32_t CDose :: ECPBSet##a	( const int32_t n, const BOOL b)		\
{																    \
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);					    \
	auto& item = mDGlobal[n].get();									\
	if (item.##a != b)												\
	{																\
		std::lock_guard<std::mutex> lock(mDGlobal[n]);				\
		item.##a = b;												\
    }																\
	return DOSIERUNG_OK;										    \
}									

#define IMPLEMENT_EC_PBRESET(a)										\
int32_t CDose :: ECPBReset##a	( const int32_t n)                  \
{	return ECPBSet##a(n, FALSE);	}									


#define IMPLEMENT_EC_PB(a)	 \
			IMPLEMENT_EC_PBGET(a)   \
			IMPLEMENT_EC_PBRESET(a)   \
			IMPLEMENT_EC_PBSET(a)

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
