//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EasyControlConfigXml
///
/// @file   EasyControlConfigXml.h
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
#include "BASE/types.h"

namespace base
{
namespace xml
{

template <class T >
class XmlContainer
{
private:
	T	     m_Field;
	BOOL	 m_Modified;


public:
	XmlContainer() : m_Field(), m_Modified(FALSE)
	{}
	XmlContainer(const T& _field) : m_Field(_field), m_Modified(FALSE)
	{}

	virtual ~XmlContainer() = default;

	BOOL IsModified() const
	{ return m_Modified; }

	void SetModified(BOOL value)
	{ m_Modified = value;	}

	void Set(const T& _field)
	{ m_Field = _field;	};

	const T& Get() const
	{ return m_Field;	};

	T& Get()
	{ return m_Field;};

	virtual BOOL Load(const std::string& _szPath2Device) = 0;
	virtual BOOL Save(const std::string& _szPath2Device = std::string("")) = 0;
};
};
};


