//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MassflowMemory.h
///
/// @file   MassflowMemory.h
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
#include "BASE/include/MassflowSelect.h"
#include "BASE/Utils/public/RingBuffer.h"

namespace base
{
namespace utils
{

static constexpr uint32_t RECORDINGTIME = 600;
class CMassflowMemory : public CRingBuffer< CMassflowEntry, RECORDINGTIME>
{
public:
    CMassflowMemory(void) : CRingBuffer()
    {}

    ~CMassflowMemory(void) override = default;
    CMassflowMemory(const CMassflowMemory& dest) : CRingBuffer(dest)
    {}
    CMassflowMemory& operator=(const CMassflowMemory& dest)
    {
        CRingBuffer::operator = (dest);
        return *this;
    }

    float32_t GetMaxValue(const eMassflowSelect nView, const int32_t n) const;
    float32_t GetMinValue(const eMassflowSelect nView, const int32_t n) const;

    //------------------------------------------------------------------------------------
    ///  @brief   Save
    ///
    ///           This method saves the applied table
    ///
    ///  @param[in] sz , LPCSTR
    ///  @return BOOL
    ///
    //------------------------------------------------------------------------------------
    BOOL Save(const std::string& filename);
};


};
};



    

