//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ProfibusXml
///
/// @file   ProfibusXml.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <string>
#include "BASE/types.h"
#include "BASE/include/ProfibusTypes.h"

class CProfibusXml
{
public:
    base::eProfibusProtocol m_protocol;

public:
    CProfibusXml() : m_protocol(base::eProfibusProtocol::ePB_PROTOCOL_NONE)
    {}
    ~CProfibusXml() = default;

    BOOL Open(const std::string& _szPath2Device);
};

