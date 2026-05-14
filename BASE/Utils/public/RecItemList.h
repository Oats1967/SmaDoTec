//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecItem.h
///
/// @file   RecItem.h
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

#include <ctime>
#include <vector>
#include "BASE/types.h"
#include "BASE/Utils/public/RecItem.h"
#include "BASE/Utils/public/GenericList.h"

namespace base
{
namespace utils
{
class CRecItemList : public CGenericList< CRecItem >
{
private:
    uint32_t		m_MaxItems;		// RadID
    std::string     m_filename;

public:
    CRecItemList(int32_t lID = -1L) : CGenericList(lID)
        , m_MaxItems{ 0 }
        , m_filename ("")
    {}

    ~CRecItemList(void) = default;

    CRecItemList(CRecItemList&& rList) noexcept : CGenericList{ std::move(rList) }
        , m_MaxItems {  std::move(rList.m_MaxItems)   }
        , m_filename { std::move(rList.m_filename) }
    {}

    CRecItemList(const CRecItemList& rList) : CGenericList{ rList }
        , m_MaxItems { rList.m_MaxItems }
        , m_filename{ rList.m_filename }
    {}

    CRecItemList& operator = (const CRecItemList& rList)
    {
        if (this != &rList)
        {
            CGenericList::operator=(rList);
            m_MaxItems = rList.m_MaxItems;
            m_filename = rList.m_filename;
        }
        return *this;
    }

    CRecItemList& operator += (const CRecItemList& aP)
    {
        CGenericList::operator+=(aP);
        m_MaxItems = __max(m_MaxItems, aP.m_MaxItems);
        return *this;
    }


    SETGET(uint32_t, MaxItems)

    const std::string GetFilename(void) const override
    {  return m_filename;   }

    void SetFilename(const std::string& filename)
    {   m_filename = filename;    }
    void SetFilename(std::time_t);

    void CreateFilename(void);


    uint32_t ScanMaxItems() const;

    BOOL	    SaveAll(void) override;
    BOOL	    LoadAll(void) override;
    BOOL        SaveLastItem() override;

    float32_t GetMaxValue(const eMassflowSelect iView, const int32_t id) const;
    float32_t GetMinValue(const eMassflowSelect iView, const int32_t id) const;

    int32_t FindRecItemTime(const std::time_t& t) const;
    void Extract(const std::string& ANNumber);
    void Extract(const time_t& tStart, const time_t& tEnd);


    void GetANNumbers(std::vector<std::string>& ANNumber) const;
 };


}
};


