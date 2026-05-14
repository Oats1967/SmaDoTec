//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module StepControl
///
/// @file   StepControl.h
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

#include <map>
#include <cassert>
#include "BASE/Base.h"


namespace base
{
//--------------------------------------------------------------------------------------------------------------------------------------
template < class T1, class T2>
class CStepControl
{
	typedef void (T2::* MFP)();
	std::map<T1, MFP> r_map;

public:
	CStepControl() {}
	CStepControl(std::map<T1, MFP>&& vMap) : r_map{ std::move(vmap) }

	void Control(const T1 v);
};

template < class T1, class T2>
void CStepControl::Control(const T1 vIndex)
{
	try
	{
		(this->*r_map.at(vIndex))();
	}
	catch (std::out_of_range)
	{
		assert(FALSE);
	}
}
