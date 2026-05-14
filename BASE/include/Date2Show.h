//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DateToShow
///
/// @file   DateToShow.h
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

#include "BASE/Utils/public/GenericTime.h"


namespace base
{
	struct DateToShow
	{
		std::time_t m_dateStart;
		std::time_t m_dateEnd;

		DateToShow() : m_dateStart(0), m_dateEnd(0)
		{}

		BOOL operator == (const DateToShow& rC) const
		{
			return (m_dateStart == rC.m_dateStart) && (m_dateEnd == rC.m_dateEnd);
		}

		BOOL operator != (const DateToShow& rC) const
		{
			return !operator == (rC);
		}

		void SetStartEndTime(const std::time_t& Start, const std::time_t& End)
		{
			m_dateStart = Start;
			m_dateEnd = End;
		}

		void SetStartEndTime(const COleDateTime& Start, const COleDateTime& End)
		{
			m_dateStart = utils::OleDateTime2Time(Start);
			m_dateEnd   = utils::OleDateTime2Time(End);
		}

		int32_t GetDiffDays() const
		{
			auto diff = GetOleDateTimeEnd() - GetOleDateTimeStart();
			return _S32(diff.GetTotalDays());
		}

		void now()
		{
			m_dateStart = std::time(0);
			m_dateEnd = m_dateStart;
		}

		COleDateTime GetOleDateTimeStart() const
		{	return utils::Time2OleDateTime(m_dateStart);		}

		COleDateTime GetOleDateTimeEnd() const
		{	return utils::Time2OleDateTime(m_dateEnd);		}

		BOOL IsSameDay() const
		{ return utils::IsSameDay(m_dateStart, m_dateEnd);		}

	};
};