//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDoseImpl.h
///
/// @file   AdsDoseImpl.h
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

//***********************************************************************************************************************************
//***********************************************************************************************************************************
#define SIM_LINE_DI(__CLASS, __a) \
int32_t __CLASS::Get ##__a ( BOOL& value) \
{ \
	value = TRUE; \
	return ADS_OK; \
}
//------------------------------------------------------------------
#define DECLARE_LINE_DI(__a) \
int32_t Get ##__a ( BOOL& value);

#define IMPL_LINE_DI(__CLASS, __a, __b) \
int32_t __CLASS::Get ##__a ( BOOL& value) \
{	return GetDigitalInput(__b, value); }

#define IMPL_LINE_DI_TRUE(__CLASS, __a, __b) \
int32_t __CLASS::Get ##__a (BOOL& value) \
{	return GetDigitalInput(__b, value, TRUE); }

//------------------------------------------------------------------
#define DECLARE_LINE_DO(__a) \
int32_t Set ##__a ( const BOOL value);

#define IMPL_LINE_DO(__CLASS, __a, __b) \
int32_t __CLASS::Set ##__a ( const BOOL value) \
{	return SetDigitalOutput(__b, value); }
//------------------------------------------------------------------
#define DECLARE_LINE_AI(__a) \
int32_t Get ##__a ( float32_t& value);

#define IMPL_LINE_AI(__CLASS, __a, __b) \
int32_t __CLASS::Get ##__a ( float32_t& value) \
{	return GetAnalogInput( __b, value); }
//------------------------------------------------------------------
#define DECLARE_LINE_AO(__a) \
int32_t Set ##__a ( const float32_t value);

#define IMPL_LINE_AO(__CLASS, __a, __b) \
int32_t __CLASS::Set ##__a ( const float32_t value) \
{	return SetAnalogOutput( __b, value); }
