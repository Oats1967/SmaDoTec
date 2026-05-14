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
#define SIM_DI(__CLASS, __a) \
int32_t __CLASS::Get ##__a ( int32_t index, BOOL& value) \
{ \
	value = TRUE; \
	return ADS_OK; \
}
//------------------------------------------------------------------
#define DECLARE_DI(__a) \
int32_t Get ##__a ( int32_t index, BOOL& value);

#define IMPL_DI(__CLASS, __a, __b) \
int32_t __CLASS::Get ##__a ( int32_t index, BOOL& value) \
{	return GetDigitalInput(index, __b, value); }

#define IMPL_DI_TRUE(__CLASS, __a, __b) \
int32_t __CLASS::Get ##__a ( int32_t index, BOOL& value) \
{	return GetDigitalInput(index, __b, value, TRUE); }

//------------------------------------------------------------------
#define DECLARE_DO(__a) \
int32_t Set ##__a ( int32_t index, const BOOL value);

#define IMPL_DO(__CLASS, __a, __b) \
int32_t __CLASS::Set ##__a ( int32_t index, const BOOL value) \
{	return SetDigitalOutput(index, __b, value); }
//------------------------------------------------------------------
#define DECLARE_AI(__a) \
int32_t Get ##__a ( int32_t index, float32_t& value);

#define IMPL_AI(__CLASS, __a, __b) \
int32_t __CLASS::Get ##__a ( int32_t index, float32_t& value) \
{	return GetAnalogInput(index, __b, value); }
//------------------------------------------------------------------
#define DECLARE_AO(__a) \
int32_t Set ##__a ( int32_t index, const float32_t value);

#define IMPL_AO(__CLASS, __a, __b) \
int32_t __CLASS::Set ##__a ( int32_t index, const float32_t value) \
{	return SetAnalogOutput(index, __b, value); }
