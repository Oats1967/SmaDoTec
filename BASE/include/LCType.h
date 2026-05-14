//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LCType
///
/// @file   LCType.h
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
#include "BASE/types.h"

namespace base
{
	enum class eLcType : uint32_t
	{
		LC_EMPTY = 0,
		LC_PESA,
		LC_SOEMER,
		LC_LAUMAS,
		LC_KTRON,
		LC_MAXCOUNT
	};
	constexpr uint32_t cMaxLCTyp = _U32(eLcType::LC_MAXCOUNT);

	enum class LC_STATUS : uint32_t
	{
		LC_STATUS_OK = 0,					// present
		LC_STATUS_PROTOCOLERROR,			// present
		LC_STATUS_OVERLOADED,				// present
		LC_STATUS_NOTAUTHORIZED,			// present
		LC_STATUS_TIMEOUT,					// present
		LC_STATUS_UNKNOWN,					// not
		LC_STATUS_ERROR,					// not
		LC_STATUS_SYNCERROR,				// not
		LC_STATUS_NOTOPEN,					// not
		LC_STATUS_MAX
	};
	constexpr uint32_t cMaxLCStatus = _U32(LC_STATUS::LC_STATUS_MAX);



	enum class LC_PRIORITY : uint32_t
	{
		LC_PRIORITY_LOW = 0,
		LC_PRIORITY_NORMAL,
		LC_PRIORITY_HIGH,
		LC_PRIORITY_MAX
	};

	enum class LC_ERRORCODE : uint32_t
	{
		LC_OK = 0,
		LC_INCOMPATIBLE_SCALES,
		LC_NO_SOURCE_SCALE,
		LC_NO_DEST_SCALE,
		LC_SRC_BUSY,
		LC_NO_DEFINITION,
		LC_THREAD_OPEN_ERROR,
		LC_EEPROMERROR,
		LC_REGISTERCELL,
	};


	struct LcConfig
	{
		uint32_t m_Address = 0;
		eLcType m_LcType = eLcType::LC_EMPTY;

		LcConfig() : m_Address{ 0 }, m_LcType{ eLcType::LC_EMPTY }
		{}

		LcConfig(const LcConfig& cfg) : m_Address{ cfg.m_Address }, m_LcType{ cfg.m_LcType }
		{}

		LcConfig& operator = (const LcConfig& cfg) 
		{
			if (&cfg != this)
			{
				m_Address = cfg.m_Address;
				m_LcType = cfg.m_LcType;
			}
			return *this;
		}

		LcConfig(LcConfig&& cfg) noexcept : m_Address{ std::move(cfg.m_Address) }
								 , m_LcType{ std::move(cfg.m_LcType) }
		{}

		BOOL operator == (const LcConfig& cfg) const
		{ return (m_Address == cfg.m_Address) && (m_LcType == cfg.m_LcType);	}

		BOOL operator != (const LcConfig& cfg) const
		{	return ! (operator == (cfg));	}
	};

#define __ISLCENABLED(_a) ((_S32(_a) > _S32(base::eLcType::LC_EMPTY)) && (_S32(_a) < _S32(base::eLcType::LC_MAXCOUNT)))
#define INVALID_LC_LINKADDRESS (-1)

};



