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


class CAdsBusRS485Sim : public IAdsBusInterface
{
    const int32_t g_MaxDoseAvail = GLOBALDOSEMAXCOUNT;

public:
    CAdsBusRS485Sim() = default;
    ~CAdsBusRS485Sim() = default;

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
inline int32_t CAdsBusRS485Sim::Open(const uint16_t iTaskAdress)
{   return ADS_OK; }

//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusRS485Sim::Close(void)
{   return ADS_OK; }
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline void CAdsBusRS485Sim::Reset(void)
{}
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusRS485Sim::SendReceiveDataFromDoseIndex(const int32_t, LPCSTR, LPSTR, int32_t*, BOOL, uint64_t, BOOL)
{   return ADS_OK;}
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusRS485Sim::ClampInit(const int32_t clampindex)
{  return ADS_OK;}
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusRS485Sim::DoseInit(const int32_t index)
{ return ADS_OK;}
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusRS485Sim::GetClampCount() const
{   return 1; }
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
inline int32_t CAdsBusRS485Sim::GetMaxDosePerClampCount() const
{    return GLOBALDOSEMAXCOUNT; }
//****************************************************************************************
//****************************************************************************************
inline int32_t   CAdsBusRS485Sim::GetMaxSlaveCount() const
{    return g_MaxDoseAvail + 1; }
//****************************************************************************************
//****************************************************************************************
inline int32_t   CAdsBusRS485Sim::GetMaxDoseAvailCount() const
{   return g_MaxDoseAvail;}
//****************************************************************************************
//****************************************************************************************
inline uint8_t   CAdsBusRS485Sim::GetBusAddress(const int32_t index) const
{   return 0; }
//****************************************************************************************
//****************************************************************************************
inline int32_t   CAdsBusRS485Sim::GetZeroIndexForDoseIndex(const int32_t index) const
{    return 0; }









