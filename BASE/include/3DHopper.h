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

#include <cstdlib>
#include "BASE/Base.h"
#include "GRect.h"
#include "2DTrapeze.h"

namespace base
{
class C3DHopper
{
public:
	static const uint32_t c_MaxPoints = 5;
	using HopperPoints = std::array<IPoint, c_MaxPoints>;

private:
	const IRect		c_rect;
	HopperPoints	m_SavePoints;
	int32_t			m_lastFilling;
	uint32_t		m_count;
	float32_t		m_Ag;

public:
	explicit C3DHopper(const HopperPoints& points) :
		  c_rect { points[0], points[2] }
		, m_Ag { _F32(c_rect.width() * c_rect.height()) }
		, m_SavePoints{ points }
		, m_lastFilling(-1)
		, m_count(0)
	{}
	C3DHopper() = delete;
	void GetFilling(const float32_t filling, HopperPoints&, uint32_t& count);
};
//**********************************************************************************************************************
inline	void C3DHopper::GetFilling(const float32_t filling, HopperPoints& points, uint32_t& count)
{
	const float32_t Ah = filling * m_Ag;
	const int32_t As = _S32(Ah);
	if (As != m_lastFilling)
	{
		m_lastFilling = As;
		if (As > 0)
		{
			const int32_t y = __min(NEXTLONG(Ah / c_rect.width()), c_rect.height());
			m_SavePoints[0] = base::IPoint{ c_rect.topleft.x, c_rect.bottomright.y + y };
			m_SavePoints[1] = base::IPoint{ c_rect.bottomright.x, c_rect.bottomright.y + y };
			m_SavePoints[2] = base::IPoint{ c_rect.bottomright.x, c_rect.bottomright.y };
			m_SavePoints[3] = base::IPoint{ c_rect.topleft.x, c_rect.bottomright.y };
			m_SavePoints[4] = m_SavePoints[0];
			m_count = 5;
		}
		else
		{
			m_count = 0;
		}
	}
	count = m_count;
	points = m_SavePoints;
}
};
