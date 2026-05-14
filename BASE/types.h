//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module types
///
/// @file   types.h
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

#include <stdint.h>
typedef int BOOL;
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif


typedef float float32_t;
typedef double float64_t;
typedef char char_t;
typedef uint8_t BYTE;
typedef BYTE BIT;
typedef unsigned long ULONG;
typedef unsigned short WORD;
typedef long LONG;

#ifndef LPCSTR
typedef const char* LPCSTR;
#endif
#ifndef LPSTR
typedef char* LPSTR;
#endif


#ifndef PVOID
typedef void* PVOID;
#endif
#ifdef __cplusplus
#define _U64(_a) static_cast<uint64_t>(_a)
#define _U32(_a) static_cast<uint32_t>(_a)
#define _U16(_a) static_cast<uint16_t>(_a)
#define _U8(_a)  static_cast<uint8_t>(_a)
#define _S64(_a) static_cast<int64_t>(_a)
#define _S32(_a) static_cast<int32_t>(_a)
#define _S16(_a) static_cast<int16_t>(_a)
#define _S8(_a)  static_cast<int8_t>(_a)
#define _C(_a)   static_cast<char_t>(_a)
#define _F32(_a) static_cast<float32_t>(_a)
#define _F64(_a) static_cast<float64_t>(_a)
#else
#define _U64(_a) ((uint64_t)(_a))
#define _U32(_a) ((uint32_t)(_a))
#define _U16(_a) ((uint16_t)(_a))
#define _U8(_a)  ((uint8_t)(_a))
#define _S64(_a) ((int64_t)(_a))
#define _S32(_a) ((int32_t)(_a))
#define _S16(_a) ((int16_t)(_a))
#define _S8(_a)  ((int8_t)(_a))
#define _C(_a)   ((char_t)(_a))
#define _F32(_a) ((float32_t>(_a))
#define _F64(_a) ((float64_t>(_a))
#endif
