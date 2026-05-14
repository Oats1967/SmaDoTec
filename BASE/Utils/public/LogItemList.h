//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LogItemList
///
/// @file   LogItemList.h
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


#include <string>
#include <ctime>
#include "BASE/types.h"
#include "BASE/Utils/public/LogItem.h"
#include "BASE/Utils/public/GenericList.h"

namespace base
{
namespace utils
{
class CLogItemList : public CGenericList< CLogItem>
{
private:
	BOOL		m_bChanged;
	std::string m_filename;

protected:
	//------------------------------------------------------------------------------------
	///  @brief   GetLastUnCheckedItem
	///
	///           This method ....
	///
	///  @param[in] lAlarmTyp , int32_t
	///  @return CLogItem*
	///
	//------------------------------------------------------------------------------------
	const CLogItem* GetLastUnCheckedItem(const eAlarmClass lAlarmTyp) const;

public:
	CLogItemList(int32_t lID = -1L) : CGenericList(lID)
		, m_bChanged(FALSE)
		, m_filename ("")
	{}

	~CLogItemList(void) override = default;
	CLogItemList(const CLogItemList& rList) : CGenericList{ rList }
		, m_bChanged{ rList.m_bChanged }
		, m_filename { rList.m_filename }
	{}

	CLogItemList(CLogItemList&& rList) noexcept : CGenericList{ std::move(rList) }
		, m_bChanged{ std::move(rList.m_bChanged) }
		, m_filename{ std::move(rList.m_filename) }
	{}

	CLogItemList& operator = (const CLogItemList& rList)
	{
		if (&rList != this)
		{
			CGenericList::operator = (rList);
			m_bChanged = rList.m_bChanged;
			m_filename = rList.m_filename;
		}
		return *this;
	}

	CLogItemList& operator += (const CLogItemList& rList)
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
	{	return m_filename; 	}
	///------------------------------------------------------------------------------------
	///  @brief   CreateFilename
	///
	///           This method ....
	///
	///  @param[in] const std::time_t
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetFilename(const std::time_t = 0);
	void SetFilename(const int32_t year, const int32_t month, const int32_t day);
	void SetFilename(const std::string& filename)
	{ m_filename = filename;}


	void	AddItem(const CLogItem& bPosition) override
	{
		CGenericList::AddItem(bPosition);
		m_bChanged = TRUE;
	}

	BOOL	IsChanged(void) const
	{	return m_bChanged;	}

	void	SetChanged(BOOL b)
	{	m_bChanged = b;	}

	void    CheckItems(const int32_t lS);

	//------------------------------------------------------------------------------------
		///  @brief   GetLastUnCheckedItem
		///
		///           This method ....
		///
		///  @param[in] none
		///  @return CLogItem*
		///
		//------------------------------------------------------------------------------------
	const CLogItem* GetLastUnCheckedItem(void) const;
	void    CheckLogItems(int32_t sItem, const base::eAlarmError error, const eAlarmClass lAlarmClass);
	void	TransitionAlarm(int32_t sItem, const base::eAlarmError error, const eAlarmClass oldClass, const eAlarmClass newClass);
	void	Extract(const time_t& start, const time_t& end);

	BOOL CheckAllItems();
};

};
};

