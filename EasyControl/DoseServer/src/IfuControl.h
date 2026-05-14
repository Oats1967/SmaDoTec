//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IfuControl
///         Underfeeding control
///
/// @file   IfuControl.h
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

#include "IfsControl.h"


class CIfuControl : public CIfsControl
{
	using CBaseClass = CIfsControl;

	/*
				|    -> StepHighReached
		High	+
				|	 -> StepMaxReached
		Max  	+
				|	 -> StepMinReached
		Min		+
				|	-> StepLowReached
		Low		+
				|	-> StepEmptyReached

	*/
protected:
	uint32_t		m_t0;
	uint32_t		m_tEF;

protected:
	void EnterInitReached() override;
	void EnterEmptyOverflowReached() override;
	void EnterMaxOverflowReached() override;
	void EnterHighOverflowReached() override;
	void EnterHighReached() override;
	void EnterMaxReached() override;
	void EnterMinReached() override;
	void EnterLowReached() override;
	void EnterEmptyReached() override;
	void StepInitReached() override;

	void Restart() override;

	void OnMinReached() override;
	void OnHighReached() override;
	void OnMaxReached() override;
	void OnLowReached() override;
	void OnEmptyReached() override;
	void OnEmptyOverflowReached() override;
	void OnMaxOverflowReached() override;
	void OnHighOverflowReached() override;

public:
	CIfuControl(int32_t id) : CBaseClass(id)
		, m_t0{ 0 }
		, m_tEF{ 0 }
	{}

	~CIfuControl(void) override = default;
};

