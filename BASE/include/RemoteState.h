//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteState
///
/// @file   RemoteState.h
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

#include "BASE/Base.h"
#include "BASE/include/RemoteConfig.h"

namespace base
{
class CRemoteState
{
private:
    base::RemoteServerStatus m_State;
    base::RemoteServerError  m_Error;

public:
    CRemoteState() : m_State(base::RemoteServerStatus::REMOTE_STATUS_DISCONNECTED)
                    , m_Error(base::RemoteServerError::REMOTE_OK)
    {}
    ~CRemoteState() = default;
    //------------------------------------------------------------------------------------
    ///  @brief   setRemoteState
    ///
    ///           This method ....
    ///
    ///  @param[in] none
    ///  @return remote::RemoteServerError
    ///
    //------------------------------------------------------------------------------------
    SETGET(base::RemoteServerStatus, State)
    SETGET(base::RemoteServerError, Error)
};

};


