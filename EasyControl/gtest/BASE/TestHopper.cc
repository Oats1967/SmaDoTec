#include "pch.h"
#include "BASE/include/2DTrapeze.h"


using namespace base;


#define CURVESIZE 5U
namespace
{
TEST(Trapeze, Filling) 
{
    uint32_t count = 0;
    C2DTrapeze trapez{ { base::IPoint{ 6, 6 }, base::IPoint{ 12, 6 }, base::IPoint{ 11, 3 }, base::IPoint{ 7, 3 }  } };

    C2DTrapeze::TrapezePoints p;
    trapez.GetFilling(0.1F, p, count);
    EXPECT_EQ(4, count);
    trapez.GetFilling(0.5F, p, count);
    EXPECT_EQ(4, count);
    trapez.GetFilling(1.1F, p, count);
    EXPECT_EQ(4, count);
}
};