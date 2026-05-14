//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ExControlMakro.h
///
/// @file   ExControlMakro.h
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

#define DECLARE_EXINIT(t, a)		int32_t EXInit##a			( const t );	
#define DECLARE_EXGET(t, a)			int32_t EXGet##a			( t * ) const;	
#define DECLARE_EXPOP(t, a)			int32_t EXPop##a			( t * );	
#define DECLARE_EXSET(t, a)			int32_t EXSet##a			( const t );
#define DECLARE_EXGETSTRING(T, a)	int32_t EXGet##a			( T& ) const;	
#define DECLARE_EXSETSTRING(T, a)	int32_t EXSet##a			( const T& );
#define DECLARE_EXGETCHANGED(a)		int32_t EXGetChanged##a		(  BOOL * ) const;	
#define DECLARE_EXSETCHANGED(a)		int32_t EXSetChanged##a		( const BOOL );


#define DECLARE_INLINE_EX(t, a)								\
		DECLARE_EXINIT(t, a)								\
		DECLARE_EXGET(t, a)									\
		DECLARE_EXPOP(t, a)									\
		DECLARE_EXSET(t, a)									\
		DECLARE_EXSETCHANGED(a)								\
		DECLARE_EXGETCHANGED(a)

#define DECLARE_INLINE_EX_STRING(T, a)							\
		DECLARE_EXGETSTRING(T, a)								\
		DECLARE_EXSETSTRING(T, a)								\
		DECLARE_EXSETCHANGED(a)								\
		DECLARE_EXGETCHANGED(a)


//-----------------------------------------------------------------------------------------------
#define DECLARE_EXPBGET(a)	        int32_t EXPBGet##a	        (  BOOL *) const;
#define DECLARE_EXPBSET(a)	        int32_t EXPBSet##a	        ( const BOOL b);
#define DECLARE_EXPBRESET(a)	    int32_t EXPBReset##a	    ( void );



#define DECLARE_INLINE_EXPB(a)									\
		DECLARE_EXPBGET(a)										\
		DECLARE_EXPBSET(a)										\
		DECLARE_EXPBRESET(a)




