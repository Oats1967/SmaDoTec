//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsHelper
///
/// @file   AdsHelper.h
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

#include <functional>
#include "AdsClient/include/AdsTypen.h"
#include "AdsClient/src/private/AdsHelperTypes.h"
#include "AdsClient/src/private/AdsHelperProfibus.h"


inline LongHandle MakeLongHandle(const uint16_t portaddress, const uint32_t hdl)
{
    return (_U64(portaddress) << 32U) | _U64(hdl);
}

inline std::pair<uint16_t, uint32_t> ExtractLongHandle(const LongHandle& value)
{
    return std::make_pair(_U16((value>> 32)&0xFFFF), _U32(value & 0xFFFFFFFF));
}

#if 0
//------------------------------------------------------------------------------------
///  @brief   Ads_GetStartTime : This method returns the current tick count and checks for wrap around
///
///  @param[in] offset , uint32_t
///  @return uint32_t
///
//------------------------------------------------------------------------------------
uint64_t AdsHelperGetStartTime(uint64_t offset );
#endif


int32_t AdsHelperAnalogInput(const uint16_t iPortAddress, const uint32_t& hdl, float32_t& input);
int32_t AdsHelperAnalogOutput(const uint16_t iPortAddress, const uint32_t& hdl, const float32_t output);
int32_t AdsHelperDigitalOutput(const uint16_t iPortAddress, const uint32_t& hdl, const BOOL output);
int32_t AdsHelperDigitalInput(const uint16_t iPortAddress, const uint32_t& hdl, BOOL& input);
int32_t AdsHelperStatusInput(const uint16_t iPortAddress, const uint32_t& hdl, uint16_t& status);
int32_t AdsHelperStreamOutput(const uint16_t iPortAddress, const uint32_t& hdl, const char_t* output, const uint32_t length);
int32_t AdsHelperStreamInput(const uint16_t iPortAddress, const uint32_t& hdl,  char_t* output, uint32_t& length);


using AdsDigitalInputChanged = std::function < void (const uint32_t, BOOL)>;
int32_t AdsHelperRegisterInputCallback(const uint16_t iAddress, const uint32_t& hdl, AdsDigitalInputChanged);
int32_t AdsHelperUnregisterInputCallback(const uint16_t iAddress, const uint32_t& hdl);

//------------------------------------------------------------------------------------
///  @brief   AdsGetHandle
///
///  @param[in] iPortAddress [ 300, 301 ]
///  @param[in] symbol symbol name
///  @param[in] hdl		 Handle to be allocated
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t AdsHelperGetHandle(const uint16_t iPortAddress, LPSTR symbol, uint32_t& hdl);

//------------------------------------------------------------------------------------
///  @brief   AdsReleaseHandle
///
///  @param[in] iPortAddress [ 300, 301 ]
///  @param[in] hdl		 Handle to be released
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t AdsHelperReleaseHandle(const uint16_t iPortAddress, uint32_t& hdl);


int32_t AdsHelperRS485Open(const uint16_t iPortAddress, BUSHANDLECTRL& ahCtrl);
int32_t AdsHelperRS485Close(const uint16_t iPortAddress, BUSHANDLECTRL& ahCtrl);
int32_t AdsHelperRS485SendReceiveData(const uint16_t iPortAddress, BUSHANDLECTRL& aCtrl, LPCSTR out, LPSTR in, int32_t* pN, BOOL bReceive, uint64_t dTimeOut, BOOL bRetry);

int32_t AdsHelperEtherCATSendReceiveData(const uint16_t iPortAddress, BUSHANDLECTRL& aCtrl, LPCSTR out, LPSTR in, int32_t* pN, BOOL bReceive, uint64_t dTimeOut, BOOL bRetry);
int32_t AdsHelperEtherCATOpen(const uint16_t iPortAddress, BUSHANDLECTRL& ahCtrl);
int32_t AdsHelperEtherCATClose(const uint16_t iPortAddress, BUSHANDLECTRL& ahCtrl);


int32_t AdsHelperOpen();
int32_t AdsHelperClose();



