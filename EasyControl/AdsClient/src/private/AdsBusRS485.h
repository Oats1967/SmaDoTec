//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsBusRS485.h
///
/// @file   AdsBusRS485.h
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

#include "AdsClient/src/private/AdsHelperTypes.h"
#include "AdsClient/src/private/AdsBusInterface.h"

class CAdsBusRS485 : public IAdsBusInterface
{
    enum
    {
        eINPUTS_DSIO_PESAIN,
        eINPUTS_DSIO_PESASTATUS,
        eOUTPUTS_DSIO_PESAOUT,
        eOUTPUTS_DSIO_PESACTRL,
        eHandleMax
    };

    uint16_t    g_PortAddress;
    int32_t     g_MaxClampCount;
    int32_t     g_MaxDosePerClamp;
    int32_t     g_MaxDoseAvail;
    BOOL        m_bETCOpen;
    uint8_t     g_BusAddress[ADS_LC_SLAVECOUNT];    // welche Busadresse für Dosierer
    uint8_t     g_Clamp[ADS_LC_SLAVECOUNT];         // welche Klemme für Dosierer

    BUSHANDLECTRL  g_HandleCtrl[GLOBALDOSEMAXCOUNT];

private:
    BUSHANDLECTRL& GetHandleFromClampIndex(const int32_t index);
    BUSHANDLECTRL& GetHandleFromDoseIndex(const int32_t index);
    BOOL GetClampStatus(const int32_t clampindex) const;
    void     ClampInit();
    int32_t  SendReceiveDataFromClampIndex(const int32_t clampindex, LPCSTR out, LPSTR in, int32_t* pN, BOOL bReceive, uint64_t dTimeOut, BOOL bRetry);
    void     Reset(const int32_t clampindex);
    void     LoadClampIx(void);

public:
    CAdsBusRS485();
    virtual ~CAdsBusRS485() override;

    int32_t Open(const uint16_t iTaskAdress) override;
    int32_t Close(void) override;;
    void    Reset(void) override;;

    int32_t   SendReceiveDataFromDoseIndex(const int32_t index, LPCSTR out, LPSTR in, int32_t* pN, BOOL bReceive, uint64_t dTimeOut, BOOL bRetry) override;;
    int32_t   ClampInit(const int32_t clampindex) override;;
    int32_t   DoseInit(const int32_t index) override;;

    int32_t   GetClampCount() const  override
    {  return g_MaxClampCount;    }

    int32_t   GetMaxDosePerClampCount() const  override
    {  return g_MaxDosePerClamp;    }

    int32_t   GetMaxSlaveCount() const  override
    {    return g_MaxDoseAvail + g_MaxClampCount;    }

    int32_t   GetMaxDoseAvailCount() const  override
    {  return g_MaxDoseAvail;    }

    uint8_t  GetBusAddress(const int32_t index) const  override
    {
        assert((index >= 0) && (index < _S32(__ARRAYSIZE(g_BusAddress))));
        return g_BusAddress[index];
    }

    int32_t   GetZeroIndexForDoseIndex(const int32_t index) const override;


    static int32_t GetInterface(IAdsBusInterface**);
    static int32_t ReleaseInterface();
};



//****************************************************************************************
//****************************************************************************************
inline BOOL CAdsBusRS485::GetClampStatus(const int32_t clampindex) const
{
    return clampindex < g_MaxClampCount;
}
//****************************************************************************************
//****************************************************************************************
inline int32_t CAdsBusRS485 :: CAdsBusRS485::GetZeroIndexForDoseIndex(const int32_t index) const
{
    return (g_MaxDosePerClamp > 0) ? (g_MaxDoseAvail + index / g_MaxDosePerClamp) : 0;
}









