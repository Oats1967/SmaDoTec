//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseStatistics
///
/// @file   DoseStatistics.h
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
#include <numeric>
#include "BASE/types.h"
#include "BASE/include/DoseType.h"

namespace base
{
	class CDoseStatistics
	{
		std::array<uint32_t, cDoseTypeMax> m_Statistic;
		uint32_t m_total;

	public:
		CDoseStatistics(void) : m_Statistic{ 0 }, m_total { 0 }
		{}

		uint32_t& operator [] (const uint32_t index)
		{	
			assert(index < cDoseTypeMax);
			return m_Statistic[index];	
		}
		//---------------------------------------------------------------------------------------
		virtual ~CDoseStatistics() = default;

		uint32_t CalcTotal()
		{
			m_total = std::accumulate(m_Statistic.cbegin(), m_Statistic.cend(), 0);
			return m_total;
		}

		BOOL IsAllBatcher() const
		{
			auto sum =	m_Statistic[_S32(eDoseType::DOSETYPE_LWB)] +
						m_Statistic[_S32(eDoseType::DOSETYPE_GWB)];
			return sum == m_total;
		}

		BOOL IsOneBatcher() const
		{
			auto sum = m_Statistic[_S32(eDoseType::DOSETYPE_LWB)] +
					   m_Statistic[_S32(eDoseType::DOSETYPE_GWB)];
			return sum > 0;
		}

		BOOL IsAllLwf() const
		{
			auto sum  = m_Statistic[_S32(eDoseType::DOSETYPE_LWS)] +
						m_Statistic[_S32(eDoseType::DOSETYPE_LWV)] +
						m_Statistic[_S32(eDoseType::DOSETYPE_LWP)];

			return sum == m_total;
		}

		BOOL IsOneLwf() const
		{
			auto sum = m_Statistic[_S32(eDoseType::DOSETYPE_LWS)] +
				m_Statistic[_S32(eDoseType::DOSETYPE_LWV)] +
				m_Statistic[_S32(eDoseType::DOSETYPE_LWP)];

			return sum > 0;
		}

		void Clear()
		{
			m_Statistic.fill(0);
			m_total = 0;
		}

	};

};

