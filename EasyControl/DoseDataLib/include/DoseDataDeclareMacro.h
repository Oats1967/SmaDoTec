//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseDataDeclareMakro
///
/// @file   DoseDataDeclareMakro.h
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

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------

#define DECLARE_DOSEEX_INIT(t, a)										\
int32_t Dose_EXInit##a	(const t);									\

#define DECLARE_DOSEEX_GET(t, a)										\
int32_t Dose_EXGet##a	( t *);			\

#define DECLARE_DOSEEX_POP(t, a)										\
int32_t Dose_EXPop##a	( t *);			\

#define DECLARE_DOSEEX_SET(t, a)										\
int32_t Dose_EXSet##a	( const t);		\

#define DECLARE_DOSEEX_ADD(t, a)										\
int32_t Dose_EXAdd##a	( const t);		\

#define DECLARE_DOSEEX_GETCHANGED(a)										\
int32_t Dose_EXGetChanged##a	( BOOL *);			\

#define DECLARE_DOSEEX_SETCHANGED(a)										\
int32_t Dose_EXSetChanged##a	( const BOOL);		\

#define DECLARE_DOSEEX_GETSTRING(T, a)									\
int32_t Dose_EXGet##a	( T& );			\

#define DECLARE_DOSEEX_SETSTRING(T, a)									\
int32_t Dose_EXSet##a	 ( const T& );		\


#define DECLARE_DOSEEX(t, a)					\
	DECLARE_DOSEEX_INIT(t, a)					\
	DECLARE_DOSEEX_GET(t, a)					\
	DECLARE_DOSEEX_POP(t, a)					\
	DECLARE_DOSEEX_SET(t, a)					\
	DECLARE_DOSEEX_GETCHANGED(a)				\
	DECLARE_DOSEEX_SETCHANGED(a)			

#define DECLARE_DOSEEX_STRING(T, a)				\
	DECLARE_DOSEEX_GETSTRING(T, a)					\
	DECLARE_DOSEEX_SETSTRING(T, a)					\
	DECLARE_DOSEEX_GETCHANGED(a)				\
	DECLARE_DOSEEX_SETCHANGED(a)

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------

#define DECLARE_DOSEDSV_GET(t,a)											\
 int32_t Dose_DSVGet##a	( const int32_t n, t *);				

#define DECLARE_DOSEDSV_POP(t,a)											\
 int32_t Dose_DSVPop##a	( const int32_t n, t *);				

#define DECLARE_DOSEDSV_GETCHANGED(a)										\
 int32_t Dose_DSVGetChanged##a	( const int32_t n, BOOL *);				

#define DECLARE_DOSEDSV_SET(t,a)											\
 int32_t Dose_DSVSet##a		( const int32_t n, const t);			

#define DECLARE_DOSEDSV_ADD(t,a)											\
 int32_t Dose_DSVAdd##a		( const int32_t n, const t);			

#define DECLARE_DOSEDSV_INIT(t, a)											\
 int32_t Dose_DSVInit##a		( const int32_t n, const t);			

#define DECLARE_DOSEDSV_SETCHANGED(a)										\
 int32_t Dose_DSVSetChanged##a	( const int32_t n, const BOOL);			

#define DECLARE_DOSEDSV_GETSTRING(T, a)										\
 int32_t Dose_DSVGet##a	( const int32_t n, T& );			

#define DECLARE_DOSEDSV_SETSTRING(T, a)										\
 int32_t Dose_DSVSet##a	( const int32_t n, const T& );	

#define DECLARE_DOSEDSV_INITSTRING(a)										\
 int32_t Dose_DSVInitString##a	( const int32_t n);	

#define DECLARE_DOSEDSV(t, a)				\
	DECLARE_DOSEDSV_INIT(t, a)					\
	DECLARE_DOSEDSV_GET(t, a)			    \
	DECLARE_DOSEDSV_POP(t, a)			    \
	DECLARE_DOSEDSV_SET(t, a)			    \
	DECLARE_DOSEDSV_GETCHANGED(a)			\
	DECLARE_DOSEDSV_SETCHANGED(a)

