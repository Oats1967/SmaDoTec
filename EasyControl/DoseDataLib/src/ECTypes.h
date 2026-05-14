//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ECTypes.h
///
/// @file   ECTypes.h
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

#include "BASE/Base.h"
#include "BASE/include/AlarmTypes.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
struct  WSTRINGCHARS
{
    static const int32_t MAXDOSIERUNGCHAR = 32;

    wchar_t sz[MAXDOSIERUNGCHAR];

    WSTRINGCHARS()
    {
        sz[0] = 0;
    }

    WSTRINGCHARS(const std::wstring& rsz)
    {
        const auto length = rsz.length();
        wcsncpy_s(sz, rsz.c_str(), __min(length, MAXDOSIERUNGCHAR - 1));
        sz[MAXDOSIERUNGCHAR - 1] = 0;
    }

    WSTRINGCHARS(const wchar_t* s)
    {
        const auto length = wcslen(s);
        wcsncpy_s(sz, s, __min(length, MAXDOSIERUNGCHAR - 1));
        sz[MAXDOSIERUNGCHAR - 1] = 0;
    }

    BOOL operator == (const WSTRINGCHARS& s) const
    {
        return BOOL(!wcscmp(s.sz, sz));
    }

    BOOL operator != (const WSTRINGCHARS& s) const
    {
        return !BOOL(*this == s);
    }

};
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
struct  STRINGCHARS
{
    static const int32_t MAXDOSIERUNGCHAR = 32;

    char sz[MAXDOSIERUNGCHAR];

    STRINGCHARS()
    {  sz[0] = 0;    }

    STRINGCHARS(const std::string& rsz)
    {
        const auto length = rsz.length();
        strncpy_s(sz, rsz.c_str(), __min(length, MAXDOSIERUNGCHAR - 1));
        sz[MAXDOSIERUNGCHAR - 1] = 0;
    }

    STRINGCHARS(const char* s)
    {  
        const auto length = strlen(s);
        strncpy_s(sz, s, __min(length, MAXDOSIERUNGCHAR - 1));
        sz[MAXDOSIERUNGCHAR - 1] = 0;
    }

    BOOL operator == (const STRINGCHARS& s) const
    {  return BOOL( ! strcmp(s.sz, sz));    }

    BOOL operator != (const STRINGCHARS& s) const
    {  return ! BOOL(*this == s);    }

};

#ifdef _UNICODE
#define TSTRINGCHARS WSTRINGCHARS
#else
#define TSTRINGCHARS STRINGCHARS
#endif


//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 8)
template <class T>
struct ECTypes
{
    T value;
    BOOL bChanged = FALSE;

    __forceinline void Set(const T& v)
    {
        assert(v != value);
        Add(v);
    }

    __forceinline void Init(const T& v)
    {
        bChanged = FALSE; value = v;
    }

    __forceinline void Add(const T& v)
    {
        bChanged = TRUE;   value = v;
    }

    __forceinline void Reset()
    {
        bChanged = FALSE;   value = T(0);
    }

    __forceinline void Delete()
    {
        bChanged = (value != 0);    value = T(0);
    }

    __forceinline const T& Get(void) const
    {
        return value;
    }

    __forceinline T& Get(void)
    {
        return value;
    }

    __forceinline const T& Pop(void)
    {
        bChanged = FALSE;  return value;
    }

    __forceinline const BOOL IsChanged(void) const
    {
        return bChanged;
    }

    __forceinline void SetChanged(BOOL changed)
    {
        bChanged = changed;
    }

    __forceinline BOOL Test(const T& newvalue) const
    {
        return BOOL(newvalue != value);
    }

    __forceinline BOOL operator != (const T& v) const
    {
        return BOOL(v != value);
    }

};
#pragma pack(pop)

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
template<>
__forceinline void ECTypes< STRINGCHARS >::Reset()
{
    bChanged = FALSE;   
    value.sz[0] = 0;
}

template<>
__forceinline void ECTypes< STRINGCHARS >::Delete()
{
    bChanged = value.sz[0] != 0;
    value.sz[0] = 0;
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
template<>
__forceinline void ECTypes< WSTRINGCHARS >::Reset()
{
    bChanged = FALSE;
    value.sz[0] = 0;
}

template<>
__forceinline void ECTypes< WSTRINGCHARS >::Delete()
{
    bChanged = value.sz[0] != 0;
    value.sz[0] = 0;
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
template<>
__forceinline void ECTypes< base::eAlarmErrorBits >::Reset()
{
    bChanged = FALSE;
    value.allflags = 0;
}

template<>
__forceinline void ECTypes< base::eAlarmErrorBits >::Delete()
{
    bChanged = value.allflags != 0;
    value.allflags = 0;
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
template<>
__forceinline void ECTypes< base::CAlarmCategories >::Reset()
{
    value.Reset();
    bChanged = FALSE;
}

template<>
__forceinline void ECTypes< base::CAlarmCategories >::Delete()
{
    bChanged = value.Delete();
}
