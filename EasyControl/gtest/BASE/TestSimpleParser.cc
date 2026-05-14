#include "pch.h"
#include <cassert>
#include <map>
#include "BASE/BoolParser/SimpleParser.h"


using namespace  simpleparser;



namespace
{
    TEST(SimpleParser, And)
    {
        std::map<const std::string, BOOL> refMap{ { "A", 1}, { "B", 0} };

        BOOL boolresult = FALSE;
        int32_t result = evaluate("A AND B", refMap, boolresult);
        EXPECT_FALSE(boolresult);
        EXPECT_EQ(result, 0);

        result = evaluate("A OR B", refMap, boolresult);
        EXPECT_TRUE(boolresult);
        EXPECT_EQ(result, 0);

        result = evaluate("A AND ( A OR B)", refMap, boolresult);
        EXPECT_TRUE(boolresult);
        EXPECT_EQ(result, 0);

        result = evaluate("A OR( A AND B)", refMap, boolresult);
        EXPECT_TRUE(boolresult);
        EXPECT_EQ(result, 0);

        result = evaluate("A OR( A OR ( A OR B) AND B)", refMap, boolresult);
        EXPECT_TRUE(boolresult);
        EXPECT_EQ(result, 0);

        result = evaluate("B AND( A OR ( A OR B) AND B)", refMap, boolresult);
        EXPECT_FALSE(boolresult);
        EXPECT_EQ(result, 0);

    }
}
