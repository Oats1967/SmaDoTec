  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module EasyGraphSettings
///
/// @file   EasyGraphSettings.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include <cstring>
#include "BASE/types.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/EasyGraphSettingsXml.h"
#include "Logger/include/Logger.h"


STATIC_KEY( CONFIGURATION,"EasyGraphSettings")
STATIC_KEY( HEADERVERSION, "Version")
STATIC_KEY( DATESPAN,"Date")
STATIC_KEY( STARTDATE, "Start")
STATIC_KEY( ENDDATE, "End")
STATIC_KEY( ACTIVELINE, "ActiveLine")
STATIC_KEY( ACTIVEFEEDER, "ActiveFeeder")
STATIC_KEY( REALMONITORINGREFRESHTIME, "RealMonitoringRefreshTime")
STATIC_KEY( REALMONITORINGHISTORY, "RealMonitoringHistoryMinutes")
STATIC_KEY( ATTRIBUTES,"Attributes")
STATIC_KEY( MASSFLOW, "Massflow")
STATIC_KEY( SETPOINT, "Setpoint")
STATIC_KEY( DRIVECOMMAND, "Drivecommand")
STATIC_KEY( NETWEIGHT, "Netweight")
STATIC_KEY( DOSEPERFORMANCE, "Doseperformance")
STATIC_KEY( ROTSPEED, "Rotspeed")
STATIC_KEY( LINEWIDTH, "LineWidth")
STATIC_KEY( VISIBLE, "Visible")
STATIC_KEY( CATEGORY, "Category")
STATIC_KEY( COLOR, "Color")


const static int32_t c_HeaderVersion = 1;
static int32_t g_HeaderVersion = 0;


using namespace tinyxml2;

