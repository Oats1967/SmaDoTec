//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EncoderState
///
/// @file   EncoderState.h
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

namespace base
{
enum class eEncoderState : uint32_t
{
    ENCODERSTATE_OFF = 0, 
    ENCODERSTATE_STOP, 
    ENCODERSTATE_RUN, 
    ENCODERSTATE_UNDEFINED, 
    ENCODERSTATE_MAX
};

};


