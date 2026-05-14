//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDoseSensor
///
/// @file   AdsDoseSensor.h
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
#include "AdsSensor.h"

namespace AdsSensor
{

template <typename T>
class CAdsDoseSensor : public CAdsSensor<T>
{
	using CBaseClass = CAdsSensor<T>;

public:
	typedef int32_t(*AdsSensorFunc) (int32_t, T&);
	typedef int32_t(*SetFunc) (int32_t, T);


private:
	AdsSensorFunc   m_pfAdsSensorFunc;
	SetFunc		    m_pfSetFunc;
	

protected:
	void SetValue( const T b) override;
public:
	CAdsDoseSensor(const int32_t id, AdsSensorFunc pfSensorFunc, SetFunc pfSetFunc = nullptr);
    CAdsDoseSensor() : m_pfAdsSensorFunc{ nullptr },
                       m_pfSetFunc{ nullptr }
    {}


	virtual ~CAdsDoseSensor() = default;

	void Init() override;
    void Exit() override;
    BOOL UpdateState()  override;
    BOOL UpdateState(T&) override;
};

//**********************************************************************************************
//**********************************************************************************************
template <typename T>
inline CAdsDoseSensor<T>::CAdsDoseSensor(const int32_t id, AdsSensorFunc pfSensorFunc, SetFunc pfSetFunc) : CBaseClass(id)
, m_pfAdsSensorFunc{ pfSensorFunc }
, m_pfSetFunc{ pfSetFunc }
{
    assert(m_pfAdsSensorFunc);
}
//**********************************************************************************************
//**********************************************************************************************
template <typename T>
inline void CAdsDoseSensor<T>::SetValue(const T value)
{
    assert(CBaseClass::IsInit());
    CBaseClass::SetState(value);
    if (m_pfSetFunc)
    {
        (*m_pfSetFunc)(CBaseClass::m_id, value);
    }
}
//**********************************************************************************************
//**********************************************************************************************
template <typename T>
inline void CAdsDoseSensor<T>::Init()
{
    assert(!CBaseClass::IsInit());
    CBaseClass::Init();

    T bValue = T(0);
    auto result = (*m_pfAdsSensorFunc)(CBaseClass::m_id, bValue);
    CBaseClass::SetEnable(result == 0);
    SetValue(bValue);
}
//**********************************************************************************************
//**********************************************************************************************
template <typename T>
inline void CAdsDoseSensor<T>::Exit()
{
    assert(CBaseClass::IsInit());

    SetValue(T(0));
    CBaseClass::Exit();
}
//**********************************************************************************************
//**********************************************************************************************
template <typename T>
inline BOOL CAdsDoseSensor<T>::UpdateState()
{
    assert(CBaseClass::IsInit());
    T bValue = T(0);
    return UpdateState(bValue);
}
//**********************************************************************************************
//**********************************************************************************************
template <typename T>
inline BOOL CAdsDoseSensor<T>::UpdateState(T& bValue)
{
    assert(CBaseClass::IsInit());
    bValue = T(0);
    auto bEnable = CBaseClass::GetEnable();
    if (bEnable)
    {
        auto result = (*m_pfAdsSensorFunc)(CBaseClass::m_id, bValue);
        assert(result == 0);
        if (CBaseClass::IsChanged(bValue))
        {
            SetValue(bValue);
        }
    }
    return bEnable;
}

using CAdsDoseDigitalInput = CAdsDoseSensor<BOOL>;
using CAdsDoseAnalogInput = CAdsDoseSensor<float32_t>;

};

