//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseServerImpl.h
///
/// @file   DoseServerImpl.h
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
#include <string>
#include "remote/include/RemoteInterface.h"
#include "RemoteServer/include/RemoteServerInterface.h"
#include "BASE/include/SystemConfig.h"
#include "ErrorCodes.h"



class IDoseServerInterface 
{
public:
	IDoseServerInterface(void) = default;
	virtual ~IDoseServerInterface(void) = default;

	IDoseServerInterface(const IDoseServerInterface&) = delete;
	IDoseServerInterface& operator=(const IDoseServerInterface&) = delete;


	virtual BOOL Open(const std::string& _path2DoseServer)  = 0;
    virtual BOOL Close	( void ) = 0;
	virtual BOOL Start(void)= 0;
	virtual BOOL Stop(void)	= 0;
	virtual remote::IRemoteInterface* GetRemoteInterface() = 0;
	virtual remote::IRemoteServerInterface* GetRemoteServerInterface() = 0;
	virtual BOOL IsStarted(void) const = 0;
	virtual ErrorCodes GetLastErrorCode(void) = 0;

	virtual void SetSystemConfig(const base::CSystemConfig&) = 0;
	virtual const base::CSystemConfig& GetSystemConfig() const = 0;
	virtual void UpdateSystemConfig()= 0;
	virtual BOOL SaveData() = 0;

};



