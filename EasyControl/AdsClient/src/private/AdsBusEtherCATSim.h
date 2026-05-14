//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsEtherCATSim.h
///
/// @file   AdsEtherCATSim.h
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


class CAdsBusEtherCATSim : public IAdsBusInterface
{
    const int32_t g_MaxDoseAvail = GLOBALDOSEMAXCOUNT;

public:
    CAdsBusEtherCATSim() = default;
    ~CAdsBusEtherCATSim() = default;

    int32_t Open(const uint16_t iTaskAdress) override;
    int32_t Close(void) override;
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

//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusEtherCATSim::Open(const uint16_t iTaskAdress)
{   return ADS_OK; }

//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusEtherCATSim::Close(void)
{   return ADS_OK; }
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline void CAdsBusEtherCATSim::Reset(void)
{}
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusEtherCATSim::SendReceiveDataFromDoseIndex(const int32_t, LPCSTR, LPSTR, int32_t*, BOOL, uint64_t, BOOL)
{   return ADS_OK;}
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusEtherCATSim::ClampInit(const int32_t clampindex)
{  return ADS_OK;}
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusEtherCATSim::DoseInit(const int32_t index)
{ return ADS_OK;}
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusEtherCATSim::GetClampCount() const
{   return 1; }
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusEtherCATSim::GetMaxDosePerClampCount() const
{    return GLOBALDOSEMAXCOUNT; }
//****************************************************************************************
//****************************************************************************************
inline int32_t   CAdsBusEtherCATSim::GetMaxSlaveCount() const
{    return g_MaxDoseAvail + 1; }
//****************************************************************************************
//****************************************************************************************
inline int32_t   CAdsBusEtherCATSim::GetMaxDoseAvailCount() const
{   return g_MaxDoseAvail;}
//****************************************************************************************
//****************************************************************************************
inline uint8_t   CAdsBusEtherCATSim::GetBusAddress(const int32_t index) const
{   return 0; }
//****************************************************************************************
//****************************************************************************************
inline int32_t   CAdsBusEtherCATSim::GetZeroIndexForDoseIndex(const int32_t index) const
{    return 0; }