namespace base
{
namespace xml
{
//*********************************************************************************************************************
//*********************************************************************************************************************
static void String2Date(const char* buf, time_t& value)
{
    assert(buf);
    int32_t m = 0, d = 0, y = 0;
    sscanf_s(buf, "%d.%d.%d", &d, &m, &y);
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    tm.tm_year = y - 1900;
    tm.tm_mday = d;
    tm.tm_mon = m - 1;
    tm.tm_isdst = -1;
    value = mktime(&tm);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
static void Date2String(const time_t& value, char* buf)
{
    assert(buf);
    std::tm tm;
    auto error = ::localtime_s(&tm , &value);
    if (!error)
    {
        int32_t len = _S32(strlen(buf));
        // Format: Mo, 15.06.2009 20:20:00
        std::strftime(buf, len, "%d.%m.%Y", &tm);
    }
    else
    {
       *buf = 0;
    }
}

//*********************************************************************************************************************
//*********************************************************************************************************************
static BOOL GetDate(const XMLElement& element, LPCSTR symbol, time_t& value)
{
    BOOL result = FALSE;
    auto el = element.FirstChildElement(symbol);
    if (el)
    {
        const auto lp = el->GetText();
        if (lp && (strlen(lp) > 0))
        {
            String2Date(lp, value);
            result = TRUE;
        }
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
static BOOL SetDate(XMLElement& element, const time_t& value)
{
    char buffer[32];
    Date2String(value, buffer);
    element.SetText(buffer);
    return TRUE;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
static BOOL GetDate(XMLElement& config, time_t& startTime, time_t& endTime)
{
    BOOL result = FALSE;
    auto date = config.FirstChildElement(DATESPAN);
    if (date)
    {
        auto result1 = GetDate(*date, STARTDATE, startTime);
        auto result2 = GetDate(*date, ENDDATE, endTime);
        result = result1 & result2;
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CEasyGraphSettingsXml::GetAttrib(const XMLElement& config, const base::eMassflowSelect select)
{
    BOOL result = FALSE;

    auto pSelect = config.FirstChildElement(c_MassflowSelectMap.get(select).c_str());
    if (pSelect)
    { 
        auto& attrib = Get().m_Attribues[_S32(select)];
        result = base::xml::helper::GetNumber(*pSelect, LINEWIDTH, attrib.m_LineWidth);
        if ( ! result)
        {
            attrib.m_LineWidth = 1;
            LOGERROR("Error in Xml-file: " << LINEWIDTH);
        }
        std::string szTemp;
        result = base::xml::helper::GetString(*pSelect, VISIBLE, szTemp);
        if ( ! result)
        {
            attrib.m_Visible = TRUE;
            LOGERROR("Error in Xml-file: " << VISIBLE);
        }
        else
        {
            attrib.m_Visible = c_YesNoMap.get(szTemp);
        }
        result = base::xml::helper::GetString(*pSelect, CATEGORY, szTemp);
        if ( ! result)
        {
            attrib.m_Category = LineCategory::eLine;
            LOGERROR("Error in Xml-file: " << CATEGORY);
        }
        else
        {
            attrib.m_Category = c_LineCategoryMap.get(szTemp);
        }
        uint64_t value = 0;
        result = base::xml::helper::GetNumber(*pSelect, COLOR, value);
        if (!result)
        {
            attrib.m_Color = 0xFFFFFFF;
            LOGERROR("Error in Xml-file: " << COLOR);
        }
        else
        {
            attrib.m_Color = _U32(value);
        }
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CEasyGraphSettingsXml::SetAttrib(tinyxml2::XMLDocument& rDoc, XMLNode& config, const base::eMassflowSelect select)
{
    BOOL result = FALSE;

    auto attrib = config.InsertEndChild(rDoc.NewElement(c_MassflowSelectMap.get(select).c_str()));
    if (attrib)
    {
        const auto& rAttrib = Get().m_Attribues[_S32(select)];
        auto keyel = attrib->InsertEndChild(rDoc.NewElement(LINEWIDTH));
        keyel->ToElement()->SetText(rAttrib.m_LineWidth);

        keyel = attrib->InsertEndChild(rDoc.NewElement(VISIBLE));
        keyel->ToElement()->SetText(c_YesNoMap.get(rAttrib.m_Visible).c_str());

        keyel = attrib->InsertEndChild(rDoc.NewElement(CATEGORY));
        keyel->ToElement()->SetText(c_LineCategoryMap.get(rAttrib.m_Category).c_str());

        keyel = attrib->InsertEndChild(rDoc.NewElement(COLOR));
        keyel->ToElement()->SetText(rAttrib.m_Color);
    }
    return result;
}


//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CEasyGraphSettingsXml::Load(const std::string& szfilename)
{
    //m_file = szfilename;

    tinyxml2::XMLDocument doc;
    auto loadOkay = doc.LoadFile(szfilename.c_str());
    if (loadOkay!= XML_SUCCESS)
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
    auto& field = Get();
    result = GetDate(*config, field.m_ShowTime.m_dateStart, field.m_ShowTime.m_dateEnd);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << STARTDATE);
    }
    field.m_ActiveLine = 0;
    result = helper::GetNumber(*config, ACTIVELINE, field.m_ActiveLine);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << ACTIVELINE);
    }
    field.m_ActiveFeeder = 0;
    result = helper::GetNumber(*config, ACTIVEFEEDER, field.m_ActiveFeeder);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << ACTIVEFEEDER);
    }
    field.m_RealMonitoringRefreshTime = 5u;
    result = helper::GetNumber(*config, REALMONITORINGREFRESHTIME, field.m_RealMonitoringRefreshTime);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << REALMONITORINGREFRESHTIME);
        g_HeaderVersion = -1;
    }
    field.m_RealMonitoringHistoryMinutes = 30u;
    result = helper::GetNumber(*config, REALMONITORINGHISTORY, field.m_RealMonitoringHistoryMinutes);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << REALMONITORINGHISTORY);
        g_HeaderVersion = -1;
    }
    auto attrib = config->FirstChildElement(ATTRIBUTES);
    if (!attrib)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << ATTRIBUTES);
    }
    DECLARE_MASSFLOWSELECT(it);
    for ( const auto& rItem : it)
    {
        if (rItem != eMassflowSelect::eVIEWTOTALIZER)
        {
            GetAttrib(*attrib, rItem);
        }
    }
    doc.Clear();
    if (!result)
    {
        result = Save(szfilename);
    }
    return result;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CEasyGraphSettingsXml::Save(const std::string& szfilename)
{
    tinyxml2::XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    assert(config);
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);

    const auto& field = Get();
    // Date
    auto date = config->InsertEndChild(doc.NewElement(DATESPAN));
    auto keyel = date->InsertEndChild(doc.NewElement(STARTDATE));
    SetDate(*keyel->ToElement(), field.m_ShowTime.m_dateStart);
    keyel = date->InsertEndChild(doc.NewElement(ENDDATE));
    SetDate(*keyel->ToElement(), field.m_ShowTime.m_dateEnd);
    keyel = config->InsertEndChild(doc.NewElement(ACTIVELINE));
    keyel->ToElement()->SetText(field.m_ActiveLine);
    keyel = config->InsertEndChild(doc.NewElement(ACTIVEFEEDER));
    keyel->ToElement()->SetText(field.m_ActiveFeeder);
    keyel = config->InsertEndChild(doc.NewElement(REALMONITORINGREFRESHTIME));
    keyel->ToElement()->SetText(field.m_RealMonitoringRefreshTime);
    keyel = config->InsertEndChild(doc.NewElement(REALMONITORINGHISTORY));
    keyel->ToElement()->SetText(field.m_RealMonitoringHistoryMinutes);
    auto attrib = config->InsertEndChild(doc.NewElement(ATTRIBUTES));

    DECLARE_MASSFLOWSELECT(it);
    for (const auto& rItem : it)
    {
        if (rItem != eMassflowSelect::eVIEWTOTALIZER)
        {
            SetAttrib(doc, *attrib, rItem);
        }
    }
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




