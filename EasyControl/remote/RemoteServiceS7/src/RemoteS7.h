//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteS7Leistritz
///
/// @file   RemoteS7Leistritz.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "RemoteServiceImpl.h"

namespace remote
{
class CRemoteS7 : public CRemoteServiceImpl
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
    CRemoteS7() = default;
    ~CRemoteS7() = default;
};

};


