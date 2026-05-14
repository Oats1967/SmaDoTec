#include "pch.h"
#include "BASE/Utils/public/MassflowMemory.h"


using namespace base;


namespace
{
    static void SetMassflowItem(int32_t k, base::CMassflowItem<float32_t>& rItem)
    {
        for (uint32_t i = 0; i < cViewSelectMax; i++)
        {
            const float32_t value = _F32(k + 1) * (i + 1);
            rItem.Set(static_cast<eMassflowSelect>(i), value);
        }
    }

    TEST(TestMassflow, MassflowItem)
    {
        base::CMassflowItem<float32_t> aItem;

        for (uint32_t i = 0; i < cViewSelectMax; i++)
        {
            const float32_t value = 1.0f * (i + 1);
            aItem.Set(static_cast<eMassflowSelect>(i), value);
            auto ret = aItem.Get(static_cast<eMassflowSelect>(i));
            EXPECT_EQ(value, ret);
        }
    }
    TEST(TestMassflow, MassflowItemList)
    {
        const uint32_t N = GLOBALDOSEMAXCOUNT;
        base::CMassflowItemList<float32_t,N> aListItem;


        for (uint32_t k = 0; k < N; k++)
        {
            base::CMassflowItem<float32_t> aItem;
            SetMassflowItem(k, aItem);
            aListItem.Set(k, aItem);
        }

        for (uint32_t k = 0; k < N; k++)
        {
            const auto& aItem = aListItem.Get(k);
            for (uint32_t i = 0; i < cViewSelectMax; i++)
            {
                const float32_t value = _F32(k + 1) * (i + 1);
                auto ret = aItem.Get(static_cast<eMassflowSelect>(i));
                EXPECT_EQ(value, ret);
            }
        }
    }

    TEST(TestMassflow, MassflowEntry)
    {
        const uint32_t N = 8U;

         base::CMassflowEntry aEntry(N);
         for (uint32_t k = 0; k < N; k++)
         {
             base::CMassflowItem<float32_t> aItem;
             SetMassflowItem(k, aItem);
             aEntry.Set(k, aItem);
         }
         EXPECT_EQ(aEntry.GetMaxItems(), N);
         for (uint32_t i = 0; i < cViewSelectMax; i++)
         {
             auto iSelect = static_cast<eMassflowSelect>(i);
             float32_t retmax = _F32(N) * (i + 1);
             auto maxvalue = aEntry.GetMaxValue(iSelect, -1);
             EXPECT_EQ(maxvalue, retmax);
             float32_t retmin = _F32(0 + 1) * (i + 1);
             auto minvalue = aEntry.GetMinValue(iSelect, -1);
             EXPECT_EQ(minvalue, retmin);
         }
    }

    TEST(TestMassflow, MassflowEntryMemory)
    {
        base::utils::CMassflowMemory aMemory;
        const uint32_t N = base::utils::RECORDINGTIME;
        aMemory.Init();
        for (uint32_t k = 0; k < N; k++)
        {
            const uint32_t maxItems = 1U;
            base::CMassflowEntry aEntry(maxItems);
            for (uint32_t t = 0; t < maxItems; t++)
            {
                base::CMassflowItem<float32_t> aItem;
                SetMassflowItem(t*k, aItem);
                aEntry.Set(t, aItem);
            }
            aMemory.Add(aEntry);
        }
        EXPECT_EQ(aMemory.GetCounter(), N);
    }
};