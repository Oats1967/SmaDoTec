//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseServerMakro.h
///
/// @file   DoseServerMakro.h
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
#define IMPLEMENT_DOSEEX_INIT(t, a)						\
int32_t Dose_EXInit##a (const t b)						\
{														\
	assert(DOSEPOINTER);								\
	return DOSEREF.EXInit##a(b);							\
}

#define IMPLEMENT_DOSEEX_SET(t, a)						\
int32_t Dose_EXSet##a (const t b)						\
{														\
	assert(DOSEPOINTER);								\
	return DOSEREF.EXSet##a(b);							\
}



#define IMPLEMENT_DOSEEX_ADD(t, a)						\
int32_t Dose_EXAdd##a (const t b)						\
{														\
	assert(DOSEPOINTER);								\
	return DOSEREF.EXAdd##a(b);							\
}


#define IMPLEMENT_DOSEEX_SETCHANGED(a)					\
int32_t Dose_EXSetChanged##a (const BOOL b)			    \
{														\
	assert(DOSEPOINTER);										\
	return DOSEREF.EXSetChanged##a(b);					\
}


#define IMPLEMENT_DOSEEX_GET(t, a)						\
int32_t Dose_EXGet##a ( t *b)							\
{														\
	assert(DOSEPOINTER);										\
	return DOSEREF.EXGet##a(b);							\
}

#define IMPLEMENT_DOSEEX_POP(t, a)						\
int32_t Dose_EXPop##a ( t *b)							\
{														\
	assert(DOSEPOINTER);										\
	return DOSEREF.EXPop##a(b);							\
}



#define IMPLEMENT_DOSEEX_GETCHANGED(a)					\
int32_t Dose_EXGetChanged##a ( BOOL *b)					\
{														\
	assert(DOSEPOINTER);										\
	return DOSEREF.EXGetChanged##a(b);					\
}


#define IMPLEMENT_DOSEEX_SETSTRING(T, a)						\
int32_t Dose_EXSet##a		( const T& s)			\
{															\
	assert(DOSEPOINTER);											\
	return DOSEREF.EXSet##a(s);								\
}


#define IMPLEMENT_DOSEEX_GETSTRING(T, a)							\
int32_t Dose_EXGet##a	( T& s)						\
{																\
	assert(DOSEPOINTER);												\
	return DOSEREF.EXGet##a( s);									\
}

#define IMPLEMENT_DOSEEX(t, a)							\
	IMPLEMENT_DOSEEX_INIT(t, a)							\
	IMPLEMENT_DOSEEX_GET(t, a)							\
	IMPLEMENT_DOSEEX_POP(t, a)							\
	IMPLEMENT_DOSEEX_SET(t, a)							\
	IMPLEMENT_DOSEEX_GETCHANGED(a)						\
	IMPLEMENT_DOSEEX_SETCHANGED(a)			


#define IMPLEMENT_DOSEEX_STRING(T, a)						\
	IMPLEMENT_DOSEEX_GETSTRING(T, a)						\
	IMPLEMENT_DOSEEX_SETSTRING(T, a)						\
	IMPLEMENT_DOSEEX_GETCHANGED(a)						\
	IMPLEMENT_DOSEEX_SETCHANGED(a)			


//--------------------------------------------------------------------------------------------
#define IMPLEMENT_DOSEDSV_SET(T, a)						\
int32_t Dose_DSVSet##a (const int32_t n,  const T b)						\
{																	\
	assert(DOSEPOINTER);													\
	return DOSEREF.ECSet##a(n, b);							\
}																	



#define IMPLEMENT_DOSEDSV_ADD(T, a)						\
int32_t Dose_DSVAdd##a (const int32_t n,  const T b)						\
{																	\
	assert(DOSEPOINTER);													\
	return DOSEREF.ECAdd##a(n, b);							\
}																	


#define IMPLEMENT_DOSEDSV_INIT(T, a)						\
int32_t Dose_DSVInit##a (const int32_t n, const T b)			    \
{																	\
	assert(DOSEPOINTER);													\
	return DOSEREF.ECInit##a(n, b);								\
}																	


