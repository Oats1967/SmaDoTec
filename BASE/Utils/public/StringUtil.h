//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module StringUtil
///
/// @file   StringUtil.h
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
#include <string>


namespace base
{
namespace utils
{
	std::wstring s2ws(const std::string& str);
	std::string  ws2s(const std::wstring& wstr);
	uint64_t ustoull(const std::wstring& _Str);
	uint64_t ustoull(const std::string& _Str);
	uint32_t ustoul(const std::wstring& _Str);
	uint32_t ustoul(const std::string& _Str);
	float32_t ustof(const std::wstring& _Str);
	float32_t ustof(const std::string& _Str);
	int32_t ustoi(const std::string& _Str);
	int32_t ustoi(const std::wstring& _Str);

	template<class T1, class T2>
	class CStringConvert
	{
		const T1& src;
		T2 dest;

		CStringConvert() = delete;

	private:
		void convert();

	public:
		CStringConvert(const T1& _src) : src(_src)
		{	convert();		}


		const T2& get() const
		{ return dest;	}

		operator const T2& () const
		{ return dest; 	}
	};

	template<>
	inline void CStringConvert<std::string, std::wstring>::CStringConvert::convert()
	{ dest = s2ws(src);	}

	template<>
	inline void CStringConvert<std::string, std::string>::CStringConvert::convert()
	{	dest = src;	}

	template<>
	inline void CStringConvert<std::wstring, std::wstring>::CStringConvert::convert()
	{	dest = src;	}

	template<>
	inline void CStringConvert<std::wstring, std::string>::CStringConvert::convert()
	{	dest = ws2s(src);	}


};
};




