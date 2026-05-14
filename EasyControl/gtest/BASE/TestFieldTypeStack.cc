#include "pch.h"
#include <cassert>
#include "BASE/include/FieldTypeStack.h"


using namespace base;

namespace
{
    TEST(FieldTypeStack, Stack)
    {
        uint8_t buffer[1000];

        {
            CFieldTypeStack aStack(buffer, 1000, RemoteFieldType::FLOAT32, FALSE);

            uint8_t k = 100;
            aStack.push(k);
            k = 255;
            aStack.push(k);
        }
        {
            CFieldTypeStack aStack(buffer, 1000, RemoteFieldType::FLOAT32, FALSE);

            auto v = aStack.pop<uint8_t>();
            EXPECT_EQ(v, 100);

            v = aStack.pop<uint8_t>();
            EXPECT_EQ(v, 255);
        }

        {
            CFieldTypeStack aStack(buffer, 1000, RemoteFieldType::FLOAT32, TRUE);

            uint8_t k = 100;
            aStack.push(k);
            k = 255;
            aStack.push(k);
        }
        {
            CFieldTypeStack aStack(buffer, 1000, RemoteFieldType::FLOAT32, TRUE);

            auto v = aStack.pop<uint8_t>();
            EXPECT_EQ(v, 100);

            v = aStack.pop<uint8_t>();
            EXPECT_EQ(v, 255);
        }

        {
            CFieldTypeStack aStack(buffer, 1000, RemoteFieldType::UINT8, FALSE);

            uint16_t k = 256;
            aStack.push(k);
        }
        {
            CFieldTypeStack aStack(buffer, 1000, RemoteFieldType::UINT8, FALSE);

            auto v = aStack.pop<uint8_t>();
            EXPECT_EQ(v, 255);
        }

    }
}
