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
#include "BASE/Utils/public/StringUtil.h"
#include "BASE/Utils/public/xml/GenericRemoteXml.h"
#include "Logger/include/Logger.h"



STATIC_KEY( CONFIGURATION, "RemoteTable")
STATIC_KEY( TABLE,"Table")
STATIC_KEY( ITEM,"Item")
STATIC_KEY( ARRAY,"Array")
STATIC_KEY( CATEGORY,"CATEGORY")
STATIC_KEY( BASEADDRESS,"BASEADDRESS")
STATIC_KEY( OFFSET,"OFFSET")
STATIC_KEY( FIELDTYPE,"FIELDTYPE")
STATIC_KEY( DIRECTION,"DIRECTION")
STATIC_KEY( ELEMENTS,"ELEMENTS")
STATIC_KEY( WRITE,"WRITE")
STATIC_KEY( READ,"READ")
STATIC_KEY( STARTIX,"STARTIX")
STATIC_KEY( ENDIAN,"ENDIAN")
STATIC_KEY( BIGENDIAN,"BIGENDIAN")
STATIC_KEY( SCALE,"SCALE")
STATIC_KEY( ADDRESSFORMAT,"ADDRESSFORMAT")
STATIC_KEY( BYTEALIGN,"BYTEALIGN")
STATIC_KEY( WORDALIGN,"WORDALIGN")
STATIC_KEY(HEADERVERSION, ("Version"))


//--------------------------------------------------------------------------------
STATIC_KEY( PROCESSCONTROL,"ProcessControl")
STATIC_KEY( PROCESSSTATUS  ,"ProcessStatus")
STATIC_KEY( ALARMSTATUS,"AlarmStatus")
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
STATIC_KEY( LINEPROCESSCONTROL,"LineProcessControl")
STATIC_KEY( LINEPROCESSSTATUS,"LineProcessStatus")
STATIC_KEY( LINEALARMSTATUS,"LineAlarmStatus")
STATIC_KEY( PARAMETERMASK,"ParameterMask")


//--------------------------------------------------------------------------------

const static int32_t c_HeaderVersion = 3;
static int32_t g_HeaderVersion = 0;


using namespace tinyxml2;

