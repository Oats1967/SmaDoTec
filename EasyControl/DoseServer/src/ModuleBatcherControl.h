//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ModuleBatcherControl
///
/// @file   ModuleBatcherControl.h
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

#include "BASE/include/Statemachine.h"


class CModuleBatcherControl : public base::CStatemachine<>
{
	using CBaseClass = base::CStatemachine<>;

private:
	static void Init();

	CModuleBatcherControl() = delete;
public:
	CModuleBatcherControl(int32_t id);

	~CModuleBatcherControl(void) override = default;
	BOOL Start(const uint32_t t) override;
};

