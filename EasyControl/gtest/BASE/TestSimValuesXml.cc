#include "pch.h"
#include <cassert>
#include "BASE/Utils/public/xml/SimValuesXml.h"

using namespace base;



namespace
{
    TEST(TestSimValuesXml, XmlFile)
    {
        base::xml::CSimValuesXml xml;

        auto result = xml.Load("C://EasyControl//config//SimValuesConfig.xml");
        EXPECT_TRUE(result);
        result = xml.Save("C://EasyControl//config//SimValuesConfig2.xml");
        EXPECT_TRUE(result);
    }
}
