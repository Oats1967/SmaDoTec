//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IfsMacros
///
/// @file   IfsMacros.hxx
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

#if _DEBUG

static const char* szStep[9] = { 
        "eInitReached",
        "eEmptyReached",
        "eLowReached",
        "eMinReached",
        "eMaxReached",
        "eHighReached",
        "eMaxOverflowReached",
        "eHighOverflowReached",
        "eEmptyOverflowReached"
};

static int32_t g_LastControlSteps = 0;

#define LOGFUNCTION(__s) \
{\
	{ \
        static float32_t OldValue = -1.0; \
        float32_t value = 0.0F; \
        Dose_EXGetLineSetpoint(&value); \
        if (value != OldValue) \
        { \
            OldValue = value; \
		    LOGDEBUG(__FUNCTION__ << " Step = " << szStep[_S32(__s)] <<  " Setpoint = " << value); \
	    }\
	}\
}

#define LOGFUNCTIONSTEP(__s) \
{\
    static float32_t OldValue = -1.0; \
    float32_t value = 0.0F; \
    Dose_EXGetLineSetpoint(&value); \
    if ((g_LastControlSteps != _S32(__s)) || (value != OldValue)) \
    { \
         g_LastControlSteps = _S32(__s); \
         OldValue = value; \
         LOGDEBUG(__FUNCTION__ << "(t = " << m_st << ") Step = " <<  szStep[_S32(__s)] << " Setpoint = " << value); \
	}\
}
#else
#define LOGFUNCTION(__s) 
#define LOGFUNCTIONSTEP(__s)
#endif


