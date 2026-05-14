//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ECDeclareMacro
///
/// @file   ECDeclareMacro.h
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

#define DECLARE_ECGET(t, a)			int32_t ECGet##a			    ( const int32_t n, t * ) const;	
#define DECLARE_ECPOP(t, a)			int32_t ECPop##a			    ( const int32_t n, t * );	
#define DECLARE_ECSET(t, a)			int32_t ECSet##a			    ( const int32_t n, const t );
#define DECLARE_ECADD(t, a)			int32_t ECAdd##a			    ( const int32_t n, const t );
#define DECLARE_ECINIT(t, a)		int32_t ECInit##a				( const int32_t n, const t);	
#define DECLARE_ECGETSTRING(T, a)	int32_t ECGet##a			    ( const int32_t n, T& ) const;	
#define DECLARE_ECSETSTRING(T, a)	int32_t ECSet##a			    ( const int32_t n, const T& );
#define DECLARE_ECINITSTRING(a)		int32_t ECInitString##a	    ( const int32_t n);
#define DECLARE_ECGETCHANGED(a)		int32_t ECGetChanged##a	    ( const int32_t n, BOOL * ) const;	
#define DECLARE_ECSETCHANGED(a)		int32_t ECSetChanged##a	    ( const int32_t n, const BOOL );


#define DECLARE_ECPBGET(a)			int32_t ECPBGet##a				( const int32_t n, BOOL *) const;
#define DECLARE_ECPBSET(a)			int32_t ECPBSet##a				( const int32_t n, const BOOL );
#define DECLARE_ECPBRESET(a)	    int32_t ECPBReset##a			( const int32_t n);


#define DECLARE_DOSEDATA_STRING(T, a)							\
		DECLARE_ECGETSTRING(T, a)									\
		DECLARE_ECSETSTRING(T, a)									\
		DECLARE_ECINITSTRING(a)                                \
		DECLARE_ECSETCHANGED(a)								\
		DECLARE_ECGETCHANGED(a)                                \


#define DECLARE_DOSEDATAPB(a)								\
		DECLARE_ECPBGET(a)									\
		DECLARE_ECPBSET(a)									\
		DECLARE_ECPBRESET(a)

#define DECLARE_DOSEDATA(t, a)								\
		DECLARE_ECGET(t, a)									\
	    DECLARE_ECPOP(t, a)									\
		DECLARE_ECSET(t, a)									\
		DECLARE_ECADD(t, a)									\
		DECLARE_ECINIT(t, a)								    \
		DECLARE_ECGETCHANGED(a)								\
		DECLARE_ECSETCHANGED(a)											


#define DECLARE_ECGETFIELDALARM(a)     int32_t ECGetField##a		( const int32_t n, base::eAlarmErrorBits& ul) const;
#define DECLARE_ECDELETEFIELDALARM(a)  int32_t ECDeleteField##a	( const int32_t n);
#define DECLARE_ECSETFIELDALARM(a)     int32_t ECSetField##a		( const int32_t n, const base::eAlarmErrorBits& ul);
#define DECLARE_ECISANYALARM(a)        int32_t ECIsAny##a			( const int32_t n, BOOL *b) const;
#define DECLARE_ECISALARM(a)           int32_t ECIs##a	        ( const int32_t n, const base::eAlarmError l, BOOL *b) const;
#define DECLARE_ECDELETEALARM(a)       int32_t ECDelete##a	    ( const int32_t n, const base::eAlarmError l);
#define DECLARE_ECSETALARM(a)          int32_t ECSet##a	        ( const int32_t n, const base::eAlarmError l);
#define DECLARE_ECGETALARM(a)          int32_t ECGet##a	        ( const int32_t n,  base::eAlarmError *l) const;


#define DECLARE_DOSEDATAALARM(a)	                                            \
        DECLARE_ECDELETEALARM(a)									            \
        DECLARE_ECSETALARM(a)									                \
        DECLARE_ECGETALARM(a)									                \
        DECLARE_ECISALARM(a)									                \
        DECLARE_ECISANYALARM(a)									            \
        DECLARE_ECSETFIELDALARM(a)									            \
        DECLARE_ECDELETEFIELDALARM(a)									        \
        DECLARE_ECGETFIELDALARM(a)



