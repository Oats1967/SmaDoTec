//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module S7SnapXml
///
/// @file   S7SnapXml.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/Base.h"
#include "BASE/Utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/S7SnapXml.h"
#include "Logger/include/Logger.h"

#define CONFIGURATION "PROTOCOL_S7SNAP"
#define PORT "Port"
#define RACK "Rack"
#define SLOT "Slot"
#define REMOTEIP "Remote-IP"
#define NETLINK "Netlink"
#define TIMEOUT "Timeout"
#define HEADLINE "xml-configuration file for S7-device using S7Snap library"

using namespace tinyxml2;

namespace base
{
namespace xml
{
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CS7SnapXml::Load(const std::string& szfilename)
{
    int32_t port = 300;
    int32_t rack = 1;
    int32_t slot = 2;
    int32_t netlink = 0;
    int32_t timeout = 200;
    std::string sRemoteIP = "192.168.178.2";

    XMLDocument doc;
    auto loadOkay = doc.LoadFile(szfilename.c_str());
    if (loadOkay != XML_SUCCESS)
    {
        LOGERROR("Error loading xml-file: " << szfilename << "; " << doc.ErrorStr());
        return FALSE;
    }
    auto config = doc.FirstChildElement(CONFIGURATION);
    if (!config)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << CONFIGURATION);
        doc.Clear();
        return FALSE;
    }

    BOOL result = helper::GetNumber(*config, PORT, port);
    if ( ! result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << PORT);
    }

    result = helper::GetNumber(*config, RACK, rack);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << RACK);
    }
    result = helper::GetNumber(*config, SLOT, slot);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << SLOT);
    }
    result = helper::GetNumber(*config, NETLINK, netlink);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << NETLINK);
    }
    result = helper::GetNumber(*config, TIMEOUT, timeout);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << TIMEOUT);
    }
    result = helper::GetString(*config, REMOTEIP, sRemoteIP);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << REMOTEIP);
    }
    if (result)
    {
        CDeviceConfig cfg;

        cfg.SetRemoteIP(sRemoteIP);
        cfg.SetNetlink(netlink);
        cfg.SetPort(port);
        cfg.SetSlot(slot);
        cfg.SetTimeout(timeout);
        cfg.SetRack(rack);
        Set(cfg);
        LOGDEBUG("Timeout = " << std::dec << cfg.GetTimeout() <<
            " Slot = " << std::dec << cfg.GetSlot() <<
            " Rack = " << std::dec << cfg.GetRack() <<
            " Port = " << std::dec << cfg.GetPort() <<
            " Netlink = " << std::dec << cfg.GetNetlink() <<
            " IP   = " << cfg.GetRemoteIP());
    }
    doc.Clear();
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CS7SnapXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    doc.InsertEndChild(doc.NewComment(HEADLINE));
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));

    const auto& cfg = Get();
    auto key = config->InsertEndChild(doc.NewElement(PORT));
    key->ToElement()->SetText(cfg.GetPort());
    key = config->InsertEndChild(doc.NewElement(RACK));
    key->ToElement()->SetText(cfg.GetRack());
    key = config->InsertEndChild(doc.NewElement(SLOT));
    key->ToElement()->SetText(cfg.GetSlot());
    key = config->InsertEndChild(doc.NewElement(NETLINK));
    key->ToElement()->SetText(cfg.GetNetlink());
    key = config->InsertEndChild(doc.NewElement(TIMEOUT));
    key->ToElement()->SetText(cfg.GetTimeout());
    key = config->InsertEndChild(doc.NewElement(REMOTEIP));
    key->ToElement()->SetText(cfg.GetRemoteIP().c_str());
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};






