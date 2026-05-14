//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteServiceConfig
///
/// @file   RemoteServiceConfig.h
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
#include "BASE/include/RemoteConfig.h"
#include "remote/include/RemoteInterface.h"
#include "remote/include/DeviceInterface.h"


namespace remote
{
typedef struct
{
	base::RemoteServerProtocol iProtokoll;
	std::shared_ptr<IRemoteInterface> pRemoteInterface;
	IDeviceInterface* pProfiBusInterface;
	std::string szDeviceConfigXml;
	std::string szTableConfigXml;
} RemoteServiceConfig;

};



