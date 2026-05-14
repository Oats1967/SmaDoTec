//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsHelperProfibus
///
/// @file   AdsHelperProfibus.h
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

#include "AdsClient/include/AdsTypen.h"
#include "BASE/include/ProfibusTypes.h"

//------------------------------------------------------------------------------------
///  @brief   AdsHelperProfibusOpen
///
///           This method initiliazes the profibus device
///
///  @author  Detlef Hafer
///
///
///  @param[in] iPortAddress , portaddress
///  @param[in] protocol , protocol to be used
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t AdsHelperProfibusOpen(const uint16_t iPortAddress, const base::eProfibusProtocol protocol);

//------------------------------------------------------------------------------------
///  @brief   AdsHelperProfibusClose
///
///           This method closes the profibus device
///
///  @author  Detlef Hafer
///
///
///  @param[in] none
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t AdsHelperProfibusClose();


//------------------------------------------------------------------------------------
///  @brief   AdsHelperProfibusReadStatus
///
///           This method reads status from profibus device
///
///  @author  Detlef Hafer
///
///
///  @param[in] iPortAddress , portaddress
///  @param[in] hdl , handle to symbolic address
///  @param[out] rStatus , status
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t AdsHelperProfibusReadStatus(const uint16_t iPortAddress, const uint32_t& hdl, uint16_t& rStatus);

//------------------------------------------------------------------------------------
///  @brief   AdsHelperProfibusWrite
///
///           This method writes a stream of profibus data data to the output
///
///  @author  Detlef Hafer
///
///
///  @param[in] pData , const PROFIBUSDATA*
///  @return short
///
//------------------------------------------------------------------------------------
int32_t AdsHelperProfibusWrite(const uint16_t iPortAddress, const uint32_t& hdl, const base::ProfibusData& rData);


//------------------------------------------------------------------------------------
///  @brief   AdsHelperProfibusRead
///
///           This method reads data from input buffer
///
///  @author  Detlef Hafer
///
///
///  @param[in] aData , PROFIBUSDATA&
///  @return int32_t
///
//------------------------------------------------------------------------------------
int32_t AdsHelperProfibusRead(base::ProfibusData& rData);
