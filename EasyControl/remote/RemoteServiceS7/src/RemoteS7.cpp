//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteS7
///
/// @file   RemoteS7.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cstdio>
#include <cassert>
#include "BASE/Task/public/Task.h"
#include "remote/S7Simulator/include/S7Simulator64.h"
#include "remote/S7Snap/include/S7Snap64.h"
#include "include/RemoteS7.h"
#include "Logger/include/Logger.h"


namespace remote
{

//**********************************************************************************************
//**********************************************************************************************
int32_t CRemoteS7::execute(void)
{
	auto result = CRemoteServiceImpl::execute();
	base::task::Sleep(300);
	return result;
}
//**********************************************************************************************
//**********************************************************************************************
IDeviceInterface* CRemoteS7::GetDeviceInterface( remote::RemoteServiceConfig& aCfg)
{
	remote::IDeviceInterface* pDeviceInterface = nullptr;
	switch (aCfg.iProtokoll)
	{
	default:
	case base::RemoteServerProtocol::REMOTE_PROTOCOL_S7SNAP:
		GetS7Snap64Interface(&pDeviceInterface);
		break;
	case base::RemoteServerProtocol::REMOTE_PROTOCOL_S7SIMULATOR:
		GetS7SimulatorInterface(&pDeviceInterface);
		break;
	}
	return pDeviceInterface;
}
//**********************************************************************************************
//**********************************************************************************************
void CRemoteS7::ReleaseDevice()
{
	ReleaseS7Snap64Interface();
	ReleaseS7SimulatorInterface();
};


};




