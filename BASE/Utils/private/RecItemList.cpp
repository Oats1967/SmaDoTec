//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecItemList
///
/// @file   RecItemList.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include <sstream>      // std::ostringstream
#include <iomanip>
#include <limits>
#include "BASE/Utils/public/RecItemList.h"
#include "Logger/include/Logger.h"


namespace base
{
namespace utils
{


//**********************************************************************************************
//**********************************************************************************************
void CRecItemList::CreateFilename(void)
{
	SetFilename(std::time(nullptr));
}
//**********************************************************************************************
//**********************************************************************************************
uint32_t CRecItemList::ScanMaxItems(void) const
{
	uint32_t sMaxItems = 0;
	for (const auto& item : m_ItemList)
	{
		auto sValue = item.GetMaxItems();
		if (sValue > sMaxItems)
		{
			sMaxItems = sValue;
		}
	}
	return sMaxItems;
}
//**********************************************************************************************
//**********************************************************************************************
void CRecItemList::SetFilename(std::time_t currenttime)
{
	std::tm aT;

	localtime_s(&aT, &currenttime);

	std::ostringstream stringStream;
	stringStream << "EC";
	stringStream << std::setfill('0') << std::setw(2) << (aT.tm_year + 1900);
	stringStream << std::setfill('0') << std::setw(2) << (aT.tm_mon + 1);
	stringStream << std::setfill('0') << std::setw(2) << (aT.tm_mday);
	stringStream << ".rec";
	m_filename = stringStream.str();
}
//**********************************************************************************************
//**********************************************************************************************
float32_t CRecItemList :: GetMaxValue(const eMassflowSelect iView, const int32_t id) const
{
    float32_t fMax = m_ItemList.empty() ? 0.0F : -FLT_MAX/2;
	for (const auto& item : m_ItemList)
	{
		const float32_t f = item.GetMaxValue(iView, id);
		if (f > fMax)
		{
			fMax = f;
		}
	}
    return fMax;
}
//**********************************************************************************************
//**********************************************************************************************
float32_t CRecItemList::GetMinValue(const eMassflowSelect iView, const int32_t id) const
{
	float32_t fMin = m_ItemList.empty() ? 0.0F : -FLT_MAX / 2;
	for (const auto& item : m_ItemList)
	{
		const float32_t f = item.GetMinValue(iView, id);
		if (f < fMin)
		{
			fMin = f;
		}
	}
	return fMin;
}
//**********************************************************************************************
//**********************************************************************************************
BOOL CRecItemList::SaveAll()
{
	for (auto& item : m_ItemList)
	{
		item.SetMaxItems(m_MaxItems);
	}
	return CGenericList::SaveAll();
}
//**********************************************************************************************
//**********************************************************************************************
BOOL CRecItemList::LoadAll()
{
	BOOL result = CGenericList::LoadAll();
	if (result)
	{
		m_MaxItems = ScanMaxItems();
	}
	return result;
}
//**********************************************************************************************
//**********************************************************************************************
BOOL CRecItemList::SaveLastItem()
{
	if (!m_ItemList.empty())
	{
		CRecItem& aLast = GetLastItem();
		aLast.SetMaxItems(m_MaxItems);
	}
	return CGenericList::SaveLastItem();
}
//**********************************************************************************************
//**********************************************************************************************
int32_t CRecItemList::FindRecItemTime(const std::time_t& t) const
{
	if (m_ItemList.empty())
	{
		return -1;
	}
	int32_t l = 0;
	int32_t r = _S32(GetCount()) - 1;

	int32_t index = -1;
	while (l <= r)
	{
		int32_t q = (l + r) >> 1;
		const auto& tA = GetItem(q).GetTime();
		if (tA == t)
		{
			index = q;
			break;
		}
		if ( std::difftime(t, tA) > 0.0)
		{
			l = q + 1;
		}
		else
		{
			r = q - 1;
		}
	}
	if (index < 0)
	{
		if (l >= 0)
		{
			l = __min( l, _S32(GetCount()) - 1);
			if (r >= 0)
			{
				if (fabs(std::difftime(t, GetItem(l).GetTime())) < fabs(std::difftime(t, GetItem(r).GetTime())))
				{
					index = l;
				}
				else
				{
					index = r;
				}
			}
			else
			{
				index = l;
			}
		}
		else
		{
			// l < 0
			index = (r >= 0) ? r : -1;
		}
	}
#if 0
	{
		std::string s0 = TimeAndDateToString(t);
		std::string i0, l0, r0;
		if (index >= 0)
		{
			i0 = TimeAndDateToString(GetItem(index).GetTime());
		}
		if (l >= 0)
		{
			l0 = TimeAndDateToString(GetItem(l).GetTime());
		}
		if (r >= 0)
		{
			r0 = TimeAndDateToString(GetItem(r).GetTime());
		}
		int k = 0;
	}
#endif
	return index;
}
//**********************************************************************************************
//**********************************************************************************************
void CRecItemList::Extract(const std::string& ANNumber) 
{
	if (! ANNumber.empty())
	{
		CGenericList::Extract([&ANNumber](const CRecItem& rItem) 
		{
			return (rItem.GetANNumber() == ANNumber);
		});
	}
}
//**********************************************************************************************
//**********************************************************************************************
void CRecItemList::Extract(const time_t& tStart, const time_t& tEnd)
{
	CGenericList::Extract([&tStart, &tEnd](const CRecItem& rItem)
		{
			const auto& t = rItem.GetTime();
			return((t >= tStart) && (t <= tEnd));
		});
}
//**********************************************************************************************
//**********************************************************************************************
void CRecItemList::GetANNumbers(std::vector<std::string>& rANNumberList) const
{
	rANNumberList.clear();
	const auto count = GetCount();
	for (int32_t k = 0; k < _S32(count); k++)
	{
		const auto& rItem = GetItem(k).GetANNumber();
		if (! rItem.empty())
		{
			auto it = find(rANNumberList.cbegin(), rANNumberList.cend(), rItem);
			if (it == rANNumberList.cend())
			{
				rANNumberList.push_back(rItem);
			}
		}
	}
}


};
};


