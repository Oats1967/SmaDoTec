//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MassflowSelect
///
/// @file   MassflowSelect.h
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

#include <array>
#include <cassert>
#include "BASE/Base.h"

namespace base
{
enum class eMassflowSelect : uint16_t 
{
    eVIEWMASSFLOW = 0,
    eVIEWSETPOINT,
    eVIEWDRIVECOMMAND,
    eVIEWWEIGHT,
    eVIEWDOSEPERFORMANCE,
    eVIEWTOTALIZER,
    eVIEWROTSPEED, 
    eVIEWMAX,
};
constexpr uint32_t cMassflowSelectMax = _S32(eMassflowSelect::eVIEWMAX);

union MassflowBitfield
{
    struct
    {
        uint32_t bitVIEWMASSFLOW        : 1;         // Massflow
        uint32_t bitVIEWSETPOINT        : 1;         // setpoint
        uint32_t bitVIEWDRIVECOMMAND    : 1;     // drivecommand
        uint32_t bitVIEWWEIGHT          : 1;           // weight
        uint32_t bitVIEWDOSEPERFORMANCE : 1;  // Doseperformance
        uint32_t bitVIEWTOTALIZER       : 1;        // totalizer
        uint32_t bitVIEWROTSPEED        : 1;         // rotational speed
    } flags;
    uint32_t allflags = 0;
};

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
template<typename T>
class CMassflowItem 
{
protected:
    std::array< T, cMassflowSelectMax> m_attribute;

public:
    CMassflowItem() : m_attribute{}
    {  m_attribute.fill(static_cast<T>(0));   }

    const T& Get(const eMassflowSelect select) const
    {  
        assert(_S32(select) < cMassflowSelectMax);
        return m_attribute[_S32(select)];    
    }

    void Set(const eMassflowSelect select, const T value)
    {   
        assert(_S32(select) < cMassflowSelectMax);
        m_attribute[_S32(select)] = value;   
    }
};

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
template< typename T, uint32_t N>
class CMassflowItemList
{
protected:
    std::array< CMassflowItem<T>, N> m_Attribute;

public:
    const T& Get(const eMassflowSelect select, const uint32_t index) const
    {  
        assert(index < N);
        return m_Attribute[index].Get(select);    
    }

    void Set(const eMassflowSelect select, const uint32_t index, const T value)
    {   
        assert(index < N);
        m_Attribute[index].Set(select,value);    
    }

    const CMassflowItem<T>& Get(const uint32_t index) const
    {  
        assert(index < N);
        return m_Attribute[index];   
    }

    void Set(const uint32_t index, const CMassflowItem<T>& value)
    {  
        assert(index < N);
        m_Attribute[index] = value;   
    }

    CMassflowItemList()
    {  
        m_Attribute.fill(CMassflowItem<T>());   
    }
};
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
class CMassflowEntry : public CMassflowItemList<float32_t, GLOBALDOSEMAXCOUNT>
{
protected:
    uint32_t m_MaxItems;

public:
    CMassflowEntry(uint32_t maxItems = 0) :m_MaxItems(maxItems)
    {}

    SETGET(uint32_t, MaxItems);

    float32_t GetMaxValue(const eMassflowSelect nView, const int32_t n) const;
    float32_t GetMinValue(const eMassflowSelect nView, const int32_t n) const;
};

//************************************************************************************************************
//************************************************************************************************************
inline float32_t CMassflowEntry::GetMaxValue(const eMassflowSelect nView, const int32_t index) const
{
    float32_t valuemax = 0.0F;

    if (index < 0)
    {
        if ( m_MaxItems > 0 )
        {
            valuemax = -FLT_MAX / 2;
            for (uint32_t ix = 0; ix < m_MaxItems; ix++)
            {
                const auto& value = Get(nView, ix);
                valuemax = __max(valuemax, value);
            }
        }
    }
    else
    {
        valuemax = Get(nView, index);
    }
    return valuemax;
}
//************************************************************************************************************
//************************************************************************************************************
inline float32_t CMassflowEntry::GetMinValue(const eMassflowSelect nView, const int32_t index) const
{
    float32_t valuemin = 0.0F;
    if (index < 0)
    {
        if (m_MaxItems > 0)
        {
            valuemin = FLT_MAX;
            for (uint32_t ix = 0; ix < m_MaxItems; ix++)
            {
                const auto& value = Get(nView, ix);
                valuemin = __min(valuemin, value);
            }
        }
    }
    else
    {
        valuemin = Get(nView, index);
    }
    return valuemin;
}




#define DECLARE_MASSFLOWSELECT(field) const std::array<base::eMassflowSelect, base::cMassflowSelectMax> field = { \
    base::eMassflowSelect::eVIEWMASSFLOW, \
	base::eMassflowSelect::eVIEWSETPOINT,\
	base::eMassflowSelect::eVIEWDRIVECOMMAND,\
	base::eMassflowSelect::eVIEWWEIGHT,\
	base::eMassflowSelect::eVIEWDOSEPERFORMANCE,\
	base::eMassflowSelect::eVIEWTOTALIZER,\
	base::eMassflowSelect::eVIEWROTSPEED \
}

};



    