#define IMPLEMENT_DOSEDSV_SETCHANGED(a)						\
int32_t Dose_DSVSetChanged##a (const int32_t n,  const BOOL b)						\
{																	\
	assert(DOSEPOINTER);													\
	return DOSEREF.ECSetChanged##a(n, b);								\
}																	


#define IMPLEMENT_DOSEDSV_GET(T, a)						\
int32_t Dose_DSVGet##a (const int32_t n,  T *b)							\
{																	\
	assert(DOSEPOINTER);													\
	return DOSEREF.ECGet##a(n, b);									\
}



#define IMPLEMENT_DOSEDSV_POP(T, a)						\
int32_t Dose_DSVPop##a (const int32_t n,  T *b)							\
{																	\
	assert(DOSEPOINTER);													\
	return DOSEREF.ECPop##a(n, b);									\
}



#define IMPLEMENT_DOSEDSV_GETCHANGED(a)					\
int32_t Dose_DSVGetChanged##a (const int32_t n,  BOOL *b)					\
{																	\
	assert(DOSEPOINTER);													\
	return DOSEREF.ECGetChanged##a(n, b);									\
}


#define IMPLEMENT_DOSEDSV_SETSTRING(T, a)						\
int32_t Dose_DSVSet##a		( const int32_t n , const T& s)			\
{																	\
	assert(DOSEPOINTER);													\
	return DOSEREF.ECSet##a(n, s);							\
}

#define IMPLEMENT_DOSEDSV_INITSTRING(a)					\
int32_t Dose_DSVInitString##a		( const int32_t n )			            \
{																	\
	assert(DOSEPOINTER);													\
	return DOSEREF.ECInitString##a(n);						\
}


#define IMPLEMENT_DOSEDSV_GETSTRING(T, a)					\
int32_t Dose_DSVGet##a	( const int32_t n, T& s)					\
{																	\
	assert(DOSEPOINTER);													\
	return DOSEREF.ECGet##a( n, s);									\
}


#define IMPLEMENT_DOSEDSV(t, a)							\
	IMPLEMENT_DOSEDSV_GET(t, a)							\
	IMPLEMENT_DOSEDSV_POP(t, a)							\
	IMPLEMENT_DOSEDSV_SET(t, a)							\
	IMPLEMENT_DOSEDSV_INIT(t, a)							    \
	IMPLEMENT_DOSEDSV_GETCHANGED(a)						\
	IMPLEMENT_DOSEDSV_SETCHANGED(a)			            \


#define IMPLEMENT_DOSEDSV_STRING(T, a)					    \
	IMPLEMENT_DOSEDSV_GETSTRING(T, a)						\
	IMPLEMENT_DOSEDSV_SETSTRING(T, a)						\
	IMPLEMENT_DOSEDSV_INITSTRING(a)						\
	IMPLEMENT_DOSEDSV_GETCHANGED(a)					    \
	IMPLEMENT_DOSEDSV_SETCHANGED(a)			            \

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
#define IMPLEMENT_DOSEEXPB_SET(a)	\
int32_t Dose_EXPBSet##a (const BOOL b) \
{									\
	assert(DOSEPOINTER);			\
	return DOSEREF.EXPBSet##a(b);	\
}


#define IMPLEMENT_DOSEEXPB_RESET(a)	\
int32_t Dose_EXPBReset##a ( void )	\
{									\
	assert(DOSEPOINTER);			\
	return DOSEREF.EXPBReset##a();  \
}

#define IMPLEMENT_DOSEEXPB_GET( a)	\
int32_t Dose_EXPBGet##a (BOOL *b)	\
{									\
	assert(DOSEPOINTER);			\
	return DOSEREF.EXPBGet##a(b);	\
}

#define IMPLEMENT_DOSEEXPB(a)		\
	IMPLEMENT_DOSEEXPB_GET(a)		\
	IMPLEMENT_DOSEEXPB_RESET(a)		\
	IMPLEMENT_DOSEEXPB_SET(a)


#define IMPLEMENT_DOSEDSVGETFIELDALARM(a)                                     \
int32_t Dose_DSVGetField##a	( const int32_t n, base::eAlarmErrorBits& ul)                                   \
{																	                    \
	assert(DOSEPOINTER);													                    \
	return DOSEREF.ECGetField##a(n, ul);					                            \
}

