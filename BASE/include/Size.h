//------------------------------------------------------------------------------------
///
///                           <<<   E A S Height C O N T R O L   >>>
///
///
/// @brief  Declaration of module Size
///
/// @file   Size.h
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

#include "BASE/types.h"

namespace base
{
template <typename T>
class CSize
{
	T cx;
	T cy;

public:
	CSize() :cx{ 0 }, cy{ 0 }
	{}

	CSize(T _x, T _y) :cx{ _x }, cy{ _y }
	{}

	T& Width() { return cx; }
	const T& Width() const { return cx; }

	T& Height() { return cy; }
	const T& Height() const { return cy; }

	BOOL operator == (const CSize<T>& p) const
	{	return (cx == p.cx) && (cy == p.cy);}

	BOOL operator != (const CSize<T>& p) const
	{	return !(*this == (p));	}

};

using INTSIZE = CSize<int32_t>;
using FLOATSIZE = CSize<float32_t>;
using DOUBLETSIZE = CSize<float32_t>;

};