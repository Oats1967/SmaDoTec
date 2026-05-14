//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteServerImpl
///
/// @file   RemoteServerImpl.h
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
#include "remote/include/RemoteServiceInterface.h"
#include "RemoteServerInterface.h"
#include "BASE/include/RemoteServerConfig.h"
#include "BASE/include/RemoteState.h"
#include "BASE/include/Statemachine.h"

class CRemoteServerImpl : public remote::IRemoteServerInterface
                        , public base::CTimerStatemachine<int32_t>
{
    private:
        static const uint32_t cRetryMax = 5U;
        static const uint32_t RETRYINTERVALL = 10U;


    private:
        std::shared_ptr<remote::IRemoteInterface> m_remoteinterface;
        remote::IDeviceInterface* m_profiBusInterface;
        remote::IDeviceInterface* m_profiNetInterface;

        remote::IRemoteServiceInterface* m_remoteservice;
        base::CRemoteServerConfig        m_remoteServerConfig;
        uint32_t                         m_t0;
        uint32_t                         m_RetryCounter;
        base::RemoteServerProtocol       m_protocol;
        base::CRemoteState               m_RemoteState;

    private:
        void Connect(remote::RemoteServiceConfig& aMCfg);
        //------------------------------------------------------------------------------------
      ///  @brief   ConnectRemoteService
      ///
      ///           This method ....
      ///
      ///  @param[in] none
      ///  @return RemoteServerProtocol
      ///
      //------------------------------------------------------------------------------------
       void ConnectRemoteService( remote::RemoteServiceConfig& aMCfg);
        //------------------------------------------------------------------------------------
      ///  @brief   OpenRemoteService
      ///
      ///           This method ....
      ///
      ///  @param[in] none
      ///  @return RemoteServerProtocol
      ///
      //------------------------------------------------------------------------------------
       void OpenRemoteService( void );

        //void EnableAnlagenState(const BOOL);

        void setRemoteState(const base::RemoteServerStatus state, const base::RemoteServerError error = base::RemoteServerError::REMOTE_OK)
        {
            m_RemoteState.SetState(state);
            m_RemoteState.SetError(error);
        }


    public:
        CRemoteServerImpl ();
        ~CRemoteServerImpl(void) override;


        void wireRemoteInterface(std::shared_ptr<remote::IRemoteInterface> _RemoteInterface) override;
        void wireProfiBusInterface(remote::IDeviceInterface* _ProfibusInterface)  override;

        //------------------------------------------------------------------------------------
       ///  @brief   Update
       ///
       ///           This method ....
       ///
       ///  @param[in] none
       ///  @return BOOL
       ///
       //------------------------------------------------------------------------------------
        void Update() override;

        //------------------------------------------------------------------------------------
        ///  @brief   Close
        ///
        ///           This method ....
        ///
        ///  @param[in] none
        ///  @return BOOL
        ///
        //------------------------------------------------------------------------------------
        BOOL Close(void) override;
        //------------------------------------------------------------------------------------
       ///  @brief   Open
       ///
       ///           This method ....
       ///
       ///  @param[in] none
       ///  @return BOOL
       ///
       //------------------------------------------------------------------------------------
        BOOL Open(const std::string& pathtoXml) override;

        //------------------------------------------------------------------------------------
        ///  @brief   getRemoteState
        ///
        ///           This method ....
        ///
        ///  @param[in] none
        ///  @return base::RemoteServerError
        ///
        //------------------------------------------------------------------------------------
        base::RemoteServerStatus getRemoteState() const override
        {   return m_RemoteState.GetState();   }

        //------------------------------------------------------------------------------------
        ///  @brief   getRemoteError
        ///
        ///           This method ....
        ///
        ///  @param[in] none
        ///  @return base::RemoteServerError
        ///
        //------------------------------------------------------------------------------------
        base::RemoteServerError getRemoteError() const override
        {  return m_RemoteState.GetError();    }

        void SetInputOffline(BOOL bOffline) override;
};


