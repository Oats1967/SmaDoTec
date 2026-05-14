//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseType
///
/// @file   DoseType.h
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

#include "BASE/types.h"

namespace base
{
	enum class eDoseType : uint32_t
	{
		DOSETYPE_NONE = 0,
		DOSETYPE_LWS,
		DOSETYPE_LWV,
		DOSETYPE_LWP,
		DOSETYPE_LWB,	// Loss in weight batcher
		DOSETYPE_GWB,	// Gain in weight batcher
		DOSETYPE_ICB,	// Integral control port batcher
		DOSETYPE_WBF,
		DOSETYPE_PID,
		DOSETYPE_ICP,
		DOSETYPE_WBM,
		DOSETYPE_VOL,
		DOSETYPE_LCL,  // Nur Waage
		DOSETYPE_IFL,  // Infeed hopper - Einlauftricher mit Gewicht ( Loadcell )
		DOSETYPE_IFSO, // Infeed hopper - Einlauftricher mit Switches und Überfütterung
		DOSETYPE_IFSU, // Infeed hopper - Einlauftricher mit Switches und Unterfütterung
		DOSETYPE_MAX
	};
	static const uint32_t cDoseTypeMax = _U32(eDoseType::DOSETYPE_MAX);


	inline BOOL __ISNONETYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_NONE);
	}

	inline BOOL __ISVALIDDOSETYPE(const eDoseType _type)
	{
		return (_S32(_type) > 0) && (_S32(_type) < _S32(eDoseType::DOSETYPE_LCL));
	}

	inline BOOL __ISLCLTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_LCL);
	}

	inline BOOL __ISICPTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_ICP);
	}

	inline BOOL __ISPIDTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_PID);
	}

	inline BOOL __ISLWVTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_LWV);
	}

	inline BOOL __ISLWPTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_LWP);
	}

	inline BOOL __ISLWSTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_LWS);
	}

	inline BOOL __ISLWFTYPE(const eDoseType _type)
	{
		return __ISLWPTYPE(_type) || __ISLWVTYPE(_type) || __ISLWSTYPE(_type);
	}

	inline BOOL __ISLWBTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_LWB);
	}

	inline BOOL __ISLWXTYPE(const eDoseType _type)
	{
		return __ISLWFTYPE(_type) || __ISLWBTYPE(_type);
	}

	inline BOOL __ISWBMTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_WBM);
	}

	inline BOOL __ISGWBTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_GWB);
	}

	inline BOOL __ISICBTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_ICB);
	}

	inline BOOL __ISBATCHERTYPE(const eDoseType _type)
	{
		return __ISLWBTYPE(_type) || __ISGWBTYPE(_type) || __ISICBTYPE(_type);
	}

	inline BOOL __ISWBFTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_WBF);
	}

	inline BOOL __ISBELTWEIGHERTYPE(const eDoseType _type)
	{
		return __ISWBMTYPE(_type) || __ISWBFTYPE(_type);
	}


	inline BOOL __ISVOLTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_VOL);
	}

	inline BOOL __ISIFLTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_IFL);
	}

	inline BOOL __ISIFSTYPE(const eDoseType _type)
	{
		return (_type == eDoseType::DOSETYPE_IFSO) || (_type == eDoseType::DOSETYPE_IFSU);
	}

	inline BOOL __ISIFXTYPE(const eDoseType _type)
	{
		return __ISLCLTYPE(_type);
	}

	inline BOOL __ISLOADCELLTYPE(const eDoseType _type)
	{
		return __ISLWXTYPE(_type) || __ISBELTWEIGHERTYPE(_type) || __ISGWBTYPE(_type) || __ISLCLTYPE(_type) || __ISIFLTYPE(_type);
	}

};

#define __DOSETYPETOSTRING  { "-", "LWS", "LWV", "LWP", "LWB", "GWB", "ICB", "WBF", "PID", "ICP", "WBM", "VOL", "LCL", "IFL", "IFS" }

#define DECLARE_DOSETYPE(field) const std::array<base::eDoseType, base::cDoseTypeMax> field = { \
    base::eDoseType::DOSETYPE_NONE, \
	base::eDoseType::DOSETYPE_LWS,\
	base::eDoseType::DOSETYPE_LWV,\
	base::eDoseType::DOSETYPE_LWP,\
	base::eDoseType::DOSETYPE_LWB,\
	base::eDoseType::DOSETYPE_GWB,\
	base::eDoseType::DOSETYPE_ICB,\
	base::eDoseType::DOSETYPE_WBF,\
	base::eDoseType::DOSETYPE_PID,\
	base::eDoseType::DOSETYPE_ICP,\
	base::eDoseType::DOSETYPE_WBM,\
	base::eDoseType::DOSETYPE_VOL\
	base::eDoseType::DOSETYPE_LCL\
	base::eDoseType::DOSETYPE_IFL\
	base::eDoseType::DOSETYPE_IFS\
}



