//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsLineSensor
///
/// @file   AdsLineSensor.h
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
	class CAdsLineSensor : public CAdsSensor<T>
	{
		using CBaseClass = CAdsSensor<T>;

	public:
		typedef int32_t(*AdsSensorFunc) ( T&);
		typedef int32_t(*SetFunc) (T);

	private:
		AdsSensorFunc   m_pfAdsSensorFunc;
		SetFunc		    m_pfSetFunc;
	

	protected:
		void SetValue(const T b) override;

	public:
		CAdsLineSensor(AdsSensorFunc pfSensorFunc, SetFunc pfSetFunc = nullptr);
        CAdsLineSensor() :  m_pfAdsSensorFunc{ nullptr }
                            , m_pfSetFunc { nullptr }
        {}

		virtual ~CAdsLineSensor() = default;

		void Init() override;
        void Exit() override;
        BOOL UpdateState() override;
        BOOL UpdateState(T& bValue)  override;
    };

    //**********************************************************************************************
    //**********************************************************************************************
    template <typename T>
    inline CAdsLineSensor<T>::CAdsLineSensor(AdsSensorFunc pfSensorFunc, SetFunc pfSetFunc) : CBaseClass(0)
        , m_pfAdsSensorFunc{ pfSensorFunc }
        , m_pfSetFunc{ pfSetFunc }
    {
        assert(m_pfAdsSensorFunc);
    }
    //**********************************************************************************************
    //**********************************************************************************************
    template <typename T>
    inline void CAdsLineSensor<T>::SetValue(const T value)
    {
        assert(CBaseClass::IsInit());
        CBaseClass::SetState(value);
        if (m_pfSetFunc)
        {
            (*m_pfSetFunc)(value);
        }
    }
    //**********************************************************************************************
    //**********************************************************************************************
    template <typename T>
    inline void CAdsLineSensor<T>::Init()
    {
        assert( ! CBaseClass::IsInit());

        CBaseClass::Init();

        T bValue = T(0);
        auto result = (*m_pfAdsSensorFunc)(bValue);
        CBaseClass::SetEnable(result == 0);
        SetValue(bValue);
    }
    //**********************************************************************************************
    //**********************************************************************************************
    template <typename T>
    inline void CAdsLineSensor<T>::Exit()
    {
        assert(CBaseClass::IsInit());

        SetValue(T(0));
        CBaseClass::Exit();
    }
    //**********************************************************************************************
    //**********************************************************************************************
    template <typename T>
    inline BOOL CAdsLineSensor<T>::UpdateState()
    {
        T bValue = T(0);
        return UpdateState(bValue);
    }
    //**********************************************************************************************
    //**********************************************************************************************
    template <typename T>
    inline BOOL CAdsLineSensor<T>::UpdateState(T& bValue)
    {
        assert(CBaseClass::IsInit());

        bValue = T(0);
        auto bEnable = CBaseClass::GetEnable();
        if (bEnable)
        {
            auto result = (*m_pfAdsSensorFunc)(bValue);
            assert(result == 0);
            if (CBaseClass::IsChanged(bValue))
            {
                SetValue(bValue);
            }
        }
        return bEnable;
    }

    using CAdsLineDigitalInput = CAdsLineSensor<BOOL>;
    using CAdsLineAnalogInput  = CAdsLineSensor<float32_t>;
};

