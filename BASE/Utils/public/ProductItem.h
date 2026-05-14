//------------------------------------------------------------------------------------
///
///                           >><   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ProductItem
///
/// @file   LogFile.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <fstream>
#include "BASE/Base.h"

namespace base
{
namespace utils
{
//*****************************************************************************************
	class CProductItem
	{
	private:
		int32_t				m_ID;
		std::string			m_ProductName;			  // Name
		uint64_t			m_QMNumber;               // QM-Number

	public:
		std::string MakeString(void) const;

	public:
		CProductItem(int32_t _lID = -1L) : m_ID(_lID)
			, m_ProductName()
			, m_QMNumber{ 0 }
		{
		}

		CProductItem(const int32_t _lID, const std::string& _name, const uint64_t _number) : m_ID(_lID)
			, m_ProductName(_name)
			, m_QMNumber(_number)
		{
		}
		CProductItem(const CProductItem&) = default;
		CProductItem& operator = (const CProductItem&) = default;
		virtual ~CProductItem(void) = default;

		SETGET(int32_t, ID)
		SETGET(const std::string&, ProductName)
		SETGET(const uint64_t&, QMNumber)
	};

	//------------------------------------------------------------------------------------
	std::ostream& operator << (std::ostream& os, const CProductItem& dt);

	//------------------------------------------------------------------------------------
	std::istream& operator >> (std::istream& is, CProductItem& dt);

}
}


