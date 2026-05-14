//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteServer.h
///
/// @file   RemoteServer.h
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


#include <string>
#include <memory>
#include "BASE/include/RemoteConfig.h"
#include "remote/include/RemoteInterface.h"
#include "remote/include/DeviceInterface.h"

namespace remote
{
class IRemoteServerInterface
{
public:
    IRemoteServerInterface() = default;
    virtual ~IRemoteServerInterface(void) = default;
    IRemoteServerInterface(const IRemoteServerInterface&) = delete;
    IRemoteServerInterface& operator = (const IRemoteServerInterface&) = delete;

    virtual void wireRemoteInterface(std::shared_ptr<remote::IRemoteInterface> _RemoteInterface) = 0;
    virtual void wireProfiBusInterface(remote::IDeviceInterface* _ProfibusInterface) = 0;


    //--------------------- ---------------------------------------------------------------
    ///  @brief   Update
    ///
    ///           This method ....
    ///
    ///  @param[in] none
    ///  @return BOOL
    ///
    //------------------------------------------------------------------------------------
    virtual void Update() = 0;

    //------------------------------------------------------------------------------------
    ///  @brief   Close
    ///
    ///           This method ....
    ///
    ///  @param[in] none
    ///  @return BOOL
    ///
    //------------------------------------------------------------------------------------
    virtual BOOL Close(void) = 0;
    //------------------------------------------------------------------------------------
    ///  @brief   Open
    ///
    ///           This method ....
    ///
    ///  @param[in] none
    ///  @return BOOL
    ///
    //------------------------------------------------------------------------------------
    virtual BOOL Open(const std::string& pathtoXml) = 0;

    //------------------------------------------------------------------------------------
    ///  @brief   getRemoteState
    ///
    ///           This method ....
    ///
    ///  @param[in] none
    ///  @return base::RemoteServerError
    ///
    //------------------------------------------------------------------------------------
    virtual base::RemoteServerStatus getRemoteState() const = 0;

    //------------------------------------------------------------------------------------
///  @brief   getRemoteError
///
///           This method ....
///
///  @param[in] none
///  @return base::RemoteServerError
///
//------------------------------------------------------------------------------------
    virtual base::RemoteServerError getRemoteError() const = 0;

    virtual void SetInputOffline(BOOL bOffline) = 0;
    //virtual void SetSlaveMode() = 0;


};

};


