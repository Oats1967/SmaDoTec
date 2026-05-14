//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module MassflowMemory.cpp
///
/// @file   MassflowMemory.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include "BASE/Base.h"
#include "BASE/utils/public/MassflowMemory.h"

using std::cout; using std::ofstream;
using std::endl; using std::string;
using std::fstream;

namespace base
{
namespace utils
{

 constexpr char_t DELIMITER = ';';
 constexpr char_t EOL = '\n';
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CMassflowMemory::Save(const std::string& filename)
{
    BOOL result = FALSE;
    ofstream file_out;

    file_out.open(filename);
    if (!file_out.is_open())
    {
        LOGERROR("File : " << filename << "cannot be opened !");
    }
    else
    {
        const uint32_t count = GetCounter();
        for ( uint32_t i = 0; i < count; i++)
        {
            const auto& item = GetElement(i);
            std::ostringstream stringStream;

            stringStream << (i + 1) << DELIMITER;
            for (uint32_t j = 0; j < item.GetMaxItems(); j++)
            {
                for (uint32_t k = 0; k < cMassflowSelectMax; k++)
                {
                    auto& value = item.Get(static_cast<eMassflowSelect>(k), j);
                    stringStream << ROUND3(value) << DELIMITER;
                }
            }
            stringStream << EOL;
            file_out << stringStream.str();
        }
        file_out.close();
        result = TRUE;
    }
    return result;
}

//---------------------------------------------------------------------------------------------------
// Berechnet den Maximalwert
//---------------------------------------------------------------------------------------------------
float32_t CMassflowMemory::GetMaxValue(const eMassflowSelect nView, const int32_t index) const
{
    assert(index >= 0 && index < GLOBALDOSEMAXCOUNT);

    float32_t fMaxWert = -FLT_MAX / 2;
    const uint32_t count = GetCounter();
    for (uint32_t j = 0; j < count; j++)
    {
        const auto& item = GetElement(j);
        const auto value = item.GetMaxValue(nView, index);
        if (value > fMaxWert)
        {
            fMaxWert = value;
        }
    }
    return fMaxWert;
}


//---------------------------------------------------------------------------------------------------
// Berechnet den Minimalwert
//---------------------------------------------------------------------------------------------------
float32_t CMassflowMemory::GetMinValue(const eMassflowSelect nView, const int32_t index) const
{
    assert(index >= 0 && index < GLOBALDOSEMAXCOUNT);

    float32_t fMinWert = FLT_MAX;
    const uint32_t count = GetCounter();
    for (uint32_t j = 0; j < count; j++)
    {
        const auto& item = GetElement(j);
        const auto value = item.GetMinValue(nView, index);
        if (value < fMinWert)
        {
            fMinWert = value;
        }
    }
    return fMinWert;
}


 };
}



