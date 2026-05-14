//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteServiceImpl
///
/// @file   RemoteServiceImpl.h
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

#include "BASE/Task/public/ThreadModul.h"
#include "BASE/include/RemoteState.h"
#include "remote/include/RemoteServiceInterface.h"
#include "remote/include/DeviceHandler.h"

namespace remote
{
class CRemoteServiceImpl : public IRemoteServiceInterface
                         , public base::task::CThreadModul
{
    CDeviceHandler            m_DataHandler;
    base::CRemoteState        m_RemoteState;
    BOOL                      m_open;

protected:
    virtual IDeviceInterface* GetDeviceInterface( RemoteServiceConfig& aCfg) = 0;
    virtual void ReleaseDevice() = 0;

    void setRemoteState(const base::RemoteServerStatus state, const base::RemoteServerError error = base::RemoteServerError::REMOTE_OK)
    {
        m_RemoteState.SetState(state);
        m_RemoteState.SetError(error);
    }

protected:
    //------------------------------------------------------------------------------------
    ///  @brief   execute
    ///
    ///           This method ....
    ///
    ///  @param[in] none
    ///  @return int32_t
    ///
    //------------------------------------------------------------------------------------
    int32_t execute(void) override;

public:
    CRemoteServiceImpl() : m_DataHandler()
        , m_open(FALSE)
    {}

    ~CRemoteServiceImpl() = default;

    //------------------------------------------------------------------------------------
    ///  @brief   Open
    ///
    ///  @param[in] aCfg , const EXTINTERFACESERVERCFG&
    ///  @return eEXTINTERFACESERVERERROR
    ///
    //------------------------------------------------------------------------------------
    BOOL Open(RemoteServiceConfig&) override;
    BOOL Close() override;

    base::RemoteServerStatus getRemoteState() const override;
    base::RemoteServerError  getRemoteError() const  override;
    void SetInputOffline(BOOL bOffline) override;
};
};



