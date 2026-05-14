//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteServerImpl.cpp
///
/// @file   RemoteServerImpl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include "BASE/Utils/public/xml/RemoteServerXml.h"
#include "RemoteServerImpl.h"
#include "RemoteServiceModbusStandard.h"
#include "RemoteServiceS7.h"
#include "RemoteServiceProfibus.h"
#include "Logger/include/Logger.h"



using namespace remote;

//********************************************************************************************************************************
//********************************************************************************************************************************
CRemoteServerImpl::CRemoteServerImpl() :  m_remoteinterface{ nullptr } 
                                         , m_profiBusInterface { nullptr }
                                         , m_profiNetInterface{ nullptr }
                                         , m_remoteservice{ nullptr }
                                         , m_t0(0)
                                         , m_RetryCounter (0L)
                                         , m_protocol(base::RemoteServerProtocol::REMOTE_PROTOCOL_NONE)
                                         , m_RemoteState {}
{}
//********************************************************************************************************************************
//********************************************************************************************************************************
CRemoteServerImpl :: ~CRemoteServerImpl (void)
{
    Close();
}
//********************************************************************************************************************************
//********************************************************************************************************************************
void CRemoteServerImpl::wireRemoteInterface(std::shared_ptr<remote::IRemoteInterface> _RemoteInterface)
{
    m_remoteinterface = _RemoteInterface;
}
//********************************************************************************************************************************
//********************************************************************************************************************************
void CRemoteServerImpl::wireProfiBusInterface(remote::IDeviceInterface* _Interface)
{
    m_profiBusInterface = _Interface;
}
//********************************************************************************************************************************
//********************************************************************************************************************************
void CRemoteServerImpl::Connect(RemoteServiceConfig& aMCfg)
{
    if (!m_remoteservice)
    {
        setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_ERROR, base::RemoteServerError::REMOTE_ERROR_OPEN_ERROR);
    }
    else
    {
        (void)m_remoteservice->Open(aMCfg);
        setRemoteState(m_remoteservice->getRemoteState(), m_remoteservice->getRemoteError());
    }
}
//********************************************************************************************************************************
//********************************************************************************************************************************
void CRemoteServerImpl::ConnectRemoteService(RemoteServiceConfig& aMCfg)
{
    if (m_remoteservice)
    {
        (void)m_remoteservice->Close();
        m_remoteservice = nullptr;
    }
    switch (aMCfg.iProtokoll)
    {
        default:
        case base::RemoteServerProtocol:: REMOTE_PROTOCOL_NONE:
        {
            setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_DISABLED);
        }
        break;

        case base::RemoteServerProtocol::REMOTE_PROTOCOL_OPENMODBUS:
        {
            GetRemoteServiceModbusStandardInterface(&m_remoteservice);
            Connect(aMCfg);
        }
        break;

        case base::RemoteServerProtocol::REMOTE_PROTOCOL_S7SNAP:
        case base::RemoteServerProtocol::REMOTE_PROTOCOL_S7SIMULATOR:
        {
            GetRemoteServiceS7Interface(&m_remoteservice);
            Connect(aMCfg);
        }
        break;

        case base::RemoteServerProtocol::REMOTE_PROTOCOL_PROFIBUS:
        case base::RemoteServerProtocol::REMOTE_PROTOCOL_PROFINET:
        {
            aMCfg.pProfiBusInterface = m_profiBusInterface;
            GetRemoteServiceProfibusInterface(&m_remoteservice);
            Connect(aMCfg);
        }

        break;
    }
}
//********************************************************************************************************************************
//********************************************************************************************************************************
void CRemoteServerImpl::OpenRemoteService(void)
{
    RemoteServiceConfig aMCfg;

    const auto& field       = m_remoteServerConfig;
    aMCfg.szDeviceConfigXml = field.m_DevicePath;
    aMCfg.szTableConfigXml  = field.m_TablePath;
    aMCfg.iProtokoll        = field.m_Protocol;
    aMCfg.pRemoteInterface  = m_remoteinterface;
    ConnectRemoteService(aMCfg);

    const auto status = getRemoteState();
    const BOOL disableAnlage = (status == base::RemoteServerStatus::REMOTE_STATUS_CONNECTED ||
                                status == base::RemoteServerStatus::REMOTE_STATUS_OPEN);
    //EnableAnlagenState(! disableAnlage);
}
//********************************************************************************************************************************
//********************************************************************************************************************************
void CRemoteServerImpl ::Update()
{
    CTimerStatemachine::Execute();

    switch (m_sStep)
    {
    case 0:     // Erstmalige Initialisierung
    {
        (void)OpenRemoteService();
        if (getRemoteState() == base::RemoteServerStatus::REMOTE_STATUS_ERROR)
        {
            m_t0 = m_st + RETRYINTERVALL;
            m_sStep = 1;
        }
        else
        {
            m_RetryCounter = cRetryMax;
            m_sStep = 2;
        }
    }
    break;

    case 1: // Wiederholte Initialisierung
    {
        if (m_st >= m_t0)
        {
            (void)OpenRemoteService();
            if (getRemoteState() == base::RemoteServerStatus::REMOTE_STATUS_ERROR)
            {
                m_t0 = m_st + RETRYINTERVALL;
            }
            else
            {
                m_RetryCounter = cRetryMax;
                m_sStep = 2;
            }
        }
    }
    break;

    case 2: // Update
    {
        if (m_remoteservice != nullptr)
        {
            auto state = m_remoteservice->getRemoteState();
            auto error = m_remoteservice->getRemoteError();
            if (state == base::RemoteServerStatus::REMOTE_STATUS_CONNECTED)
            {
                m_RetryCounter = cRetryMax;
            }
            else if (state == base::RemoteServerStatus::REMOTE_STATUS_ERROR)
            {
                assert(m_RetryCounter > 0);
                if (--m_RetryCounter == 0)
                {
                    m_sStep = 0;
                }
            }
            setRemoteState(state, error);
        }
    }
    break;

    default:
        assert(FALSE);
        break;
    }
}
//********************************************************************************************************************************
//********************************************************************************************************************************
BOOL CRemoteServerImpl ::Open (const std::string& pathtoxml)
{
    if ( ! IsInit())
    {
        CTimerStatemachine::InitExecute();

        base::xml::CRemoteServerXml xmlfile;

        auto result = xmlfile.Load(pathtoxml);
        if (result == FALSE)
        {
            LOGERROR("Opening RemoteServer failed ! " << pathtoxml);
            setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_ERROR, base::RemoteServerError::REMOTE_ERROR_XMLREADER_FAILED);
            return result;
        }
        m_remoteServerConfig = xmlfile.Get();
        m_protocol = m_remoteServerConfig.m_Protocol;
        CTimerStatemachine::Start();
        setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_OPEN);
    }
    return TRUE;
}
//********************************************************************************************************************************
//********************************************************************************************************************************
BOOL CRemoteServerImpl ::Close ( void )
{
    if (IsInit())
    {
        if (m_remoteservice != nullptr)
        {
            (void)m_remoteservice->Close();
            m_remoteservice = nullptr;
        }
        setRemoteState(base::RemoteServerStatus::REMOTE_STATUS_CLOSED);
        ReleaseRemoteServiceModbusStandardInterface();
        ReleaseRemoteServiceS7Interface();
        ReleaseRemoteServiceProfibusInterface();
        CTimerStatemachine::ExitExecute();
    }
    return TRUE;
}
//********************************************************************************************************************************
//********************************************************************************************************************************
void CRemoteServerImpl::SetInputOffline(BOOL bOffline)
{
    if (m_remoteservice != nullptr)
    {
        (void)m_remoteservice->SetInputOffline(bOffline);
    }
}




