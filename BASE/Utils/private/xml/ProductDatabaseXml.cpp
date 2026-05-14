//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ProductDatabaseXml
///
/// @file   ProductDatabaseXml.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <array>
#include <cstring>
#include <algorithm>
#include "BASE/types.h"
#include "BASE/Utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/ProductDatabaseXml.h"
#include "Logger/include/Logger.h"

STATIC_KEY(CONFIGURATION, "ProductDatabase")
STATIC_KEY(PRODUCTLIST, "ProductList")
STATIC_KEY(PRODUCT, "Product")
STATIC_KEY(NAME, "Name")
STATIC_KEY(KEY, "Key")
STATIC_KEY(HEADERVERSION, "Version")


const static int32_t c_HeaderVersion = 1;
static int32_t g_HeaderVersion = 0;

using namespace tinyxml2;

using StringMap = std::map<uint64_t, std::string>;


namespace base
{
namespace xml
{

//*********************************************************************************************************************
//*********************************************************************************************************************
static void SlpitAttributes(const xmlentry& attriblist, StringMap& rDB)
{
    auto key = [&attriblist](const std::string& str) {
        return std::find_if(attriblist.cbegin(), attriblist.cend(), [&str](const auto& item)-> BOOL { return item.first == str; });
    };

    auto it = key(NAME);
    if (it != attriblist.cend())
    {
        std::string szName{ it->second };
        it = key(KEY);
        if (it != attriblist.cend())
        {
            const auto uKey64{ base::utils::ustoull(it->second) };
            auto rit = rDB.find(uKey64);
            if (rit != rDB.end())
            {
                LOGERROR("Double key !!!, element already in product list" << szName);
            }
            else
            {
                rDB.insert({ uKey64, szName });
            }
        }
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
static void ReadItem(const XMLElement& el, StringMap& rDB)
{
    auto attriblist = helper::ReadAttribList(el);
    assert(!attriblist.empty());
    if (attriblist.empty())
    {
        assert(FALSE);
        LOGERROR("attribList empty");
        throw std::logic_error("attriblist empty!");
    }
    SlpitAttributes(attriblist, rDB);
}
 //*********************************************************************************************************************
//*********************************************************************************************************************
 static void SetEntry(XMLDocument& doc, XMLNode& config, const std::pair<uint64_t, std::string>& rItem)
 {
     auto el = doc.NewElement(PRODUCT);
     el->SetAttribute(NAME, rItem.second.c_str());
     el->SetAttribute(KEY, rItem.first);
     config.InsertEndChild(el);
}
 
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CProductDatabaseXml::Load(const std::string& szfilename)
{
    XMLDocument doc;

    struct stat buffer;
    BOOL fileexists = stat(szfilename.c_str(), &buffer) == 0;
    if (!fileexists)
    {
        LOGERROR("xml-file does not exist ... overwriting " << szfilename);
        Save(szfilename);
    }

    auto loadOkay = doc.LoadFile(szfilename.c_str());
    if (loadOkay != XML_SUCCESS)
    {
        LOGERROR("Error loading xml-file: " << szfilename << "; " << doc.ErrorStr());
        doc.Clear();
        return FALSE;
    }
    auto config = doc.FirstChildElement(CONFIGURATION);
    if (!config)
    {
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
    auto productlist = config->FirstChildElement(PRODUCTLIST);
    if (!productlist)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << PRODUCTLIST);
        doc.Clear();
        return FALSE;
    }
    StringMap rDB;
    for (auto product = productlist->FirstChildElement(PRODUCT);  product != nullptr; product = product->NextSiblingElement())
    {
        try
        {
            ReadItem(*product, rDB);
        }
        catch (...)
        {
            LOGERROR("Error reading element!");
        }
    }
    Set(CProductDatabaseMap{ rDB });
    doc.Clear();
    return TRUE;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CProductDatabaseXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    if (!config)
    {
        assert(FALSE);
        LOGERROR("Cannot create config " << CONFIGURATION);
        return FALSE;
    }
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    if (!header)
    {
        assert(FALSE);
        LOGERROR("Cannot create header " << HEADERVERSION);
        return FALSE;
    }
    header->ToElement()->SetText(c_HeaderVersion);
    auto productlist = config->InsertEndChild(doc.NewElement(PRODUCTLIST));
    if ( ! productlist)
    {
        assert(FALSE);
        LOGERROR("Cannot create productlist " << PRODUCTLIST);
        return FALSE;
    }
    auto& field = Get().getMap();
    for (const auto& item : field)
    {
        SetEntry(doc, *productlist, item);
    }
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




