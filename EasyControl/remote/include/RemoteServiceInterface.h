//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteServiceInterface
///
/// @file   RemoteServiceInterface.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "RemoteServiceConfig.h"

namespace remote
{

class IRemoteServiceInterface
{
public:
    IRemoteServiceInterface() = default;
    virtual ~IRemoteServiceInterface() = default;

    IRemoteServiceInterface(const IRemoteServiceInterface&) = delete;
    IRemoteServiceInterface& operator=(const IRemoteServiceInterface&) = delete;

    virtual BOOL Open(RemoteServiceConfig&) = 0;
    virtual BOOL Close() = 0;

    virtual base::RemoteServerStatus getRemoteState() const = 0;
    virtual base::RemoteServerError  getRemoteError() const = 0;
    virtual void SetInputOffline(BOOL bOffline) = 0;
};

};


