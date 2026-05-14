//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RemoteServerXml
///
/// @file   RemoteServerXml.cpp
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
#include "BASE/types.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/RemoteServerXml.h"
#include "Logger/include/Logger.h"

STATIC_KEY(HEADERDECLARATION, "RemoteSercer configuration")
STATIC_KEY(HEADERVERSION, "Version")
STATIC_KEY(DEVICE, "Device")
STATIC_KEY(TABLE, "Table")
STATIC_KEY(CONFIGURATION, "RemoteServer")
STATIC_KEY(PROTOCOL, "Protocol")
STATIC_KEY(PROPERTY, "Property")

const static int32_t c_HeaderVersion = 1;
static int32_t g_HeaderVersion = 0;


using namespace tinyxml2;

namespace base
{
namespace xml
{
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRemoteServerXml::Load(const std::string& szfilename)
{
    XMLDocument doc;
    auto loadOkay = doc.LoadFile(szfilename.c_str());
    if (loadOkay != XML_SUCCESS)
    {
        assert(FALSE);
        LOGERROR("Error loading xml-file: " << szfilename << "; " << doc.ErrorStr());
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
    auto result = helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }
    auto& field = Get();
    field.m_Protocol = RemoteServerProtocol::REMOTE_PROTOCOL_NONE;
    field.m_TablePath = "";
    field.m_DevicePath = "";

    {
        std::string sProtocol;
        result = helper::GetString(*config, PROTOCOL, sProtocol);
        if ( ! result)
        {
            assert(FALSE);
            LOGERROR("Error in Xml-file: " << szfilename << " " << PROTOCOL);
            field.m_Protocol = RemoteServerProtocol::REMOTE_PROTOCOL_NONE;
            g_HeaderVersion = -1;
        }
        else
        {
            field.m_Protocol = m_RemoteProtocolMap.get(sProtocol);
        }
    }
    if ( field.m_Protocol != RemoteServerProtocol::REMOTE_PROTOCOL_NONE)
    {
        auto pp = config->FirstChildElement(PROPERTY);
        if (! pp )
        {
            assert(FALSE);
            LOGERROR("Error in Xml-file: " << szfilename << " " << PROPERTY);
            g_HeaderVersion = -1;
        }
        else
        {
            result = helper::GetString(*pp, DEVICE, field.m_DevicePath);
            if ( ! result)
            {
                assert(FALSE);
                LOGERROR("Error in Xml-file: " << szfilename << " " << DEVICE);
                g_HeaderVersion = -1;
            }
            result = helper::GetString(*pp, TABLE, field.m_TablePath);
            if ( ! result)
            {
                assert(FALSE);
                LOGERROR("Error in Xml-file: " << szfilename << " " << TABLE);
                g_HeaderVersion = -1;
            }
        }
    }
    doc.Clear();
    if (g_HeaderVersion != c_HeaderVersion)
    {
        Save(szfilename);
    }
    return TRUE;
}
//*************************************************************************************************************************************
//*************************************************************************************************************************************
BOOL CRemoteServerXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    (void)doc.InsertFirstChild(doc.NewDeclaration());
    (void)doc.InsertEndChild(doc.NewComment(HEADERDECLARATION));
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);

    {
        const char* szHeader = {
#include "RemoteServerHeader.def"   
        };
        config->InsertEndChild(doc.NewComment(szHeader));
    }
    const auto& cfg = Get();
    auto key = config->InsertEndChild(doc.NewElement(PROTOCOL));
    key->ToElement()->SetText(m_RemoteProtocolMap.get(cfg.m_Protocol).c_str());
    if (cfg.m_Protocol != RemoteServerProtocol::REMOTE_PROTOCOL_NONE)
    {
        auto pp = config->InsertEndChild(doc.NewElement(PROPERTY));
        auto dv = pp->InsertEndChild(doc.NewElement(DEVICE));
        dv->ToElement()->SetText(cfg.m_DevicePath.c_str());
        auto tb = pp->InsertEndChild(doc.NewElement(TABLE));
        tb->ToElement()->SetText(cfg.m_TablePath.c_str());
    }
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
}

};
};




