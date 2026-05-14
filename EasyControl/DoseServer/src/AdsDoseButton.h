//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDoseButton
///
/// @file   AdsDoseButton.h
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

#include <cassert>
#include "AdsButton.h"

namespace AdsSensor
{
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
template <typename T>
class CAdsDoseButton : public CAdsButton<T>
{
	using CBaseClass = CAdsButton<T>;

public:
	typedef int32_t(*AdsDoseButtonFunc) (int32_t, T);
	typedef int32_t(*SetDoseFunc)	    (int32_t, T);

protected:
	AdsDoseButtonFunc m_pfAdsDoseButtonFunc;
	SetDoseFunc		  m_pfSetDoseFunc;

protected:
	void SetValue(const T b) override;

public:
	CAdsDoseButton(const int32_t id, AdsDoseButtonFunc pfButtonFunc, SetDoseFunc pfSetDoseFunc = nullptr);
	virtual ~CAdsDoseButton() = default;

	void Init() override;
	void Exit() override;

	BOOL UpdateState(const T b) override;
};

//**********************************************************************************************
//**********************************************************************************************
template <typename T>
inline CAdsDoseButton<T>::CAdsDoseButton(const int32_t id, AdsDoseButtonFunc pfButtonFunc, SetDoseFunc pfSetDoseFunc) : CBaseClass(id)
	, m_pfAdsDoseButtonFunc{ pfButtonFunc }
	, m_pfSetDoseFunc{ pfSetDoseFunc }
{
	assert(m_pfAdsDoseButtonFunc);
}
//**********************************************************************************************
//**********************************************************************************************
template <typename T>
inline void CAdsDoseButton<T>::SetValue(const T value)
{
	assert(CBaseClass::IsInit());
	CBaseClass::SetState(value);
	if (m_pfSetDoseFunc)
	{
		(*m_pfSetDoseFunc)(CBaseClass::m_id, value);
	}
}
//**********************************************************************************************
//**********************************************************************************************
template <typename T>
inline void CAdsDoseButton<T>::Init()
{
	//assert(!CBaseClass::IsInit());
	CBaseClass::Init();

	T bValue = T(0);
	auto result = (*m_pfAdsDoseButtonFunc)(CBaseClass::m_id, bValue);
	CBaseClass::SetEnable(result == 0);
	SetValue(bValue);
}
//**********************************************************************************************
//**********************************************************************************************
template <typename T>
inline void CAdsDoseButton<T>::Exit()
{
	assert(CBaseClass::IsInit());
	SetValue(T(0));
	CBaseClass::Exit();
}
//**********************************************************************************************
//**********************************************************************************************
template <typename T>
inline BOOL CAdsDoseButton<T>::UpdateState(const T bState)
{
	assert(CBaseClass::IsInit());
	auto bChanged = CBaseClass::IsChanged(bState);
	if (bChanged)
	{
		if (CBaseClass::GetEnable())
		{
			auto result = (*m_pfAdsDoseButtonFunc)(CBaseClass::m_id, bState);
			assert(result == 0);
		}
		SetValue(bState);
	}
	return bChanged;
}

using CAdsDoseDigitalOutput = CAdsDoseButton<BOOL>;
using CAdsDoseAnalogOutput = CAdsDoseButton<float32_t>;


};


