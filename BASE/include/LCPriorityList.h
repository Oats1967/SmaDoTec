//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LCPriorityList
///
/// @file   LCPriorityList.h
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

#include <utility>
#include <array>
#include "BASE/types.h"
#include "BASE/include/LCType.h"

namespace base
{
	class CLCPriorityList
	{
		using typepair = std::pair< eLcType, uint32_t>;

	private:
		std::array<typepair, 3U> m_PriorList;

	private:
		void swap(const int32_t i0, const int32_t i1);
		void sort();

	public:
		CLCPriorityList() : m_PriorList{ { { eLcType::LC_PESA, 0 },
										   { eLcType::LC_SOEMER, 0 },
										   { eLcType::LC_LAUMAS, 0 } 
										  }	}
		{}

		void Add(const eLcType);
		void Add(const int32_t index);
		eLcType Get(const int32_t index) const;
		int32_t Size() const
		{	return _S32(m_PriorList.size());		}
		void Init();
	};
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	inline void CLCPriorityList::Init()
	{
		m_PriorList[0] = std::make_pair(eLcType::LC_PESA, 0 );
		m_PriorList[1] = std::make_pair(eLcType::LC_SOEMER, 0);
		m_PriorList[2] = std::make_pair(eLcType::LC_LAUMAS, 0);
	}
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	inline void CLCPriorityList::swap(const int32_t i0, const int32_t i1)
	{
		auto temp = m_PriorList[i0];
		m_PriorList[i0] = m_PriorList[i1];
		m_PriorList[i1] = temp;
	}
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	inline void CLCPriorityList::sort()
	{
		const auto count = _S32(m_PriorList.size());
		for (int32_t k0 = 1; k0 < count - 1; k0++)
		{
			int32_t km = k0;
			for (int32_t k1 = k0 + 1; k1 < count; k1++)
			{
				if (m_PriorList[k1].second > m_PriorList[km].second)
				{
					km = k1;
				}
			}
			if (km != k0)
			{
				swap(k0, km);
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	inline void CLCPriorityList::Add(const eLcType lctype)
	{
		const auto it = find_if(m_PriorList.cbegin(), m_PriorList.cend(), [&lctype](const auto& rItem) { return rItem.first == lctype; });
		assert(it != m_PriorList.cend());
		auto index = _S32(std::distance(m_PriorList.cbegin(), it));
		Add(index);
	}
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	inline void CLCPriorityList::Add(const int32_t index)
	{
		assert(index >= 0 && index < Size());
		m_PriorList[index].second++;
		if (index != 0)
		{
			swap(0, index);
			sort();
		}
	}
	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	inline eLcType CLCPriorityList::Get(const int32_t index) const
	{
		assert(index >= 0 && index < Size());
		return m_PriorList[index].first;
	}

};



