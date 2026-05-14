//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LayoutConfigXml
///
/// @file   LayoutConfigXml.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cstring>
#include <algorithm>
#include <sstream>
#include "BASE/types.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/LayoutConfigXml.h"
#include "Logger/include/Logger.h"

STATIC_KEY( CONFIGURATION, "LayoutConfig")
STATIC_KEY( HEADERVERSION, "Version")
STATIC_KEY( RECIPE, "Recipe")
STATIC_KEY( CUSTOMERLOGO, "CustomerLogo")
STATIC_KEY( LOGO, "Logo")
STATIC_KEY( PNG, "PNG")
STATIC_KEY( LOCATION, "Location")
STATIC_KEY( SERVICEBOX, "ServiceBox")
STATIC_KEY( LCAUTHORIZE, "LCAuthorize")
STATIC_KEY( PRODUCTNAME, "ProductName")
STATIC_KEY( REFILLBUTTON, "RefillButton")
STATIC_KEY( MESSAGEBOX, "MessageBox")
STATIC_KEY( TOTALIZERBOX, "TotalizerBox")
STATIC_KEY( BARCHART, "BarChart")
STATIC_KEY( LINEBOX, "LineBox")


const static int32_t c_HeaderVersion = 1;
static int32_t g_HeaderVersion = 0;


using namespace tinyxml2;

