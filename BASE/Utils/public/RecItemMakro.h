//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecItemMakro.h
///
/// @file   RecItemMakro.h
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


//--------------------------------------------------------------------------------------------------------
#define IMPLEMENT_GETMAX(f)  float   CRecItem :: GetMax##f   (int id) const          \
{                                                       \
    float fMax = -FLT_MAX/2;                            \
    if ( id < 0 )                                       \
    {                                                   \
        for (int i = 0; i < GLOBALDOSEMAXCOUNT; i++)    \
        {                                               \
	        if ( m_aDose[i].m_f##f > fMax )             \
            {                                           \
                fMax = m_aDose[i].m_f##f;               \
            }                                           \
        }                                               \
    }                                                   \
    else                                                \
    {                                                   \
       ASSERT(id>= 0 && id < GLOBALDOSEMAXCOUNT);       \
       fMax = m_aDose[id].m_f##f;                       \
    }                                                   \
    return fMax;                                        \
}



//--------------------------------------------------------------------------------------------------------
#define IMPLEMENT_GETMIN(f)  float   CRecItem :: GetMin##f   (int id) const          \
{                                                       \
    float fMin = FLT_MAX;                               \
    if ( id < 0 )                                       \
    {                                                   \
        for (int i = 0; i < GLOBALDOSEMAXCOUNT; i++)    \
        {                                               \
	        if ( m_aDose[i].m_f##f < fMin )             \
            {                                           \
                fMin = m_aDose[i].m_f##f;               \
            }                                           \
        }                                               \
    }                                                   \
    else                                                \
    {                                                   \
       ASSERT(id>= 0 && id < GLOBALDOSEMAXCOUNT);       \
       fMin = m_aDose[id].m_f##f;                       \
    }                                                   \
    return fMin;                                        \
}


//------------------------------------------------------------------------------------------------------
#define IMPLEMENT_GETLISTMAX(f)  float CRecItemList :: GetMax##f   (int id) const  \
{                                                       \
    float fMax = -FLT_MAX/2;                            \
	for (POSITION aPos = GetHeadPosition(); aPos; )     \
	{                                                   \
		CRecItem& aItem = *GetNext(aPos);               \
		const float _f = aItem.GetMax##f(id);           \
        if ( _f > fMax )                                \
        {                                               \
            fMax = _f;                                  \
        }                                               \
	}                                                   \
    return fMax;                                        \
}


//------------------------------------------------------------------------------------------------------
#define IMPLEMENT_GETLISTMIN(f)  float CRecItemList :: GetMin##f   (int id) const \
{                                                       \
    float fMin = FLT_MAX;                               \
	for (POSITION aPos = GetHeadPosition(); aPos; )     \
	{                                                   \
		CRecItem& aItem = *GetNext(aPos);               \
		const float _f = aItem.GetMin##f(id);           \
        if ( _f < fMin )                                \
        {                                               \
            fMin = _f;                                  \
        }                                               \
	}                                                   \
    return fMin;                                        \
}

