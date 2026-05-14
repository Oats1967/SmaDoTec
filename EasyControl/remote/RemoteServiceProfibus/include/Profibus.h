//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ProfiBusSlave.h
///
/// @file   ProfiBusSlave.h
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

#include "remote/RemoteHandler/include/RemoteServiceImpl.h"

namespace remote
{
class CProfibus : public CRemoteServiceImpl
{
protected:
    IDeviceInterface* GetDeviceInterface( RemoteServiceConfig& aCfg) override;
    void ReleaseDevice() override;

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
    CProfibus() = default;
    ~CProfibus() = default;
};
};



