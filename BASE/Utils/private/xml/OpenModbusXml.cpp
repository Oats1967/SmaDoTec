//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module OpenModbusXml
///
/// @file   OpenModbusXml.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <string>
#include "BASE/types.h"
#include "BASE/Utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/OpenModbusXml.h"
#include "Logger/include/Logger.h"

#define CONFIGURATION "PROTOCOL_OPENMODBUS"
#define PORT "Port"
#define SLAVEID "SlaveID"
#define REMOTEIP "Remote-IP"
#define LOCALIP "Local-IP"
#define TIMEOUT "Timeout"


using namespace tinyxml2;

namespace base
{
namespace xml
{

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL COpenModbusXml::Load(const std::string& szfilename)
{
    int32_t port = 300;
    int32_t slaveid = 1;
    int32_t timeout = 200;
    std::string sLocalIP = "192.168.178.2";

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
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << PORT);
    }

    result = helper::GetNumber(*config, SLAVEID, slaveid);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << SLAVEID);
    }
    result = helper::GetNumber(*config, TIMEOUT, timeout);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << TIMEOUT);
    }
    result = helper::GetString(*config, LOCALIP, sLocalIP);
    if ( ! result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << LOCALIP);
        result = helper::GetString(*config, REMOTEIP, sLocalIP);
        if ( ! result)
        {
            // Aus Kompatibilität zu älerten Versionen
            LOGERROR("Error in Xml-file: " << szfilename << " " << REMOTEIP);
        }
    }
    if (result)
    {
        CModbusConfig cfg;

        cfg.SetLocalIP(sLocalIP);
        cfg.SetSlaveID(slaveid);
        cfg.SetPort(port);
        cfg.SetTimeout(timeout);
        Set(cfg);
        LOGDEBUG("Timeout = " << std::dec << cfg.GetTimeout() <<
            " SlaveID = " << std::dec << cfg.GetSlaveID() <<
            " Port = " << std::dec << cfg.GetPort() <<
            " IP   = " << cfg.GetLocalIP());
    }
    doc.Clear();
    return result;
}

BOOL COpenModbusXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));

    const auto& cfg = Get();
    auto key = config->InsertEndChild(doc.NewElement(PORT));
    key->ToElement()->SetText(cfg.GetPort());
    key = config->InsertEndChild(doc.NewElement(SLAVEID));
    key->ToElement()->SetText(cfg.GetSlaveID());
    key = config->InsertEndChild(doc.NewElement(TIMEOUT));
    key->ToElement()->SetText(cfg.GetTimeout());
    key = config->InsertEndChild(doc.NewElement(LOCALIP));
    key->ToElement()->SetText(cfg.GetLocalIP().c_str());
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};


};
};




