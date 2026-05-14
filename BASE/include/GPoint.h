//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module 2DTrapez.h
///
/// @file   2DTrapez.h
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


namespace base
{
	template<typename T>
	class GPoint
	{
	public:
		T x;
		T y;

		GPoint(T _x = 0, T _y = 0) :x(_x), y(_y)
		{}
		GPoint(const GPoint&) = default;
		GPoint& operator = (const GPoint& p) = default;

		GPoint& operator -= (const GPoint& p)
		{
			x -= p.x;
			y -= p.y;
			return *this;
		}
		GPoint& operator += (const GPoint& p)
		{
			x += p.x;
			y += p.y;
			return *this;
		}
		GPoint<T> operator + (const GPoint& p) const
		{
			return GPoint {x + p.x, y + p.y};
		}
		GPoint operator - (const GPoint& p) const
		{
			return GPoint{ x - p.x, y - p.y };
		}
		BOOL operator == (const GPoint& p) const
		{
			return BOOL((x == p.x) && (y == p.y));
		}
		BOOL operator != (const GPoint& p) const
		{
			return !operator == (p);
		}

		operator GPoint<int32_t>() const;
		operator GPoint<float32_t>() const;
		operator GPoint<float64_t>() const;


		GPoint& rotate(int32_t angle)
		{
			GPoint p(*this);
			if (angle != 0)
			{
				if (angle == 90)
				{
					x = p.y;
					y = -p.x;
				}
				else if (angle == -90)
				{
					x = -p.y;
					y = p.x;
				}
				else
				{
				}
			}
			return *this;
		}

	};

	using FPoint = GPoint<float32_t>;
	using IPoint = GPoint<int32_t>;


	template <>
	inline GPoint<float32_t> :: operator GPoint<int32_t>() const
	{
		return GPoint<int32_t>(NEXTLONG(x), NEXTLONG(y));
	}
	template <>
	inline GPoint<float64_t> :: operator GPoint<int32_t>() const
	{
		return GPoint<int32_t>(NEXTLONG(x), NEXTLONG(y));
	}
	template <typename T>
	inline GPoint<T> :: operator GPoint<int32_t>() const
	{
		return GPoint<int32_t>(x, y);
	}
	template <>
	inline GPoint<int32_t> :: operator GPoint<float32_t>() const
	{
		return GPoint<float32_t>(_F32(x), _F32(y));
	}
	template <>
	inline GPoint<int32_t> :: operator GPoint<float64_t>() const
	{
		return GPoint<float64_t>(_F64(x), _F64(y));
	}

};
