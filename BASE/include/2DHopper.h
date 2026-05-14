//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module 2DHopper.h
///
/// @file   2DHopper.h
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

#include <array>
#include "BASE/Base.h"
#include "GRect.h"
#include "2DTrapeze.h"

namespace base
{
class C2DHopper
{
public:
	static const uint32_t c_MaxPoints = 6;
	using HopperPoints = std::array<IPoint, c_MaxPoints>;

private:
	const IRect		 c_rect;
	const C2DTrapeze c_trapez;
	const float32_t	 c_Ar;
	const float32_t	 c_At;
	const float32_t	 c_Ag;
	HopperPoints	m_SavePoints;
	int32_t			m_lastFilling;
	uint32_t		m_count;

public:
	explicit C2DHopper(const HopperPoints& points) :
		c_rect(points[0], points[2])
		, c_trapez{ { points[5], points[2], points[3], points[4] } }
		, c_Ar {_F32( c_rect.width()* c_rect.height()) }
		, c_At { c_trapez.GetA() }
		, c_Ag { c_At + c_Ar }
		, m_SavePoints{ points }
		, m_lastFilling(-1)
		, m_count(0)
	{}
	C2DHopper() = delete;
	void GetFilling(const float32_t filling, HopperPoints&, uint32_t& count);
};
//**********************************************************************************************************************
inline	void C2DHopper::GetFilling(const float32_t filling, HopperPoints& points, uint32_t& count)
{
	const float32_t Ah = filling * c_Ag;
	const int32_t As = _S32(Ah);
	if (As != m_lastFilling)
	{
		m_lastFilling = As;
		if (Ah > c_At)
		{
			const float32_t Ar = Ah - c_At;
			const int32_t y    = __min(NEXTLONG(Ar / c_rect.width()), c_rect.height());
			m_SavePoints[0] = base::IPoint{ c_rect.topleft.x, c_rect.bottomright.y + y };
			m_SavePoints[1] = base::IPoint{ c_rect.bottomright.x, m_SavePoints[0].y };
			m_SavePoints[2] = c_trapez[1];
			m_SavePoints[3] = c_trapez[2];
			m_SavePoints[4] = c_trapez[3];
			m_SavePoints[5] = c_trapez[0];
			m_count = 6;
		}
		else 
		{
			const float32_t fill = Ah / c_At;

			C2DTrapeze::TrapezePoints tpoints;
			c_trapez.GetFilling(fill, tpoints, count);
			for (uint32_t index = 0; index < count; index++)
			{
				m_SavePoints[index] = tpoints[index];
			}
			m_count = count;
		}
	}
	count = m_count;
	points = m_SavePoints;
}
};
