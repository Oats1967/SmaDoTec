//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsLineButton
///
/// @file   AdsLineButton.h
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
	class CAdsLineButton : public CAdsButton<T>
	{
		using CBaseClass = CAdsButton<T>;

		typedef int32_t(*AdsLineButtonFunc) (const T);
		typedef int32_t(*SetDoseFunc) (const T);

	protected:
		AdsLineButtonFunc m_pfAdsLineButtonFunc;
		SetDoseFunc		  m_pfSetLineFunc;

	protected:
		void SetValue(const T b) override;

	public:
		CAdsLineButton(AdsLineButtonFunc pfButtonFunc, SetDoseFunc pfSetLineFunc = nullptr) : CBaseClass(0)
			, m_pfAdsLineButtonFunc{ pfButtonFunc }
			, m_pfSetLineFunc{ pfSetLineFunc }
		{
			assert(m_pfAdsLineButtonFunc);
		}

		CAdsLineButton() : CBaseClass()
			, m_pfAdsLineButtonFunc{ nullptr }
			, m_pfSetLineFunc{ nullptr }
		{}

		virtual ~CAdsLineButton() = default;
		void Init() override;
		void Exit() override;
		BOOL UpdateState(T bState)  override;
	};

	//**********************************************************************************************
	//**********************************************************************************************
	template <typename T>
	inline void CAdsLineButton<T>::SetValue(const T value)
	{
		assert(CBaseClass::IsInit());
		CBaseClass::SetState(value);
		if (m_pfSetLineFunc)
		{
			(*m_pfSetLineFunc)(value);
		}
	}
	//**********************************************************************************************
	//**********************************************************************************************
	template <typename T>
	inline void CAdsLineButton<T>::Init()
	{
		assert(! CBaseClass::IsInit());

		CBaseClass::Init();

		T bValue = T(0);
		auto result = (*m_pfAdsLineButtonFunc)(bValue);
		CBaseClass::SetEnable(result == 0);
		SetValue(bValue);
	}
	//**********************************************************************************************
	//**********************************************************************************************
	template <typename T>
	inline void CAdsLineButton<T>::Exit()
	{
		assert(CBaseClass::IsInit());
		SetValue(T(0));
		CBaseClass::Exit();
	}
	//**********************************************************************************************
	//**********************************************************************************************
	template <typename T>
	inline BOOL CAdsLineButton<T>::UpdateState(const T bState)
	{
		assert(CBaseClass::IsInit());
		auto bChanged = CBaseClass::IsChanged(bState);
		if (bChanged)
		{
			if (CBaseClass::GetEnable())
			{
				auto result = (*m_pfAdsLineButtonFunc)(bState);
				assert(result == 0);
			}
			SetValue(bState);
		}
		return bChanged;
	}

	using CAdsLineDigitalOutput = CAdsLineButton<BOOL>;
	using CAdsLineAnalogOutput = CAdsLineButton<float32_t>;

};