#define IMPLEMENT_DOSEDSVDELETEFIELDALARM(a)                                  \
int32_t Dose_DSVDeleteField##a	( const int32_t n)                                          \
{                                                                                       \
	assert(DOSEPOINTER);													                    \
	return DOSEREF.ECDeleteField##a(n);							                \
}


#define IMPLEMENT_DOSEDSVDELETEALARM(a)                                       \
int32_t Dose_DSVDelete##a	    ( const int32_t n, const base::eAlarmError  l)          \
{                                                                                       \
	assert(DOSEPOINTER);													                    \
	return DOSEREF.ECDelete##a(n, l);							                    \
}


#define IMPLEMENT_DOSEDSVSETFIELDALARM(a)                                     \
int32_t Dose_DSVSetField##a	(const int32_t n, const base::eAlarmErrorBits& ul)                              \
{                                                                                       \
	assert(DOSEPOINTER);													                    \
	return DOSEREF.ECSetField##a(n, ul);							                \
}

#define IMPLEMENT_DOSEDSVISANYALARM(a)                                        \
int32_t Dose_DSVIsAny##a	    ( const int32_t n, BOOL *b)                                     \
{																	                    \
	assert(DOSEPOINTER);													                    \
	return DOSEREF.ECIsAny##a(n, b);					                                \
}


#define IMPLEMENT_DOSEDSVISALARM(a)                                             \
int32_t Dose_DSVIs##a	        ( const int32_t n, const base::eAlarmError l, BOOL *b)                       \
{																	                    \
	assert(DOSEPOINTER);													                    \
	return DOSEREF.ECIs##a(n, l, b);					                                \
}


#define IMPLEMENT_DOSEDSVSETALARM(a)                                          \
int32_t Dose_DSVSet##a	    ( const int32_t n, const base::eAlarmError  l)                                \
{                                                                                       \
	assert(DOSEPOINTER);													                    \
	return DOSEREF.ECSet##a(n, l);							                        \
}


#define IMPLEMENT_DOSEDSVGETALARM(a)                                          \
int32_t Dose_DSVGet##a	    ( const int32_t n, base::eAlarmError  *l )                                    \
{                                                                                       \
	assert(DOSEPOINTER);													                    \
	return DOSEREF.ECGet##a(n,l);							                            \
}



#define IMPLEMENT_DOSEDSVALARM(a)                                             \
        IMPLEMENT_DOSEDSVGETFIELDALARM(a)                                     \
        IMPLEMENT_DOSEDSVDELETEFIELDALARM(a)                                  \
        IMPLEMENT_DOSEDSVSETFIELDALARM(a)                                     \
        IMPLEMENT_DOSEDSVISANYALARM(a)                                        \
        IMPLEMENT_DOSEDSVISALARM(a)                                           \
        IMPLEMENT_DOSEDSVDELETEALARM(a)                                       \
        IMPLEMENT_DOSEDSVSETALARM(a)                                          \
        IMPLEMENT_DOSEDSVGETALARM(a)



//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
#define IMPLEMENT_DOSEDSVPB_GET(a)	\
int32_t Dose_DSVPBGet##a( const int32_t n, BOOL *pValue)			\
{																	\
	assert(DOSEPOINTER);											\
	return DOSEREF.ECPBGet##a(n, pValue);							\
}

#define IMPLEMENT_DOSEDSVPB_SET(a)	\
int32_t Dose_DSVPBSet##a( const int32_t n, BOOL value)				\
{																	\
	assert(DOSEPOINTER);											\
	return DOSEREF.ECPBSet##a(n, value);							\
}

#define IMPLEMENT_DOSEDSVPB_RESET(a)								\
int32_t Dose_DSVPBReset##a( const int32_t n)						\
{																	\
	assert(DOSEPOINTER);											\
	return DOSEREF.ECPBReset##a(n);								\
}


#define IMPLEMENT_DOSEDSVPB(a)				                \
	IMPLEMENT_DOSEDSVPB_GET(a)			                    \
	IMPLEMENT_DOSEDSVPB_RESET(a)			                \
	IMPLEMENT_DOSEDSVPB_SET(a)
