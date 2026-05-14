//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module 2DTrapeze
///
/// @file   2DTrapeze.h
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

#include <cstdlib>
#include <cmath>
#include <array>
#include <utility>
#include "BASE/Base.h"
#include "BASE/include/GPoint.h"

namespace base
{
	class C2DTrapeze
	{
	public:
		static const uint32_t c_MaxPoints = 4;
		using LineType = std::pair<float32_t, float32_t>;
		using TrapezePoints = std::array<IPoint, c_MaxPoints>;

	private:
		enum
		{
			M_POS = 0,
			B_POS
		};

		TrapezePoints m_points;
		FPoint m_S;

		float32_t m_m0;
		float32_t m_m1;
		float32_t m_mAbs;
		float32_t m_As;
		float32_t m_At;

	private:
		static LineType Calc(const IPoint& p0, const IPoint& p1);
		static FPoint Intersection(const LineType& l0, const LineType& l1);
		static float32_t GetArea(const FPoint& p0, const FPoint& p1);
		FPoint P2Q(const FPoint& p) const;
		FPoint Q2P(const FPoint& p) const;

		C2DTrapeze() = delete;

	public:
		explicit C2DTrapeze(const TrapezePoints& points) : m_points { points }
			, m_S{ Intersection(Calc(points[0], points[3]), Calc(points[1], points[2])) }
			, m_m0(0.0F)
			, m_m1(0.0F)
			, m_mAbs(0.0F)
			, m_As { GetArea(P2Q(points[2]), P2Q(points[3])) }
			, m_At(0.0F)
		{
			// Schnittpunkt berechnen
			auto q0 = P2Q(points[0]);
			auto q1 = P2Q(points[1]);
			auto Ag = GetArea(q0, q1);
			m_At = Ag - m_As;

			m_m0 = q0.y / q0.x;
			m_m1 = q1.y / q1.x;
			m_mAbs = std::abs(m_m0) + std::abs(m_m1);
		}


		void GetFilling(const float32_t filling, TrapezePoints& points, uint32_t& count) const;

		float32_t GetA() const
		{	return m_At;		}

		const IPoint& GetP(const int32_t index) const
		{ return m_points[index]; 	}

		const IPoint& operator[](const int32_t index) const
		{	return m_points[index];	}

	};
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	inline	void C2DTrapeze::GetFilling(const float32_t filling, TrapezePoints& points, uint32_t& count) const
	{
		const float32_t x = _F32(sqrt((2*(m_As + filling * m_At)) / m_mAbs));

		points[0] = Q2P(FPoint{ x, m_m0 * x });
		points[1] = Q2P(FPoint{ x, m_m1 * x });
		points[2] = m_points[2];
		points[3] = m_points[3];
		count = 4;
	}
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	inline float32_t C2DTrapeze::GetArea(const FPoint& p0, const FPoint& p1)
	{
		return (std::abs(p0.y * p0.x) + std::abs(p1.y * p1.x)) / 2.0F;
	}
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	inline	C2DTrapeze::LineType C2DTrapeze::Calc(const IPoint& p0, const IPoint& p1)
	{
		const int32_t x0 = p0.x;
		const int32_t y0 = p0.y;
		const int32_t x1 = p1.x;
		const int32_t y1 = p1.y;

		const float32_t m = _F32(y1 - y0) / _F32(x1 - x0);
		const float32_t b = y1 - m * x1;
		return std::move(std::make_pair(m, b));
	}
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	inline	FPoint C2DTrapeze::Intersection(const LineType& l0, const LineType& l1)
	{
		const auto& m0 = std::get<M_POS>(l0);
		const auto& b0 = std::get<B_POS>(l0);
		const auto& m1 = std::get<M_POS>(l1);
		const auto& b1 = std::get<B_POS>(l1);

		const float32_t x = (b1 - b0) / (m0 - m1);
		return FPoint{ x, m0 * x + b0 };
	}
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	inline	FPoint C2DTrapeze::P2Q(const FPoint& p) const
	{
		FPoint k = p - m_S;
		k.rotate(90);
		return std::move(k);
	}
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	inline	FPoint C2DTrapeze::Q2P(const FPoint& q) const
	{
		auto k = q;
		k.rotate(-90);
		k += m_S;
		return k;
	}


};