namespace base
{
namespace xml
{
//*********************************************************************************************************************
//*********************************************************************************************************************
static BOOL LoadLocation(const tinyxml2::XMLElement& node, IPoint& aP)
{
    auto pointcfg = node.FirstChildElement(LOCATION);
    auto result = BOOL(pointcfg != nullptr);
    if (result)
    {
        result = helper::GetNumber(*pointcfg, "X", aP.x);
        if (result)
        {
            result = helper::GetNumber(*pointcfg, "Y", aP.y);
            if ( ! result)
            {
                assert(FALSE);
                LOGERROR("Not found Y - location");
            }
        }
        else
        {
            assert(FALSE);
            LOGERROR("Not found X - location");
        }
    }
    else
    {
        assert(FALSE);
        LOGERROR("LOCATION not found");
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
static BOOL LoadLocation(const tinyxml2::XMLElement& node, IRect& aR)
{
    IPoint aP;
    int32_t ix = 0;
    int32_t iy = 0;
    auto pointcfg = node.FirstChildElement(LOCATION);
    auto result = BOOL(pointcfg != nullptr);
    if (result)
    {
        result = helper::GetNumber(*pointcfg, "X", aP.x);
        if (result)
        {
            result = helper::GetNumber(*pointcfg, "Y", aP.y);
            if (result)
            {
                result = helper::GetNumber(*pointcfg, "WIDTH", ix);
                if (result)
                {
                    result = helper::GetNumber(*pointcfg, "HEIGHT", iy);
                    if (result)
                    {
                        aR.topleft.x = aP.x;
                        aR.topleft.y = aP.y;
                        aR.bottomright.x = aP.x + ix;;
                        aR.bottomright.y = aP.y + iy;;
                    }
                    else
                    {
                        assert(FALSE);
                        LOGERROR("Not found HEIGHT");
                    }
                }
                else
                {
                    assert(FALSE);
                    LOGERROR("Not found WIDTH");
                }
            }
            else
            {
                assert(FALSE);
                LOGERROR("Not found Y - location");
            }
        }
        else
        {
            assert(FALSE);
            LOGERROR("Not found X - location");
        }
    }
    else
    {
        assert(FALSE);
        LOGERROR("LOCATION not found");
    }
    return result;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLayoutConfigXml::Load(const std::string& szfilename)
{
    struct stat buffer;
    BOOL fileexists = stat(szfilename.c_str(), &buffer) == 0;
    if (!fileexists)
    {
        LOGERROR("xml-file does not exist ... overwriting " << szfilename);
        Save(szfilename);
    }

    XMLDocument doc;
    auto loadOkay = doc.LoadFile(szfilename.c_str());
    if (loadOkay != XML_SUCCESS)
    {
        assert(FALSE);
        LOGERROR("Error loading xml-file: " << szfilename << "; " << doc.ErrorStr());
        doc.Clear();
        return FALSE;
    }
    auto config = doc.FirstChildElement(CONFIGURATION);
    if (!config)
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " " << CONFIGURATION);
        doc.Clear();
        return FALSE;
    }
    auto result = base::xml::helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }
    auto& field = Get();
    {
        std::string sTemp;
        result = helper::GetString(*config, RECIPE, sTemp);
        if (result)
        {
            field.SetLayoutRecipe(c_YesNoMap.get(sTemp));
        }
        else
        {
            assert(FALSE);
            LOGERROR("Error in Xml-file: " << szfilename << " " << RECIPE);
        }
    }

    {
        std::string sTemp;
        result = helper::GetString(*config, LCAUTHORIZE, sTemp);
        if (result)
        {
            field.SetLCAuthorize(c_YesNoMap.get(sTemp));
        }
        else
        {
            assert(FALSE);
            LOGERROR("Error in Xml-file: " << szfilename << " " << LCAUTHORIZE);
        }
    }

    {
        std::string sTemp;
        result = helper::GetString(*config, PRODUCTNAME, sTemp);
        if (result)
        {
            field.SetProductNameEnable(c_YesNoMap.get(sTemp));
        }
        else
        {
            LOGERROR("Error in Xml-file: " << szfilename << " " << PRODUCTNAME);
            g_HeaderVersion = -1;
        }
    }
    {
        std::string sTemp;
        result = helper::GetString(*config, REFILLBUTTON, sTemp);
        if (result)
        {
            field.SetRefillButtonEnable(c_YesNoMap.get(sTemp));
        }
        else
        {
            field.SetRefillButtonEnable(TRUE);
            LOGERROR("Error in Xml-file: " << szfilename << " " << REFILLBUTTON);
            g_HeaderVersion = -1;
        }
    }

    auto node = config->FirstChildElement(CUSTOMERLOGO);
    if (node)
    {
        std::string sTemp;
        base::CCustomerLogoConfig customerLogo;
        result = helper::GetString(*node, PNG, sTemp);
        if (result)
        {
            customerLogo.m_CustomerLogoPNG = sTemp;
        }
        else
        {
            assert(FALSE);
            LOGERROR("Not found customer logo PNG-file : " << PNG);
        }
        if (result)
        {
            customerLogo.m_bInit = TRUE;
            result = LoadLocation(*node, customerLogo.m_Location);
            if ( ! result)
            {
                assert(FALSE);
                IPoint aDefault{ 20, 300 };
                customerLogo.m_Location = aDefault;
                LOGERROR("Error customer logo location");
                g_HeaderVersion = -1;
            }
            field.SetCustomerLogo(customerLogo);
        }
    }
    {
        IPoint aP;
        node = config->FirstChildElement(LOGO);
        result = (node != nullptr);
        if ( result )
        {
            result = LoadLocation(*node, aP);
            if  ( ! result)
            {
                assert(FALSE);
                LOGERROR("Error logo location");
            }
        }
        if ( ! result)
        {
            aP = IPoint{ 20, field.GetCustomerLogo().m_Location.y };
            g_HeaderVersion = -1;
        }
        field.SetPointLogo(aP);
    }
    {
        IPoint aP;
        node = config->FirstChildElement(MESSAGEBOX);
        result = (node != nullptr);
        if (result)
        {
            result = LoadLocation(*node, aP);
            if ( ! result)
            {
                assert(FALSE);
                LOGERROR("Error MessageBox location");
            }
        }
        if ( ! result)
        {
            aP = IPoint{ 20, field.GetPointLogo().y };
            g_HeaderVersion = -1;
        }
        field.SetPointMessageBox(aP);
    }
    {
        IPoint aP;
        node = config->FirstChildElement(LINEBOX);
        result = (node != nullptr);
        if (result)
        {
            result = LoadLocation(*node, aP);
            if (!result)
            {
                assert(FALSE);
                LOGERROR("Error LineBox location");
            }
        }
        if (!result)
        {
            aP = IPoint{ 200, 200};
            g_HeaderVersion = -1;
        }
        field.SetPointLineBox(aP);
    }
    {
        IPoint aP;
        node = config->FirstChildElement(SERVICEBOX);
        result = (node != nullptr);
        if (result)
        {
            result = LoadLocation(*node, aP);
            if (!result)
            {
                assert(FALSE);
                LOGERROR("Error ServiceBox location");
            }
        }
        if (!result)
        {
            aP = IPoint{ field.GetPointLogo().x + 200, field.GetPointLogo().y };
            g_HeaderVersion = -1;
        }
        field.SetPointServiceBox(aP);
    }
    {
        IPoint aP;
        node = config->FirstChildElement(TOTALIZERBOX);
        result = (node != nullptr);
        if (result)
        {
            result = LoadLocation(*node, aP);
            if (!result)
            {
                assert(FALSE);
                LOGERROR("Error TotalizerBox location");
            }
        }
        if (!result)
        {
            // aP = IPoint{ field.GetPointLogo().x + 300, field.GetPointLogo().y - 50 };
            g_HeaderVersion = -1;
        }
        field.SetPointTotalizerBox(aP);
    }

    {
        IRect aR;
        node = config->FirstChildElement(BARCHART);
        result = (node != nullptr);
        if (result)
        {
            result = LoadLocation(*node, aR);
            if (!result)
            {
                assert(FALSE);
                LOGERROR("Error BarChart location");
            }
        }
        if (! result)
        {
            g_HeaderVersion = -1;
        }
        field.SetRectBarChart(aR);
    }

    if ( g_HeaderVersion != c_HeaderVersion)
    {
        Save(szfilename);
    }
    doc.Clear();
    return TRUE;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLayoutConfigXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());