// DECLARE_DOSEDSV_ADD(t, a)			    \

#define DECLARE_DOSEDSV_STRING(T, a)			\
	DECLARE_DOSEDSV_GETSTRING(T, a)			\
	DECLARE_DOSEDSV_SETSTRING(T, a)			\
	DECLARE_DOSEDSV_GETCHANGED(a)			\
	DECLARE_DOSEDSV_SETCHANGED(a)			\

//--------------------------------------------------------------------------------------------

#define DECLARE_DOSEEXPB_GET(a)											\
 int32_t Dose_EXPBGet##a	(  BOOL *);				

#define DECLARE_DOSEEXPB_RESET(a)											\
 int32_t Dose_EXPBReset##a		( void );			

#define DECLARE_DOSEEXPB_SET(a)											\
 int32_t Dose_EXPBSet##a		( const BOOL value = TRUE);			


#define DECLARE_DOSEEXPB(a)				                \
	DECLARE_DOSEEXPB_GET(a)			                    \
	DECLARE_DOSEEXPB_RESET(a)			                \
	DECLARE_DOSEEXPB_SET(a)			                    \


//--------------------------------------------------------------------------------------------
#define DECLARE_DOSEDSVGETFIELDALARM(a)                                   \
 int32_t Dose_DSVGetField##a	( const int32_t n, base::eAlarmErrorBits& ul);

#define DECLARE_DOSEDSVDELETEFIELDALARM(a)                                \
 int32_t Dose_DSVDeleteField##a	( const int32_t n);

#define DECLARE_DOSEDSVSETFIELDALARM(a)                                   \
 int32_t Dose_DSVSetField##a	( const int32_t n, const base::eAlarmErrorBits& ul);

#define DECLARE_DOSEDSVISANYALARM(a)                                      \
 int32_t Dose_DSVIsAny##a	    ( const int32_t n, BOOL *b);

#define DECLARE_DOSEDSVISALARM(a)                                         \
 int32_t Dose_DSVIs##a	        ( const int32_t n, const base::eAlarmError l, BOOL *b);

#define DECLARE_DOSEDSVDELETEALARM(a)                                     \
 int32_t Dose_DSVDelete##a	    ( const int32_t n, const base::eAlarmError l);

#define DECLARE_DOSEDSVSETALARM(a)                                        \
 int32_t Dose_DSVSet##a	    ( const int32_t n, const base::eAlarmError l);

#define DECLARE_DOSEDSVGETALARM(a)                                        \
 int32_t Dose_DSVGet##a	    ( const int32_t n, base::eAlarmError *l);


#define DECLARE_DOSEDSVALARM(a)                                               \
        DECLARE_DOSEDSVGETFIELDALARM(a)                                       \
        DECLARE_DOSEDSVDELETEFIELDALARM(a)                                    \
        DECLARE_DOSEDSVSETFIELDALARM(a)                                       \
        DECLARE_DOSEDSVISANYALARM(a)                                          \
        DECLARE_DOSEDSVISALARM(a)                                             \
        DECLARE_DOSEDSVDELETEALARM(a)                                         \
        DECLARE_DOSEDSVSETALARM(a)                                            \
        DECLARE_DOSEDSVGETALARM(a)


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
#define DECLARE_DOSEDSVPB_GET(a)	int32_t Dose_DSVPBGet##a( const int32_t n, BOOL *);			
#define DECLARE_DOSEDSVPB_SET(a)	int32_t Dose_DSVPBSet##a( const int32_t n,  BOOL bValue = TRUE);			
#define DECLARE_DOSEDSVPB_RESET(a)	int32_t Dose_DSVPBReset##a( const int32_t n);			

#define DECLARE_DOSEDSVPB(a)				                \
	DECLARE_DOSEDSVPB_GET(a)			                    \
	DECLARE_DOSEDSVPB_RESET(a)			                \
	DECLARE_DOSEDSVPB_SET(a)



