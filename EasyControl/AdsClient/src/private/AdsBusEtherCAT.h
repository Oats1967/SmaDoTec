//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsEtherCAT.h
///
/// @file   AdsEtherCAT.h
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

#include "AdsClient/src/private/AdsHelper.h"
#include "AdsClient/src/private/AdsBusInterface.h"


class CAdsBusEtherCAT : public IAdsBusInterface
{
    enum
    {
        eINPUTS_DSIO_LAUMASIN = 0,
        eINPUTS_DSIO_LAUMASSTATUS,
        eOUTPUTS_DSIO_LAUMASOUT,
        eHandleMax
    };

    uint16_t                g_PortAddress;
    BOOL                    m_bETCOpen;
    int32_t                 m_MaxDoseAvail;
    BUSHANDLECTRL           m_HandleCtrl[GLOBALDOSEMAXCOUNT];

private:
    BUSHANDLECTRL& GetHandleFromDoseIndex(const int32_t index);

public:
    CAdsBusEtherCAT();
    ~CAdsBusEtherCAT() override;

    int32_t Close(void) override;;
    int32_t Open(const uint16_t iTaskAdress) override;
    void    Reset(void)  override;

    int32_t   SendReceiveDataFromDoseIndex(const int32_t index, LPCSTR out, LPSTR in, int32_t* pN, BOOL bReceive, uint64_t dTimeOut, BOOL bRetry)  override;
    int32_t   ClampInit(const int32_t clampindex)  override;
    int32_t   DoseInit(const int32_t index)  override;

    int32_t   GetClampCount() const  override;
    int32_t   GetMaxDosePerClampCount() const  override;
    int32_t   GetMaxSlaveCount() const  override;
    int32_t   GetMaxDoseAvailCount() const  override;
    uint8_t   GetBusAddress(const int32_t index) const  override;
    int32_t   GetZeroIndexForDoseIndex(const int32_t index) const  override;


    static int32_t GetInterface(IAdsBusInterface** pCAdsBusEtherCAT);
    static int32_t ReleaseInterface();

};

//****************************************************************************************
//****************************************************************************************
inline int32_t   CAdsBusEtherCAT :: GetClampCount() const
{    return m_MaxDoseAvail; }
//****************************************************************************************
//****************************************************************************************
inline int32_t   CAdsBusEtherCAT::GetMaxDosePerClampCount() const
{   return __ARRAYSIZE(m_HandleCtrl); }
//****************************************************************************************
//****************************************************************************************
inline int32_t   CAdsBusEtherCAT::GetMaxSlaveCount() const
{    return m_MaxDoseAvail; }
//****************************************************************************************
//****************************************************************************************
inline int32_t   CAdsBusEtherCAT::GetMaxDoseAvailCount() const
{   return m_MaxDoseAvail;}
//****************************************************************************************
//****************************************************************************************
inline uint8_t   CAdsBusEtherCAT::GetBusAddress(const int32_t ) const
{   return 0; }
//****************************************************************************************
//****************************************************************************************
inline int32_t   CAdsBusEtherCAT::GetZeroIndexForDoseIndex(const int32_t ) const
{    return 0; }







