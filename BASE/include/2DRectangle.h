//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module 2DRectangle.h
///
/// @file   2DRectangle.h
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
#include <array>
#include "BASE/Base.h"
#include "GRect.h"

namespace base
{
class C2DRectangle
{
public:
	static const uint32_t c_MaxPoints = 2;
	using HopperPoints = std::array<IPoint, c_MaxPoints>;

private:
	const IRect		 c_rect;
	const float32_t	 c_Ag;
	HopperPoints	m_SavePoints;
	int32_t			m_lastFilling;

private:
	C2DRectangle() = delete;

public:
	explicit C2DRectangle(const HopperPoints& points) :
		c_rect(points[0], points[1])
		, c_Ag{ _F32(c_rect.width() * c_rect.height()) }
		, m_SavePoints{ points }
		, m_lastFilling(0)
	{}
	void GetFilling(const float32_t filling, HopperPoints&, uint32_t& count);
};
//**********************************************************************************************************************
inline	void C2DRectangle::GetFilling(const float32_t filling, HopperPoints& points, uint32_t& count)
{
	const float32_t Ah = filling * c_Ag;
	const int32_t As = _S32(Ah);
	if (As != m_lastFilling)
	{
		m_lastFilling = As;
		const int32_t y    = __min(NEXTLONG(_F32(Ah / c_rect.width())), _S32(c_rect.height()));
		m_SavePoints[0]    = base::IPoint{ c_rect.topleft.x, c_rect.bottomright.y - y };
		m_SavePoints[1]   = c_rect.bottomright;
	}
	count = c_MaxPoints;
	points = m_SavePoints;
}
};