    const auto& field = Get();

    std::stringstream str;
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);
    auto keyel = config->InsertEndChild(doc.NewElement(RECIPE));
    keyel->ToElement()->SetText(c_YesNoMap.get(field.GetLayoutRecipe()).c_str());
    keyel = config->InsertEndChild(doc.NewElement(LCAUTHORIZE));
    keyel->ToElement()->SetText(c_YesNoMap.get(field.GetLCAuthorize()).c_str());
    keyel = config->InsertEndChild(doc.NewElement(PRODUCTNAME));
    keyel->ToElement()->SetText(c_YesNoMap.get(field.GetProductNameEnable()).c_str());
    keyel = config->InsertEndChild(doc.NewElement(REFILLBUTTON));
    keyel->ToElement()->SetText(c_YesNoMap.get(field.GetRefillButtonEnable()).c_str());

    // CustomerLogo
    const auto& customerLogo = field.GetCustomerLogo();

    auto location = [&doc](tinyxml2::XMLNode& keyNode, const base::IPoint& aP) {
        auto keyel = keyNode.InsertEndChild(doc.NewElement(LOCATION));
        assert(keyel);
        auto pointcfg = keyel->InsertFirstChild(doc.NewElement("X"));
        assert(pointcfg);
        pointcfg->ToElement()->SetText(aP.x);
        pointcfg = keyel->InsertEndChild(doc.NewElement("Y"));
        assert(pointcfg);
        pointcfg->ToElement()->SetText(aP.y);
        return TRUE;
    };
    auto rectangle = [&doc](tinyxml2::XMLNode& keyNode, const base::IRect& aR) {
        auto keyel = keyNode.InsertEndChild(doc.NewElement(LOCATION));
        assert(keyel);
        auto pointcfg = keyel->InsertFirstChild(doc.NewElement("X"));
        assert(pointcfg);
        pointcfg->ToElement()->SetText(aR.topleft.x);
        pointcfg = keyel->InsertEndChild(doc.NewElement("Y"));
        assert(pointcfg);
        pointcfg->ToElement()->SetText(aR.topleft.y);
        pointcfg = keyel->InsertEndChild(doc.NewElement("WIDTH"));
        assert(pointcfg);
        pointcfg->ToElement()->SetText(aR.width());
        pointcfg = keyel->InsertEndChild(doc.NewElement("HEIGHT"));
        assert(pointcfg);
        pointcfg->ToElement()->SetText(aR.height());
        return TRUE;
    };

    if (customerLogo.m_bInit)
    {
        auto keyNode = config->InsertEndChild(doc.NewElement(CUSTOMERLOGO));
        assert(keyNode);
        keyel = keyNode->InsertFirstChild(doc.NewElement(PNG));
        keyel->ToElement()->SetText(customerLogo.m_CustomerLogoPNG.c_str());
        location(*keyNode, customerLogo.m_Location);
    }
    {
        auto keyNode = config->InsertEndChild(doc.NewElement(LOGO));
        assert(keyNode);
        location(*keyNode, field.GetPointLogo());
    }
    {
        auto keyNode = config->InsertEndChild(doc.NewElement(MESSAGEBOX));
        assert(keyNode);
        location(*keyNode, field.GetPointMessageBox());
    }
    {
        auto keyNode = config->InsertEndChild(doc.NewElement(LINEBOX));
        assert(keyNode);
        location(*keyNode, field.GetPointLineBox());
    }
    {
        auto keyNode = config->InsertEndChild(doc.NewElement(SERVICEBOX));
        assert(keyNode);
        location(*keyNode, field.GetPointServiceBox());
    }
    {
        auto keyNode = config->InsertEndChild(doc.NewElement(TOTALIZERBOX));
        assert(keyNode);
        location(*keyNode, field.GetPointTotalizerBox());
    }
    {
        auto keyNode = config->InsertEndChild(doc.NewElement(BARCHART));
        assert(keyNode);
        rectangle(*keyNode, field.GetRectBarChart());
    }

    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};


};
};




