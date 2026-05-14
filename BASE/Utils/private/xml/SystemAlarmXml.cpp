//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DBTable
///
/// @file   DBTable.cpp
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
#include "BASE/include/AlarmCategoryMap.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/SystemAlarmXml.h"
#include "Logger/include/Logger.h"

STATIC_KEY(HEADERVERSION, "Version")
STATIC_KEY(CONFIGURATION, "SystemAlarm")
STATIC_KEY(ID, "ID")
STATIC_KEY(CATEGORY, "Category")
STATIC_KEY(ALARM, "Alarm")
STATIC_KEY(NAME, "Name")

using namespace tinyxml2;


const static int32_t c_HeaderVersion = 2;
static int32_t g_HeaderVersion = 0;


namespace base
{
namespace xml
{

 //*********************************************************************************************************************
//*********************************************************************************************************************
 static void SetEntry(const CAlarmErrorMap& ErrorMap, const CAlarmCategoryMap& CategoryMap, XMLDocument& doc, XMLNode& config, const base::eAlarmError alarm, const CAlarmField& field)
 {
#if 0
     if ( ! field.IsAnyAlarm(alarm))
     {
         // Nothing to do
         return;
     }
#endif
     auto alarmkey = config.InsertEndChild(doc.NewElement(ALARM));
     auto nodename = alarmkey->InsertFirstChild(doc.NewElement(NAME));
     const auto& errorstr = ErrorMap.get(alarm);
     nodename->ToElement()->SetText(errorstr.c_str());

     DECLARE_ALARMCATEGORY(fieldcategory)
     for (const auto& catitem : fieldcategory)
     {
         if ( field.Get(alarm, catitem))
         {
             auto nodecat = alarmkey->InsertEndChild(doc.NewElement(CATEGORY));
             nodecat->ToElement()->SetText(CategoryMap.get(catitem).c_str());
         }
     }
}
 //*********************************************************************************************************************
 //*********************************************************************************************************************
 BOOL CSystemAlarmXml::CheckAlarms()
 {
     auto& field = Get();

     if (!field.Get(base::eAlarmError::ERROR_LINE_SHUTDOWN, base::eAlarmCategory::categoryAM))
     {
         field.Set(base::eAlarmError::ERROR_LINE_SHUTDOWN, base::eAlarmCategory::categoryAM, TRUE);
     }
     if (!field.Get(base::eAlarmError::ERROR_DOSE_ALARMSTOP, base::eAlarmCategory::categoryAM))
     {
         field.Set(base::eAlarmError::ERROR_DOSE_ALARMSTOP, base::eAlarmCategory::categoryAM, TRUE);
     }
     return TRUE;
 }


 

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CSystemAlarmXml::Load(const std::string& szfilename)
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
    auto result = helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }
    Set(base::CAlarmField());
    auto& field = Get();
    //const auto& ErrorMapInv     = ! c_FieldErrorMap;
    //const auto& CategoryMapInv  = ! c_FieldCategoryMap;
    for (auto alarmnode = config->FirstChildElement(ALARM);  alarmnode != nullptr; alarmnode = alarmnode->NextSiblingElement())
    {
        std::string sName;
        auto localresult = helper::GetString(*alarmnode, NAME, sName);
        assert(localresult);
        if (localresult)
        {
            const eAlarmError alarmerror = c_FieldErrorMap.get(sName);
            for (auto catel = alarmnode->FirstChildElement(CATEGORY); catel != nullptr; catel = catel->NextSiblingElement())
            {
                std::string sCategory = catel->Value();
                helper::trim(sCategory);
                assert(sCategory == CATEGORY);
                if (sCategory == CATEGORY)
                {
                    field.Set(alarmerror, c_FieldCategoryMap.get(catel->ToElement()->GetText()), TRUE);
                }
            }
        }
    }
    CheckAlarms();
    if (g_HeaderVersion != c_HeaderVersion)
    {
        Save(szfilename);
    }
    doc.Clear();
    return TRUE;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CSystemAlarmXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);

    const auto& field = Get();
    DECLARE_ALARMERROR(alarmfield);
    for (const auto& item : alarmfield)
    {
        SetEntry(c_FieldErrorMap, c_FieldCategoryMap, doc, *config, item, field);
    }
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




