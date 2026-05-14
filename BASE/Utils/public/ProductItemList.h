//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ProductItemList
///
/// @file   ProductItemList.h
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


#include "BASE/types.h"
#include "BASE/Utils/public/ProductItem.h"
#include "BASE/Utils/public/GenericList.h"

namespace base
{
namespace utils
{
	class CProductItemList : public CGenericList< CProductItem>
	{
	private:
		BOOL		m_bChanged;
		std::string m_filename;

	public:
		CProductItemList(int32_t lID = -1L) : CGenericList(lID)
			, m_bChanged(FALSE)
			, m_filename("")
		{}

		~CProductItemList(void) override = default;
		CProductItemList(const CProductItemList& rList) : CGenericList{ rList }
			, m_bChanged{ rList.m_bChanged }
			, m_filename{ rList.m_filename }
		{}

		CProductItemList(CProductItemList&& rList) noexcept : CGenericList{ std::move(rList) }
			, m_bChanged{ std::move(rList.m_bChanged) }
			, m_filename{ std::move(rList.m_filename) }
		{}

		CProductItemList& operator = (const CProductItemList& rList)
		{
			if (&rList != this)
			{
				CGenericList::operator = (rList);
				m_bChanged = rList.m_bChanged;
				m_filename = rList.m_filename;
			}
			return *this;
		}

		CProductItemList& operator += (const CProductItemList& rList)
		{
			CGenericList::operator += (rList);
			m_bChanged |= rList.m_bChanged;
			return *this;
		}

		///------------------------------------------------------------------------------------
		///  @brief   GetFilename
		///
		///           This method ....
		///
		///  @param[in] none
		///  @return none
		///
		//------------------------------------------------------------------------------------
		const std::string GetFilename(void) const override
		{
			return m_filename;
		}

		void SetFilename(const std::string& sz)
		{
			m_filename = sz;
		}

		void	AddItem(const CProductItem& bPosition) override
		{
			CGenericList::AddItem(bPosition);
			m_bChanged = TRUE;
		}

		BOOL	IsChanged(void) const
		{
			return m_bChanged;
		}

		void	SetChanged(BOOL b)
		{
			m_bChanged = b;
		}

		int32_t FindItemID(const std::string& _product);
		int32_t FindItemID(const uint64_t& _number);

		CProductItem* FindItem(const std::string& _product);
		CProductItem* FindItem(const uint64_t& _number);

		const CProductItem* FindItem(const std::string& _product) const;
		const CProductItem* FindItem(const uint64_t& _number) const;

	};
};
};

