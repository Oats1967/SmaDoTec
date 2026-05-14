//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DSAlarm
///
/// @file   DSAlarm.hpp
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

#include "BASE/include/AlarmTypes.def"
#include "BASE/include/AlarmTypes.h"
#include "ECImplMacro.h"


inline int32_t CDose::ECSetAlarmHelper(const base::eAlarmError l, ECTypes<base::eAlarmErrorBits>& field)
{
	if ( ! field.value.IsAlarmErrorBits(l))
	{
		field.value.SetAlarmErrorBits(l);
		field.bChanged = TRUE;
	}
	return DOSIERUNG_OK;
}

inline int32_t CDose::ECDeleteAlarmHelper(const base::eAlarmError l, ECTypes<base::eAlarmErrorBits>& field)
{
	if ( field.value.IsAlarmErrorBits(l) )
	{
		field.value.SetAlarmErrorBits(l, FALSE);
		field.bChanged = TRUE;
	}
	return DOSIERUNG_OK;
}

inline int32_t CDose::ECGetAlarmHelper(const  ECTypes<base::eAlarmErrorBits>& field, base::eAlarmError& l )
{
	const auto& value = field.value;
	if (value.allflags )
	{
		DECLARE_ALARMERROR(ar)

		auto ret = std::find_if(ar.cbegin(), ar.cend(), [value](const auto& item) { return value.IsAlarmErrorBits(item); });
		if (ret != ar.cend())
		{
			l = *ret;
		}
		else
		{
			l = base::eAlarmError::ERROR_MAX;
		}
	}
	return DOSIERUNG_OK;
}

inline int32_t CDose::ECIsAlarmHelper(const  ECTypes<base::eAlarmErrorBits>& field, const base::eAlarmError l, BOOL& b)
{
	b = field.value.IsAlarmErrorBits(l);
	return DOSIERUNG_OK;
}

inline int32_t CDose::ECIsAnyAlarmHelper(const  ECTypes<base::eAlarmErrorBits>& field, BOOL& b)
{
	b = (field.value.allflags != 0);
	return DOSIERUNG_OK;
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
int32_t CDose::ECSetAlarm(const int32_t n, base::eAlarmError l)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[n]);
	auto& item = mDGlobal[n].get();
	return ECSetAlarmHelper(l, item.Alarm);
}

int32_t CDose::ECGetAlarm(const int32_t n, base::eAlarmError* l)	const
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	return ECGetAlarmHelper(item.Alarm, *l);
}

int32_t CDose::ECDeleteAlarm(const int32_t n, const base::eAlarmError l)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[n]);
	auto& item = mDGlobal[n].get();
	return ECDeleteAlarmHelper(l, item.Alarm);
}

int32_t CDose::ECIsAlarm(const int32_t n, const base::eAlarmError l, BOOL* b) const
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	return ECIsAlarmHelper(item.Alarm, l, *b);
}

int32_t CDose::ECIsAnyAlarm(const int32_t n, BOOL* b) const
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	return ECIsAnyAlarmHelper(item.Alarm, *b);
}


int32_t CDose::ECSetFieldAlarm(const int32_t n, const base::eAlarmErrorBits& ul)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	auto& item = mDGlobal[n].get();
	if (item.Alarm.Test(ul))
	{
		std::lock_guard<std::mutex> lock(mDGlobal[n]);
		item.Alarm.Set(ul);
	}
	return DOSIERUNG_OK;
}

int32_t CDose::ECDeleteFieldAlarm(const int32_t n)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[n]);
	auto& item = mDGlobal[n].get();
	item.Alarm.Delete();
	return DOSIERUNG_OK;
}


int32_t CDose::ECGetFieldAlarm(const int32_t n, base::eAlarmErrorBits& ul) const
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	ul = item.Alarm.Get();
	return DOSIERUNG_OK;
}


//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
int32_t CDose :: ECSetClearAlarm	( const int32_t n, base::eAlarmError l)
{
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[n]);
	auto& item = mDGlobal[n].get();
	return ECSetAlarmHelper(l, item.ClearAlarm);
}	


int32_t CDose :: ECGetClearAlarm	( const int32_t n, base::eAlarmError *l)	const
{
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	return ECGetAlarmHelper(item.ClearAlarm, *l);
}


int32_t CDose :: ECDeleteClearAlarm	( const int32_t n, const base::eAlarmError l)
{
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[n]);
	auto& item = mDGlobal[n].get();
	return ECDeleteAlarmHelper(l, item.ClearAlarm);
}


int32_t CDose :: ECIsClearAlarm	( const int32_t n, const base::eAlarmError l, BOOL *b) const
{
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	return ECIsAlarmHelper(item.ClearAlarm, l, *b);
}	


int32_t CDose :: ECIsAnyClearAlarm	( const int32_t n, BOOL *b) const
{
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	return ECIsAnyAlarmHelper(item.ClearAlarm, *b);
}	


int32_t CDose :: ECSetFieldClearAlarm(const int32_t n, const base::eAlarmErrorBits& ul)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	auto& item = mDGlobal[n].get();
	if (item.ClearAlarm.Test(ul))
	{
		std::lock_guard<std::mutex> lock(mDGlobal[n]);
		item.ClearAlarm.Set(ul);
	}
	return DOSIERUNG_OK;
}	


int32_t CDose :: ECDeleteFieldClearAlarm	( const int32_t n)
{
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[n]);
	auto& item = mDGlobal[n].get();
	item.ClearAlarm.Delete();
	return DOSIERUNG_OK;
}	


