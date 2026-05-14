//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ProfiBus.cpp
///
/// @file   ProfiBus.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include "BASE/Task/public/Task.h"
#include "Logger/include/Logger.h"
#include "include/Profibus.h"


namespace remote
{
//************************************************************************************
//************************************************************************************
int32_t CProfibus::execute(void)
{
	auto result = CRemoteServiceImpl::execute();
	base::task::Sleep(100);
	return result;
}
//**********************************************************************************************
//**********************************************************************************************
IDeviceInterface* CProfibus::GetDeviceInterface( RemoteServiceConfig& aCfg)
{
	IDeviceInterface* pDeviceInterface = nullptr;

	switch (aCfg.iProtokoll)
	{
	case base::RemoteServerProtocol::REMOTE_PROTOCOL_PROFIBUS:
	case base::RemoteServerProtocol::REMOTE_PROTOCOL_PROFINET:
	{
		pDeviceInterface = aCfg.pProfiBusInterface;
	}
	break;

	default:
		assert(FALSE);
		break;
	}
	return pDeviceInterface;
}
//**********************************************************************************************
//**********************************************************************************************
void CProfibus::ReleaseDevice()
{
};
};


