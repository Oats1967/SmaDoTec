//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SystemConfigXml
///
/// @file   SystemConfigXml.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <sstream>
#include "BASE/types.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/SystemConfigXml.h"
#include "Logger/include/Logger.h"


STATIC_KEY(CONFIGURATION,"SystemConfig");
STATIC_KEY(HEADERVERSION, ("Version"))
STATIC_KEY(UNITS, "Units");
STATIC_KEY(DOSE, "Dose");
STATIC_KEY(LCTYPE, "LCType");
STATIC_KEY(FEEDINGTYPE, "FeedingType");
STATIC_KEY(TYPE, "Type");
STATIC_KEY(EMPTYFEEDER, "EmptyFeeder");
STATIC_KEY(REFILLSIGNAL, "RefillSignal");
STATIC_KEY(EMPTYFEEDERRUNTIME, "Runtime");
STATIC_KEY(EMPTYFEEDERRESTRICTION, "Restriction");
STATIC_KEY(SEQUENCE, "Sequence");
STATIC_KEY(BLOCK, "Block");
STATIC_KEY(ARCHITECTURE, "Architecture");
STATIC_KEY(MODULES, "Modules")
STATIC_KEY(MODULEREFILL, "ModuleRefill")



const static int32_t c_HeaderVersion = 1;
static int32_t g_HeaderVersion = 0;


using namespace tinyxml2;