int32_t CDose :: ECGetFieldClearAlarm(const int32_t n, base::eAlarmErrorBits& ul) const
{
	assert(n>= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
    ul = item.ClearAlarm.Get();
	return DOSIERUNG_OK;
}	


//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
int32_t CDose::ECSetWarning(const int32_t n, base::eAlarmError l)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[n]);
	auto& item = mDGlobal[n].get();
	return ECSetAlarmHelper(l, item.Warning);
}

int32_t CDose::ECGetWarning(const int32_t n, base::eAlarmError* l)	const
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	return ECGetAlarmHelper(item.Warning, *l);
}

int32_t CDose::ECDeleteWarning(const int32_t n, const base::eAlarmError l)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[n]);
	auto& item = mDGlobal[n].get();
	return ECDeleteAlarmHelper(l, item.Warning);
}

int32_t CDose::ECIsWarning(const int32_t n, const base::eAlarmError l, BOOL* b) const
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	return ECIsAlarmHelper(item.Warning, l, *b);
}

int32_t CDose::ECIsAnyWarning(const int32_t n, BOOL* b) const
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	return ECIsAnyAlarmHelper(item.Warning, *b);
}


int32_t CDose::ECSetFieldWarning(const int32_t n, const base::eAlarmErrorBits& ul)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	auto& item = mDGlobal[n].get();
	if (item.Warning.Test(ul))
	{
		std::lock_guard<std::mutex> lock(mDGlobal[n]);
		item.Warning.Set(ul);
	}
	return DOSIERUNG_OK;
}

int32_t CDose::ECDeleteFieldWarning(const int32_t n)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[n]);
	auto& item = mDGlobal[n].get();
	item.Warning.Delete();
	return DOSIERUNG_OK;
}


int32_t CDose::ECGetFieldWarning(const int32_t n, base::eAlarmErrorBits& ul) const
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	ul = item.Warning.Get();
	return DOSIERUNG_OK;
}


//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
int32_t CDose::ECSetClearWarning(const int32_t n, base::eAlarmError l)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[n]);
	auto& item = mDGlobal[n].get();
	return ECSetAlarmHelper(l, item.ClearWarning);
}


int32_t CDose::ECGetClearWarning(const int32_t n, base::eAlarmError* l) const
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	return ECGetAlarmHelper(item.ClearWarning, *l);
}


int32_t CDose::ECDeleteClearWarning(const int32_t n, const base::eAlarmError l)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[n]);
	auto& item = mDGlobal[n].get();
	return ECDeleteAlarmHelper(l, item.ClearWarning);
}


int32_t CDose::ECIsClearWarning(const int32_t n, const base::eAlarmError l, BOOL* b) const
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	return ECIsAlarmHelper(item.ClearWarning, l, *b);
}


int32_t CDose::ECIsAnyClearWarning(const int32_t n, BOOL* b) const
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	return ECIsAnyAlarmHelper(item.ClearWarning, *b);
}


int32_t CDose::ECSetFieldClearWarning(const int32_t n, const base::eAlarmErrorBits& ul)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	auto& item = mDGlobal[n].get();
	if (item.ClearWarning.Test(ul))
	{
		std::lock_guard<std::mutex> lock(mDGlobal[n]);
		item.ClearWarning.Set(ul);
	}
	return DOSIERUNG_OK;
}


int32_t CDose::ECDeleteFieldClearWarning(const int32_t n)
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[n]);
	auto& item = mDGlobal[n].get();
	item.ClearWarning.Delete();
	return DOSIERUNG_OK;
}


int32_t CDose::ECGetFieldClearWarning(const int32_t n, base::eAlarmErrorBits& ul) const
{
	assert(n >= 0 && n < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[n].get();
	ul = item.ClearWarning.Get();
	return DOSIERUNG_OK;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t CDose::ECSetAlarmCfgBit(const int32_t index, const base::eAlarmCategory k, const base::eAlarmError iBit, BOOL b)
{
	assert(index >= 0 && index < GLOBALDOSEMAXCOUNT);
	assert((b == 1) || (b == 0));
	std::lock_guard<std::mutex> lock(mDGlobal[index]);
	auto& item = mDGlobal[index].get();
	(item.AlarmCfg.value).ulAlarm[_U32(k)].SetAlarmErrorBits(iBit, b);
	return DOSIERUNG_OK;
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t CDose::ECGetAlarmCfgBit(const int32_t index, const base::eAlarmCategory k, const base::eAlarmError iBit, BOOL* b) const
{
	assert(index >= 0 && index < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[index].get();
	*b = (item.AlarmCfg.value).ulAlarm[_U32(k)].IsAlarmErrorBits(iBit);
	return DOSIERUNG_OK;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t CDose::ECGetAlarmCfg(const int32_t index, const base::eAlarmCategory k, base::eAlarmErrorBits* b) const
{
	assert(index >= 0 && index < GLOBALDOSEMAXCOUNT);
	const auto& item = mDGlobal[index].get();
	*b = item.AlarmCfg.value.ulAlarm[_U32(k)];
	return DOSIERUNG_OK;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t CDose::ECSetAlarmCfg(const int32_t index, const base::eAlarmCategory k, const base::eAlarmErrorBits& b)
{
	assert(index >= 0 && index < GLOBALDOSEMAXCOUNT);
	std::lock_guard<std::mutex> lock(mDGlobal[index]);
	auto& item = mDGlobal[index].get();
	item.AlarmCfg.value.ulAlarm[_U32(k)] = b;
	return DOSIERUNG_OK;
}


