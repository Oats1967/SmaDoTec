//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module GenericRemoteXml
///
/// @file   GenericRemoteXml.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <algorithm>
#include "BASE/types.h"
#include "BASE/Utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/ProfiBusDeviceXml.h"
#include "BASE/Utils/public/StringUtil.h"
#include "Logger/include/Logger.h"



#define CONFIGURATION "Device"
#define TYPE "Type"
#define TABLE "Table"
#define ITEM "Item"
#define CATEGORY "CATEGORY"
#define FIELDTYPE "FIELDTYPE"
#define DIRECTION "DIRECTION"
#define ELEMENTS "ELEMENTS"
#define WRITE "WRITE"
#define READ "READ"
#define PATH "PATH"
#define TASKID "TaskID"
#define PROFINET "PROFINET"
#define PROFIBUS "PROFIBUS"

using namespace tinyxml2;

namespace base
{
namespace xml
{

//*********************************************************************************************************************
//*********************************************************************************************************************
ProfiBusDescriptor CProfiBusDeviceXml::SlpitAttributes( const xmlentry& attriblist ) const
{
    uint32_t success = 0;

    ProfiBusDescriptor tableentry;

    auto key = [&attriblist](const std::string& str) {
        return std::find_if(attriblist.cbegin(), attriblist.cend(), [&str](const auto& item)-> BOOL { return item.first == str; });
    };

    auto it = key(CATEGORY);
    if (it != attriblist.cend())
    {
        tableentry.m_Category = c_CategoryMap.get(it->second);
        success++;
    }
    it = key(FIELDTYPE);
    if (it != attriblist.cend())
    {
        tableentry.m_FieldType = c_FieldTypeMap.get(it->second);
        success++;
    }
    it = key(DIRECTION);
    if (it != attriblist.cend())
    {
        if (it->second == WRITE)
        {
            tableentry.m_Direction = RemoteDirection::Write;
        }
        else
        {
            tableentry.m_Direction = RemoteDirection::Read;
        }
        success++;
    }
    else
    {
        if ((tableentry.m_Category >= RemoteCategory::W_MASSFLOW) &&
            (tableentry.m_Category <= RemoteCategory::W_COUNTER))
        {
            tableentry.m_Direction = RemoteDirection::Write;
        }
        else
        {
            tableentry.m_Direction = RemoteDirection::Read;
        }
        success++;
    }
    it = key(ELEMENTS);
    if (it != attriblist.cend())
    {
        tableentry.m_Elements = base::utils::ustoi(it->second);
        success++;
    }
    it = key(PATH);
    if (it != attriblist.cend())
    {
        tableentry.m_TaskItem = it->second;
        success++;
    }
    return std::move(tableentry);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CProfiBusDeviceXml::Load(const std::string& szfilename)
{
    BOOL result = FALSE;

    XMLDocument doc;
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
    //----------------------------------------------------------------------------
    int32_t taskid = -1;
    result = helper::GetNumber(*config, TASKID, taskid);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << TASKID);
        doc.Clear();
        return FALSE;
    }
    auto& rConfig = Get();
    rConfig.SetTaskId(taskid);
    //----------------------------------------------------------------------------
    std::string type;
    result = helper::GetString(*config, TYPE, type);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << TYPE);
    }
    rConfig.SetProfiNet(FALSE);
    if (!type.empty())
    {
        helper::toUpper(type);
        rConfig.SetProfiNet(type == PROFINET);
    }
    auto table = config->FirstChildElement(TABLE);
    if (!table)
    {
        doc.Clear();
        return FALSE;
    }
    auto& blocklist = rConfig.GetList();
    for (auto itemElement = table->FirstChildElement(ITEM);	itemElement != nullptr; itemElement = itemElement->NextSiblingElement())
    {
        xmlentry attriblist;
        for  (auto attrib = itemElement->FirstAttribute(); attrib != nullptr; attrib = attrib->Next())
        {
            std::string sz;
            sz.append(attrib->Name());
            std::string value;
            value.append(attrib->Value());
            attriblist.emplace_back(std::move(std::make_pair(sz, value)));
        }
        ProfiBusDescriptor vT = SlpitAttributes(attriblist);
        blocklist.push_back(vT);
    }
    doc.Clear();
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CProfiBusDeviceXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));

    auto as = doc.NewElement(TASKID);
    const auto& rList = Get();
    as->SetText(rList.GetTaskId());
    config->InsertEndChild(as);
    as = doc.NewElement(TYPE);
    as->SetText(rList.GetProfiNet() ? PROFINET : PROFIBUS);
    config->InsertEndChild(as);
    auto table = config->InsertEndChild(doc.NewElement(TABLE));
    auto& blocklist = rList.GetList();

    for (auto& rItem : blocklist)
    {
         auto el = doc.NewElement(ITEM);
         const auto& typestring = c_CategoryMap.get(rItem.m_Category);
         el->SetAttribute(CATEGORY, typestring.c_str());
         el->SetAttribute(FIELDTYPE, c_FieldTypeMap.get(rItem.m_FieldType).c_str());
         el->SetAttribute(DIRECTION, (rItem.m_Direction == RemoteDirection::Read)?READ:WRITE);
         el->SetAttribute(ELEMENTS, rItem.m_Elements);
         el->SetAttribute(PATH, rItem.m_TaskItem.c_str());
         table->InsertEndChild(el);
    }
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
}

};
};
