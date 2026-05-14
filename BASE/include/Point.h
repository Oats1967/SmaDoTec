//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Point
///
/// @file   Point.h
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
	class CPoint
	{
		T x;
		T y;

	public:
		CPoint() :x{ 0 }, y{ 0 }
		{}

		CPoint(T _x, T _y) :x{ _x }, y{ _y }
		{}

		T& X() { return x; }
		const T& X() const { return x; }

		T& Y() { return y; }
		const T& Y() const { return y; }

		BOOL operator == (const CPoint<T>& p) const
		{ return (x == p.x) && (y == p.y);	}

		BOOL operator != (const CPoint<T>& p) const
		{return ! (*this == (p));	}
	};
	using INTPOINT = CPoint<int32_t>;
	using FLOATPOINT = CPoint<float32_t>;
	using DOUBLETPOINT = CPoint<float32_t>;
};