namespace base
{
namespace xml
{
//*********************************************************************************************************************
//*********************************************************************************************************************
BlockDescriptor CGenericRemoteXml::SlpitAttributes( const xmlentry& attriblist) const
{
    BlockDescriptor tableentry;

    auto key = [&attriblist](const std::string& str) {
        return std::find_if(attriblist.cbegin(), attriblist.cend(), [&str](const auto& item)-> BOOL { return item.first == str; });
    };

    auto it = key(CATEGORY);
    if ( it != attriblist.cend())
    {
        tableentry.m_Category = c_CategoryMap.get(it->second);
    }
    it = key(BASEADDRESS);
    if (it != attriblist.cend())
    {
        if (!it->second.empty())
        {
            tableentry.m_BaseAddress = base::utils::ustoi(it->second);
        }
        else
        {
            tableentry.m_BaseAddress = 0;
            LOGERROR("No valid baseaddress");
        }
    }
    else
    {
        LOGDEBUG("No valid baseaddress, set baseaddress to category");
        tableentry.m_BaseAddress = _U32(tableentry.m_Category);
    }
    it = key(OFFSET);
    if (it != attriblist.cend())
    {
        if (!it->second.empty())
        {
            tableentry.m_StartOffset = base::utils::ustoi(it->second);
        }
        else
        {
            tableentry.m_StartOffset = 0;
            LOGERROR("No valid offset");
        }
    }
    else
    {
        LOGDEBUG("No valid offset, set offset to 0");
        tableentry.m_StartOffset = 0;
    }

    it = key(FIELDTYPE);
    if (it != attriblist.cend())
    {
        tableentry.m_FieldType = c_FieldTypeMap.get(it->second);
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
    }
    it = key(ELEMENTS);
    if (it != attriblist.cend())
    {
        tableentry.m_Elements = base::utils::ustoi(it->second);
    }

    tableentry.m_StartIx = 0;
    it = key(STARTIX);
    if (it != attriblist.cend())
    {
        tableentry.m_StartIx = base::utils::ustoi(it->second);
    }

    tableentry.m_bigEndian = FALSE;
    it = key(ENDIAN);
    if (it != attriblist.cend())
    {
        if (it->second == BIGENDIAN)
        {
            tableentry.m_bigEndian = TRUE;
        }
    }
    tableentry.m_Scale = 0.0F;
    it = key(SCALE);
    if (it != attriblist.cend())
    {
        tableentry.m_Scale = base::utils::ustof(it->second);
    }
    tableentry.m_WordAlign = FALSE;
    it = key(ADDRESSFORMAT);
    if (it != attriblist.cend())
    {
        if (it->second == WORDALIGN)
        {
            // Addressen mit 2 multiplizieren.
            tableentry.m_WordAlign = TRUE;
        }
    }
    return std::move(tableentry);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
xmlattributevector CGenericRemoteXml::ParseElements(const XMLNode& config )
{
    xmlattributevector rList;
    for (auto el = config.FirstChildElement(); el != NULL; el = el->NextSiblingElement())
    {
        int32_t value = 0;
        auto error = el->ToElement()->QueryIntText(&value);
        if (error == XML_SUCCESS)
        {
            rList.push_back({ el->Value(), value });
        }
        else
        {
            LOGERROR("Error reading element :" << el->Value());
        }
    }
    return std::move(rList);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::WriteElements(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config, const std::string& rName, const xmlattributevector& field)
{
    if (field.empty())
    {
        LOGDEBUG("Nothing to write ..." << rName);
        return;
    }
    config.InsertEndChild(doc.NewComment(rName.c_str()));
    auto el = doc.NewElement(rName.c_str());
    config.InsertEndChild(el);
    for (const auto& item : field)
    {
       auto as = doc.NewElement(item.first.c_str());
       as->SetText(std::to_string(item.second).c_str());
       el->InsertEndChild(as);
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::ReadEntry(const XMLElement& config, base::ProcessControlDescriptor& field)
{
    auto pcontrol = config.FirstChildElement(PROCESSCONTROL);
    if (!pcontrol)
    {
        LOGERROR("Error in Xml-file: " << PROCESSCONTROL);
    }
    else
    {
        field = c_ProcessControlMap.Fill(ParseElements(*pcontrol));
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::WriteEntry(XMLDocument& doc, XMLNode& config, const base::ProcessControlDescriptor& field)
{
    WriteElements(doc, config, PROCESSCONTROL, c_ProcessControlMap.Convert(field));
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::ReadEntry(const XMLElement& config, base::ProcessStatusDescriptor& field)
{
    auto pcontrol = config.FirstChildElement(PROCESSSTATUS);
    if (!pcontrol)
    {
        LOGERROR("Error in Xml-file: " << PROCESSSTATUS);
    }
    else
    {
        field = c_ProcessStatusMap.Fill(ParseElements(*pcontrol));
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::WriteEntry(XMLDocument& doc, XMLNode& config, const base::ProcessStatusDescriptor& field)
{
    WriteElements(doc, config, PROCESSSTATUS, c_ProcessStatusMap.Convert(field));
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::ReadEntry(const XMLElement& config, base::AlarmStatusDescriptor& field)
{
    auto pcontrol = config.FirstChildElement(ALARMSTATUS);
    if (!pcontrol)
    {
        LOGERROR("Error in Xml-file: " << ALARMSTATUS);
    }
    else
    {
        field = c_AlarmStatusMap.Fill(ParseElements(*pcontrol));
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::WriteEntry(XMLDocument& doc, XMLNode& config, const base::AlarmStatusDescriptor& field)
{
    WriteElements(doc, config, ALARMSTATUS, c_AlarmStatusMap.Convert(field));
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::ReadEntry(const XMLElement& config, base::LineProcessControlDescriptor& field)
{
    auto pcontrol = config.FirstChildElement(LINEPROCESSCONTROL);
    if (!pcontrol)
    {
        LOGERROR("Not found in Xml-file: " << LINEPROCESSCONTROL);
    }
    else
    {
        field = c_LineProcessControlMap.Fill(ParseElements(*pcontrol));
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::WriteEntry(XMLDocument& doc, XMLNode& config, const base::LineProcessControlDescriptor& field)
{
    WriteElements(doc, config, LINEPROCESSCONTROL, c_LineProcessControlMap.Convert(field));
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::ReadEntry(const XMLElement& config, base::LineAlarmStatusDescriptor& field)
{
    auto pcontrol = config.FirstChildElement(LINEALARMSTATUS);
    if (!pcontrol)
    {
        LOGERROR("Not found in Xml-file: " << LINEALARMSTATUS);
    }
    else
    {
        field = c_LineAlarmStatusMap.Fill(ParseElements(*pcontrol));
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::WriteEntry(XMLDocument& doc, XMLNode& config, const base::LineAlarmStatusDescriptor& field)
{
    WriteElements(doc, config, LINEALARMSTATUS, c_LineAlarmStatusMap.Convert(field));
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::ReadEntry(const XMLElement& config, base::LineProcessStatusDescriptor& field)
{
    auto pcontrol = config.FirstChildElement(LINEPROCESSSTATUS);
    if (!pcontrol)
    {
        LOGERROR("Not found in Xml-file: " << LINEPROCESSSTATUS);
    }
    else
    {
        field = c_LineProcessStatusMap.Fill(ParseElements(*pcontrol));
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::WriteEntry(XMLDocument& doc, XMLNode& config, const base::LineProcessStatusDescriptor& field)
{
    WriteElements(doc, config, LINEPROCESSSTATUS, c_LineProcessStatusMap.Convert(field));
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BlockDescriptor CGenericRemoteXml::ReadItem(const XMLElement& config) const
{
    auto attriblist = helper::ReadAttribList(config, TRUE);
    assert(!attriblist.empty());
    if (attriblist.empty())
    {
        throw std::length_error("attribList empty");
    }
    return SlpitAttributes(attriblist);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void  CGenericRemoteXml::ReadBlockItem(const XMLElement& config, BlockDescriptorVec& blocklist) const
{
    try
    {
        blocklist.push_back(ReadItem(config));
    }
    catch (...)
    {
        LOGERROR("Error ReadItem");
    }
}

//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::ReadBlockArray(const XMLElement& config, BlockDescriptorVec& blocklist) const
{
    auto attriblist = helper::ReadAttribList(config, TRUE);
    assert(!attriblist.empty());
    if (attriblist.empty())
    {
        LOGERROR("attriblist empty !")
    }
    else
    {
        auto vT = SlpitAttributes(attriblist);
        assert(vT.m_Elements > 1);
        if (vT.m_Elements == 0)
        {
            LOGERROR("invalid number of elements !")
        }
        else
        {
            BlockDescriptorVec arrayList;
            for (auto itemElement = config.FirstChildElement(ITEM); itemElement != nullptr; itemElement = itemElement->NextSiblingElement())
            {
                try
                {
                    auto vI = ReadItem(*itemElement);
                    vI.m_Elements = 1;
                    vI.m_BaseAddress = vT.m_BaseAddress;
                    arrayList.push_back(vI);
                }
                catch (...)
                {
                    assert(FALSE);
                    LOGERROR("ReadItem failed !")
                }
            }
            assert(!arrayList.empty());
            if (arrayList.empty())
            {
                assert(FALSE);
                LOGERROR("ArrayList empty !")
            }
            else
            {
                uint32_t startoffset = vT.m_StartOffset;
                for (uint32_t index = 1U; index <= vT.m_Elements; index++)
                {
                    for (auto& arrayItem : arrayList)
                    {
                        arrayItem.m_StartOffset = startoffset;
                        arrayItem.m_StartIx     = index;
                        blocklist.push_back(arrayItem);
                        startoffset += CGenericRemoteConfig::getSize(arrayItem.m_FieldType);
                    }
                }
            }
        }
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::ReadEntry(const XMLNode& config, base::MaskDescriptor& field)
{
    auto pcontrol = config.FirstChildElement(PARAMETERMASK);
    if (!pcontrol)
    {
        LOGERROR("Not found in Xml-file: " << PARAMETERMASK);
    }
    else
    {
        field = c_RemoteMaskMap.Fill(ParseElements(*pcontrol));
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CGenericRemoteXml::WriteEntry(XMLDocument& doc, XMLNode& config, const base::MaskDescriptor& field)
{
    WriteElements(doc, config, PARAMETERMASK, c_RemoteMaskMap.Convert(field));
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CGenericRemoteXml::Load(const std::string& szfilename)
{
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
    auto result = base::xml::helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }

    // ------------------------------------------------------------------------
    (void)ReadEntry(*config, Get().GetProcessControlDescriptor());
    // ------------------------------------------------------------------------
    (void)ReadEntry(*config, Get().GetProcessStatusDescriptor());
    // ------------------------------------------------------------------------
    (void)ReadEntry(*config, Get().GetAlarmStatusDescriptor());
    // ------------------------------------------------------------------------
    (void)ReadEntry(*config, Get().GetLineProcessControlDescriptor());
    // ------------------------------------------------------------------------
    (void)ReadEntry(*config, Get().GetLineProcessStatusDescriptor());
    // ------------------------------------------------------------------------
    (void)ReadEntry(*config, Get().GetLineAlarmStatusDescriptor());
    // ------------------------------------------------------------------------
    (void)ReadEntry(*config, Get().GetMaskDescriptor());

    //----------------------------------------------------------------------------
    auto table = config->FirstChildElement(TABLE);
    if (!table)
    {
        doc.Clear();
        return FALSE;
    }
    auto& blocklist = Get().GetList();
    for (auto itemElement = table->FirstChildElement();	itemElement != nullptr; itemElement = itemElement->NextSiblingElement())
    {
        auto node = std::string(itemElement->Name());
        if (node == ITEM)
        {
            ReadBlockItem(*itemElement, blocklist);
        }
        else if (node == ARRAY)
        {
            ReadBlockArray(*itemElement, blocklist);
        }
        else
        {
            result = FALSE;
            assert(FALSE);
        }
    }
    doc.Clear();
    result = ! blocklist.empty();
    if (result)
    {
        result = CheckResults();
    }
#if 0
    if (g_HeaderVersion != c_HeaderVersion)
    {
      Save(szfilename);
    }
#endif
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CGenericRemoteXml::CheckResults()
{
    BOOL erg = TRUE;

    auto& cfg = Get();
    auto& blocklist = cfg.GetList();
    auto find = [&blocklist](const base::RemoteCategory category)
    {
        auto it = std::find_if(blocklist.cbegin(), blocklist.cend(), [&category](const BlockDescriptor& item) { return item.m_Category == category; });
        return (it != blocklist.cend());
    };

    {
        auto& descr = cfg.GetAlarmStatusDescriptor();
        auto result = find(base::RemoteCategory::W_ALARMSTATUS);
        if (result)
        {
            if ( descr.empty() )
            {
                LOGERROR("W_ALARMSTATUS used but no bits configured !");
                erg = FALSE;
            }
        }
        else if ( ! descr.empty() )
        {
            descr.clear();
        }
        else {}
    }

    {
        auto& descr = cfg.GetProcessStatusDescriptor();
        auto result = find(base::RemoteCategory::W_PROCESSSTATUS);
        if (result)
        {
            if ( descr.empty())
            {
                LOGERROR("W_PROCESSSTATUS used but no bits configured !");
                erg = FALSE;
            }
        }
        else if ( ! descr.empty())
        {
            descr.clear();
        }
        else {}
    }

    {
        auto& descr = cfg.GetProcessControlDescriptor();
        auto result = find(base::RemoteCategory::R_CONTROL);
        if (result)
        {
            if (descr.empty() )
            {
                LOGERROR("R_CONTROL used but no bits configured !");
                erg = FALSE;
            }
        }
        else if (! descr.empty())
        {
            descr.clear();
        }
        else {}
    }

    {
        auto& descr = cfg.GetLineAlarmStatusDescriptor();
        auto result = find(base::RemoteCategory::W_LINEALARMSTATUS);
        if (result)
        {
            if ( descr.empty())
            {
                LOGERROR("W_LINEALARMSTATUS used but no bits configured !");
                erg = FALSE;
            }
        }
        else if ( ! descr.empty())
        {
            descr.clear();
        }
        else {}
    }

    {
        auto& descr = cfg.GetLineProcessStatusDescriptor();
        auto result = find(base::RemoteCategory::W_LINEPROCESSSTATUS);
        if (result)
        {
            if (descr.empty())
            {
                LOGERROR("W_LINEPROCESSSTATUS used but no bits configured !");
                erg = FALSE;
            }
        }
        else if ( ! descr.empty())
        {
            descr.clear();
        }
        else {}
    }

    {
        auto& descr = cfg.GetLineProcessControlDescriptor();
        auto result = find(base::RemoteCategory::R_LINECONTROL);
        if (result)
        {
            if (descr.empty())
            {
                LOGERROR("R_LINECONTROL used but no bits configured !");
                erg = FALSE;
            }
        }
        else if (!descr.empty())
        {
            descr.clear();
        }
        else {}
    }
    return erg;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CGenericRemoteXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    assert(config);
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    assert(header);
    header->ToElement()->SetText(c_HeaderVersion);

    const char* szHeader = {
#include "GenericRemoteHeader.def"
    };
    config->InsertEndChild(doc.NewComment(szHeader));

    WriteEntry(doc, *config, Get().GetProcessControlDescriptor());
    WriteEntry(doc, *config, Get().GetProcessStatusDescriptor());
    WriteEntry(doc, *config, Get().GetAlarmStatusDescriptor());
    WriteEntry(doc, *config, Get().GetLineProcessControlDescriptor());
    WriteEntry(doc, *config, Get().GetLineProcessStatusDescriptor());
    WriteEntry(doc, *config, Get().GetLineAlarmStatusDescriptor());
    WriteEntry(doc, *config, Get().GetMaskDescriptor());
    auto table = config->InsertEndChild(doc.NewElement(TABLE));
    auto& blocklist = Get().GetList();
    for (auto& rItem : blocklist)
    {
         auto el = doc.NewElement(ITEM);
         const auto& typestring = c_CategoryMap.get(rItem.m_Category);
         el->SetAttribute(CATEGORY, typestring.c_str());
         el->SetAttribute(BASEADDRESS, rItem.m_BaseAddress);
         el->SetAttribute(OFFSET, rItem.m_StartOffset);
         el->SetAttribute(FIELDTYPE, c_FieldTypeMap.get(rItem.m_FieldType).c_str());
         el->SetAttribute(DIRECTION, (rItem.m_Direction == RemoteDirection::Read)?READ:WRITE);
         el->SetAttribute(ELEMENTS, rItem.m_Elements);
         if (rItem.m_StartIx > 0)
         {
             el->SetAttribute(STARTIX, rItem.m_StartIx);
         }
         if (rItem.m_bigEndian)
         {
             el->SetAttribute(ENDIAN, BIGENDIAN);
         }
         if (rItem.m_Scale != 0.0F)
         {
             el->SetAttribute(SCALE, rItem.m_Scale);
         }
         if (rItem.m_WordAlign)
         {
             el->SetAttribute(ADDRESSFORMAT, WORDALIGN);
         }
         table->InsertEndChild(el);
    }
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
}

};
};
