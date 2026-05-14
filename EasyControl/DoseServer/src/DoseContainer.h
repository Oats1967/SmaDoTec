//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseContainer
///
/// @file   DoseContainer.h
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

#include <memory>
#include "BASE/include/DoseType.h"
#include "LwfControl.h"
#include "LwsControl.h"
#include "LwpControl.h"
#include "LwvControl.h"
#include "LwbControl.h"
#include "GwbControl.h"
#include "PidControl.h"
#include "WbmControl.h"
#include "WbfControl.h"
#include "IcpControl.h"
#include "IcbControl.h"
#include "NoneControl.h"
#include "VolControl.h"
#include "LclControl.h"
#include "IflControl.h"
#include "IfuControl.h"
#include "IfoControl.h"

class CDosierer
{
public:
	static std::unique_ptr<CDosierer> Create (base::eDoseType uid, int32_t id, loadcell::ILCModuleInterface& rlc);

	CDosierer() = default;
	virtual ~CDosierer(void) = default;
	CDosierer(const CDosierer&) = delete;
	CDosierer& operator=(const CDosierer&) = delete;


	virtual BOOL InitExecute(void) = 0;
	virtual BOOL ExitExecute(void) = 0;
	virtual BOOL Execute(void) = 0;
};

template<class T>
class CDoseContainer : public CDosierer
{
	T m_DoseItem;
public:
	CDoseContainer(int32_t id, loadcell::ILCModuleInterface& rlc) : m_DoseItem(id, rlc)
	{}
	CDoseContainer(int32_t id) : m_DoseItem(id)
	{}
	virtual ~CDoseContainer(void) = default;
	CDoseContainer(const CDoseContainer&) = delete;
	CDoseContainer& operator=(const CDoseContainer&) = delete;

	BOOL InitExecute(void)
	{ return m_DoseItem.InitExecute(); 	}
	BOOL ExitExecute(void)
	{ return m_DoseItem.ExitExecute(); 	}
	BOOL Execute(void)
	{ return m_DoseItem.Execute(); 	}
};




inline std::unique_ptr<CDosierer> CDosierer::Create(base::eDoseType uid, int32_t id, loadcell::ILCModuleInterface& rlc)
{
	switch (uid)
	{
	default:
	case base::eDoseType::DOSETYPE_LWS:
		return std::make_unique< CDoseContainer<CLwsControl>>(id, rlc);
	case base::eDoseType::DOSETYPE_LWP:
		return std::make_unique< CDoseContainer<CLwpControl>>(id, rlc);
	case base::eDoseType::DOSETYPE_LWV:
		return std::make_unique< CDoseContainer<CLwvControl>>(id, rlc);
	case base::eDoseType::DOSETYPE_LWB:
		return std::make_unique< CDoseContainer<CLwbControl>>(id, rlc);
	case base::eDoseType::DOSETYPE_GWB:
		return std::make_unique< CDoseContainer<CGwbControl>>(id, rlc);
	case base::eDoseType::DOSETYPE_WBF:
		return std::make_unique< CDoseContainer<CWbfControl>>(id, rlc);
	case base::eDoseType::DOSETYPE_PID:
		return std::make_unique< CDoseContainer<CPidControl>>(id);
	case base::eDoseType::DOSETYPE_ICP:
		return std::make_unique< CDoseContainer<CIcpControl>>(id);
	case base::eDoseType::DOSETYPE_WBM:
		return std::make_unique< CDoseContainer<CWbmControl>>(id, rlc);
	case base::eDoseType::DOSETYPE_VOL:
		return std::make_unique< CDoseContainer<CVolControl>>(id);
	case base::eDoseType::DOSETYPE_ICB:
		return std::make_unique< CDoseContainer<CIcbControl>>(id);
	case base::eDoseType::DOSETYPE_LCL:
		return std::make_unique< CDoseContainer<CLclControl>>(id, rlc);
	case base::eDoseType::DOSETYPE_IFSO:
		return std::make_unique< CDoseContainer<CIfoControl>>(id);
	case base::eDoseType::DOSETYPE_IFSU:
		return std::make_unique< CDoseContainer<CIfuControl>>(id);
	case base::eDoseType::DOSETYPE_IFL:
		return std::make_unique< CDoseContainer<CIflControl>>(id, rlc);
	case base::eDoseType::DOSETYPE_NONE:
		return std::make_unique< CDoseContainer<CNoneControl>>(id);
	}
	return std::make_unique< CDoseContainer<CLwfControl>>(id, rlc);
}
