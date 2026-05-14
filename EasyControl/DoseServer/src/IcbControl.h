//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IcbControl
///
/// @file   IcbControl.h
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

#include "BaseControl.h"



class CIcbControl : public CBaseControl
{
public:
	explicit CIcbControl(int32_t s) : CBaseControl(s)
	{}

	CIcbControl() = delete;
	~CIcbControl(void) override = default;
};



