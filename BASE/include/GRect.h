//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module GRect
///
/// @file   GRect.h
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
#include "GPoint.h"

namespace base
{
	template<typename T>
	class GRect
	{
	public:
		GPoint<T> topleft;
		GPoint<T> bottomright;

		GRect(GPoint<T> tl = 0, GPoint<T> br = 0) :topleft(tl), bottomright(br)
		{}
		GRect(const GRect&) = default;
		GRect& operator = (const GRect&) = default;

		GRect& operator -= (const GRect& p)
		{
			topleft		-= p.topleft;
			bottomright -= p.bottomright;
			return *this;
		}
		GRect& operator += (const GRect& p)
		{
			topleft		+= p.topleft;
			bottomright += p.bottomright;
			return *this;
		}
		GRect operator + (const GRect& p) const
		{
			return ::operator += (p);
		}
		GRect operator - (const GRect& p) const
		{
			return ::operator -= (p);
		}
		T width() const
		{
			return std::abs(bottomright.x - topleft.x);
		}
		T height() const
		{
			return std::abs(bottomright.y - topleft.y);
		}

		BOOL operator == (const GRect& p) const
		{	return p.topleft == topleft && p.bottomright == bottomright;		}

		BOOL operator != (const GRect& p) const
		{	return ! operator == (p);	}

	};
	using FRect = GRect<float32_t>;
	using IRect = GRect<int32_t>;

};