namespace base
{
namespace xml
{
//*********************************************************************************************************************
//*********************************************************************************************************************
void  CSystemConfigXml::SetDoseModules(const CDoseModules& rModules, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& dosemodules) const
{
    if ( ! rModules.IsEnabled())
    {
        return;
    }
    auto modules = doc.NewElement(MODULES);
    const auto& rRefillModule = rModules.GetModuleRefill();
    if (rRefillModule.GetEnable())
    {
        auto refillmodule = doc.NewElement(MODULEREFILL);
        {
            // Refillsignal
            const auto& rRefillSignal = rRefillModule.GetRefillSignal();
            auto el = doc.NewElement(REFILLSIGNAL);
            auto rs = doc.NewElement(SEQUENCE);
            rs->ToElement()->SetText(c_RefillTypeMap.get(rRefillSignal.m_eRefillSignal).c_str());
            el->InsertEndChild(rs);

            if ( ! rRefillSignal.m_RefillSignal.empty())
            {
                auto bl = doc.NewElement(BLOCK);
                for (const auto& rItem : rRefillSignal.m_RefillSignal)
                {
                    auto it = doc.NewElement(c_OnOffMap.get(rItem.first).c_str());
                    it->ToElement()->SetText(rItem.second);
                    bl->InsertEndChild(it);
                }
                el->InsertEndChild(bl);
            }
            refillmodule->InsertEndChild(el);
        }
        modules->InsertEndChild(refillmodule);
    }
    /// Hier neue Modules hinzufügen !!!!
    //------------------------------------
    dosemodules.InsertEndChild(modules);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CSystemConfigXml::SetEntry(const int32_t index, const base::CDoseConfig& cfg, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config) const
{
    auto doseel = doc.NewElement(DOSE);
    config.InsertEndChild(doseel);

    std::string szComment = " +++ Dosing unit = " + std::to_string(index + 1) + " +++ ";
    doseel->InsertEndChild(doc.NewComment(szComment.c_str()));

    auto el = doc.NewElement(TYPE);
    el->SetText(c_DoseTypeMap.get(cfg.GetDoseType()).c_str());
    doseel->InsertEndChild(el);

    if (__ISLOADCELLTYPE(cfg.GetDoseType()))
    {
        el = doc.NewElement(LCTYPE);
        el->SetText(c_LCTypeDetectionMap.get(cfg.GetLCTypeDetection()).c_str());
        doseel->InsertEndChild(el);
    }
    el = doc.NewElement(FEEDINGTYPE);
    el->SetText(c_FeedingTypeMap.get(cfg.GetFeedingType()).c_str());
    doseel->InsertEndChild(el);

    // EmptyFeeder
    el = doc.NewElement(EMPTYFEEDER);
    auto fl = doc.NewElement(EMPTYFEEDERRUNTIME);
    fl->ToElement()->SetText(cfg.GetEmptyFeederConfig().emptyFeederRuntime);
    el->InsertEndChild(fl);
    fl = doc.NewElement(EMPTYFEEDERRESTRICTION);
    fl->ToElement()->SetText(c_EmptyFeederTypeMap.get(cfg.GetEmptyFeederConfig().emptyFeederType).c_str());
    el->InsertEndChild(fl);
    doseel->InsertEndChild(el);

    SetDoseModules(cfg.GetDoseModules() , doc, *doseel );
}
//*********************************************************************************************************************
//*********************************************************************************************************************
CEmptyFeederConfig CSystemConfigXml::ReadEmptyFeederConfig(const tinyxml2::XMLElement& doseelement) const
{
    CEmptyFeederConfig aCfg;

    int32_t emptyFeederRuntime = 0;
    auto result = helper::GetNumber(doseelement, EMPTYFEEDERRUNTIME, emptyFeederRuntime);
    if (result)
    {
        aCfg.emptyFeederRuntime = emptyFeederRuntime;
    }
    else
    {
        assert(FALSE);
        LOGERROR("Not found in SystemConfigXml : " << EMPTYFEEDERRUNTIME << "using default = 3600");
    }

    std::string sRestriction;
    result = helper::GetString(doseelement, EMPTYFEEDERRESTRICTION, sRestriction);
    if (result)
    {
        aCfg.emptyFeederType = c_EmptyFeederTypeMap.get(sRestriction);
    }
    else
    {
        assert(FALSE);
        LOGERROR("Not found in SystemConfigXml : " << EMPTYFEEDERRESTRICTION << "using default = none");
    }
    return std::move(aCfg);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
CRefillSignalConfig CSystemConfigXml::ReadRefillSignal(const tinyxml2::XMLElement& doseelement) const
{
    CRefillSignalConfig aCfg;

    std::string szSequence;
    auto result = helper::GetString(doseelement, SEQUENCE, szSequence);
    assert(result);
    if ( ! result)
    {
        LOGERROR("Sequence missing ... using default = NORMAL");
        aCfg.m_RefillSignal.clear();
        aCfg.m_eRefillSignal = base::refillsignal::eRefillSignal::eNormal;
    }
    else
    {
        aCfg.m_eRefillSignal = c_RefillTypeMap.get(szSequence);
        if (aCfg.m_eRefillSignal == base::refillsignal::eRefillSignal::eNormal)
        {
            aCfg.m_RefillSignal.clear();
        }
        else
        {
            CRefillSignalConfig::RefillSignalVec vec;
            auto bl = doseelement.FirstChildElement(BLOCK);
            assert(bl);
            if ( ! bl)
            {
                LOGERROR("Syntax error ... No refillsignal block elements in SystemConfigXml !");
            }
            else
            {
                for (auto of = bl->FirstChildElement(); of != nullptr; of = of->NextSiblingElement())
                {
                    int32_t value = 0;
                    auto localresult = of->QueryIntText(&value);
                    assert(localresult == XML_SUCCESS);
                    vec.push_back({ c_OnOffMap.get(std::string(of->Value())), value });
                }
                assert(!vec.empty());
                if (vec.empty())
                {
                    aCfg.m_eRefillSignal = base::refillsignal::eRefillSignal::eNormal;
                    aCfg.m_RefillSignal.clear();
                    assert(FALSE);
                    LOGERROR("Invalid numbers of refill signals in SystemConfigXml !");
                }
                else
                {
                    uint32_t el = 0;
                    for (uint32_t index = 1U; index < vec.size(); index++)
                    {
                        if (vec[index].first == vec[el].first)
                        {
                            vec[el].second += vec[index].second;
                        }
                        else
                        {
                            aCfg.m_RefillSignal.push_back(vec[el]);
                            el = index;
                        }
                    }
                    aCfg.m_RefillSignal.push_back(vec[el]);
                }
            }
        }
    }
    return std::move(aCfg);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
CDoseModules  CSystemConfigXml::ReadDoseModules(const int32_t , const tinyxml2::XMLElement& dosemodules) const
{
    CDoseModules cfg;

    auto refillel = dosemodules.FirstChildElement(MODULEREFILL);
    if (refillel)
    {
        CModuleRefill refillmodule;

        {
            // refillsignal
            auto rs = refillel->FirstChildElement(REFILLSIGNAL);
            if (!rs)
            {
                LOGDEBUG("Not found in SystemConfigXml : " << REFILLSIGNAL << "using default ");
            }
            else
            {
                refillmodule.SetRefillSignal(ReadRefillSignal(*rs));
            }
        }
        refillmodule.SetEnable(TRUE);
        cfg.SetModuleRefill(refillmodule);
    }
    return std::move(cfg);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
base::CDoseConfig CSystemConfigXml::GetEntry(const int32_t index, const tinyxml2::XMLElement& doseelement)
{
    base::CDoseConfig cfg;

    {
        std::string sType;
        base::eDoseType eType = base::eDoseType::DOSETYPE_NONE;
        auto result = helper::GetString(doseelement, TYPE, sType);
        if (result)
        {
            eType = c_DoseTypeMap.get(sType);
        }
        else
        {
            assert(FALSE);
            LOGERROR("Not found in SystemConfigXml : " << TYPE << "using default = LWS");
            eType = eDoseType::DOSETYPE_LWS;
        }
        cfg.SetDoseType(eType);
    }

    {
        std::string sTemp;
        base::eLcTypeDetection eType = base::eLcTypeDetection::LC_DET_NONE;
        auto result = helper::GetString(doseelement, LCTYPE, sTemp);
        if (result)
        {
            eType = c_LCTypeDetectionMap.get(sTemp);
        }
        else
        {
            if (__ISLOADCELLTYPE(cfg.GetDoseType()))
            {
                assert(FALSE);
                eType = base::eLcTypeDetection::LC_DET_AUTO;
                LOGERROR("Error : " << LCTYPE);
            }
        }
        cfg.SetLCTypeDetection(eType);
    }


    {
        std::string sFeedingType;
        base::eFeedingType type = base::eFeedingType::FF_NONE;
        auto result = helper::GetString(doseelement, FEEDINGTYPE, sFeedingType);
        if (result)
        {
            type = c_FeedingTypeMap.get(sFeedingType);
        }
        else
        {
            assert(FALSE);
            LOGERROR("Not found in SystemConfigXml : " << FEEDINGTYPE << "using default = MF");
            type = eFeedingType::FF_MF;
        }
        cfg.SetFeedingType(type);
    }
    {
        auto fl = doseelement.FirstChildElement(EMPTYFEEDER);
        if (!fl)
        {
            if (__ISLOADCELLTYPE(cfg.GetDoseType()))
            {
                assert(FALSE);
                LOGERROR("Not found in SystemConfigXml : " << EMPTYFEEDER << "using default ");
            }
            cfg.SetEmptyFeederConfig(CEmptyFeederConfig());
        }
        else
        {
            cfg.SetEmptyFeederConfig(ReadEmptyFeederConfig(*fl));
        }
    }
    {
        auto dosemodules = doseelement.FirstChildElement(MODULES);
        if (dosemodules)
        {
            cfg.SetDoseModules(ReadDoseModules(index, *dosemodules));
        }
    }
    return std::move(cfg);
}


//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CSystemConfigXml::Load(const std::string& szfilename)
{
    m_file = szfilename;

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
    auto result = helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }

    auto& field = Get();
    std::string sArchitecture;
    result = helper::GetString(*config, ARCHITECTURE, sArchitecture);
    if (result)
    {
        field.SetArchitecture(c_ConfigMap.get(sArchitecture));
    }
    else
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " " << ARCHITECTURE);
    }
    auto units = config->FirstChildElement(UNITS);
    if (!units)
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " " << UNITS);
        doc.Clear();
        return FALSE;
    }
    int32_t index = 0;
    for  (auto doseelement = units->FirstChildElement(DOSE); doseelement != nullptr; doseelement = doseelement->NextSiblingElement())
    {
        field.AddItem(GetEntry(index, *doseelement));
        index++;
    }
    doc.Clear();
    if (g_HeaderVersion != c_HeaderVersion)
    {
        Save(szfilename);
    }
    return TRUE;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CSystemConfigXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());

    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);

    {
        const char* szHeader = { 
#include "SystemConfigHeader.def"   
        };
        config->InsertEndChild(doc.NewComment(szHeader));
    }
    const auto& field = Get();
    auto keyel = config->InsertEndChild(doc.NewElement(ARCHITECTURE));
    keyel->ToElement()->SetText(c_ConfigMap.get(field.GetArchitecture()).c_str());
    auto units = config->InsertEndChild(doc.NewElement(UNITS));

    for (uint32_t index = 0; index < field.GetCount(); index++)
    {
        SetEntry(index, field.GetItem(index), doc, *units);
    }
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




