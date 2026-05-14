//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecipefileXml
///
/// @file   RecipefileXml.cpp
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
#include "BASE/types.h"
#include "BASE/include/Round.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/RecipefileXml.h"
#include "Logger/include/Logger.h"

#define CONFIGURATION "Recipe"
#define DOSE "Dose"
#define UNITS "Units"
#define NAME "Name"
#define QMNUMBER "QMNumber"
#define LOCALMODE "LocalMode"

#define SETPOINT "Setpoint"
#define PERCENTAGE "Percentage"
#define REGENERAT "Regenerat"
#define REFILLMAX "RefillMax"
#define REFILLMIN "RefillMin"
#define REFILLALARMLIMIT "RefillAlarmLimit"
#define DOSETYPE "DoseType"
#define DOSEPERFORMANCE "DosePerformance"


using namespace tinyxml2;


namespace base
{
namespace xml
{
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipefileXml::SetEntry(const int32_t index, const utils::CRecipeItem& cfg, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config)
{
    auto doseel = doc.NewElement(DOSE);
    config.InsertEndChild(doseel);

    std::string szComment = " +++ Dosing unit = " + std::to_string(index + 1) + " +++ ";
    doseel->InsertEndChild(doc.NewComment(szComment.c_str()));

    auto el = doc.NewElement(DOSETYPE);
    el->SetText(c_DoseTypeMap.get(cfg.GetDoseType()).c_str());
    doseel->InsertEndChild(el);

    el = doc.NewElement(NAME);
    el->SetText(cfg.GetName().c_str());
    doseel->InsertEndChild(el);

    el = doc.NewElement(QMNUMBER);
    el->SetText(cfg.GetQMNumber());
    doseel->InsertEndChild(el);

    el = doc.NewElement(LOCALMODE);
    el->SetText(c_YesNoMap.get(cfg.GetLocalMode()).c_str());
    doseel->InsertEndChild(el);

    el = doc.NewElement(SETPOINT);
    helper::SetFloatRound(*el, cfg.GetSetpoint());
    doseel->InsertEndChild(el);

    el = doc.NewElement(PERCENTAGE);
    helper::SetFloatRound(*el, cfg.GetRecipePercentage());
    doseel->InsertEndChild(el);

    el = doc.NewElement(REGENERAT);
    el->SetText(c_YesNoMap.get(cfg.GetRegenerat()).c_str());
    doseel->InsertEndChild(el);

    el = doc.NewElement(REFILLMAX);
    helper::SetFloatRound(*el, cfg.GetRefillMax());
    doseel->InsertEndChild(el);

    el = doc.NewElement(REFILLMIN);
    helper::SetFloatRound(*el, cfg.GetRefillMin());
    doseel->InsertEndChild(el);

    el = doc.NewElement(REFILLALARMLIMIT);
    helper::SetFloatRound(*el, cfg.GetRefillAlarmLimit());
    doseel->InsertEndChild(el);

    el = doc.NewElement(DOSEPERFORMANCE);
    helper::SetFloatRound(*el, cfg.GetDosePerformance());
    doseel->InsertEndChild(el);
}

//*********************************************************************************************************************
//*********************************************************************************************************************
utils::CRecipeItem CRecipefileXml::GetEntry(const tinyxml2::XMLElement& doseelement) const
{
    utils::CRecipeItem cfg;

    {
        std::string temp;
        base::eDoseType eType = base::eDoseType::DOSETYPE_NONE;
        auto result = helper::GetString(doseelement, DOSETYPE, temp);
        if (!result)
        {
            assert(FALSE);
            eType = base::eDoseType::DOSETYPE_LWS;
            LOGERROR("Not found in RecipefileXml : " << DOSETYPE << "using default = LWS");
        }
        else
        {
            eType = c_DoseTypeMap.get(temp);
        }
        cfg.SetDoseType(eType);
    }
    {
        std::string sName;
        auto result = helper::GetString(doseelement, NAME, sName);
        if (! result)
        {
            assert(FALSE);
            sName = "TEST";
            LOGERROR("Not found in RecipefileXml : " << NAME << "using default = " << sName);
        }
        cfg.SetName(sName);
    }

    {
        uint64_t sQMNumber;
        auto result = helper::GetNumber(doseelement, QMNUMBER, sQMNumber);
        if ( ! result)
        {
            assert(FALSE);
            sQMNumber = 123456789U;
            LOGERROR("Not found in RecipefileXml : " << QMNUMBER << "using default = " << sQMNumber);
        }
        cfg.SetQMNumber(sQMNumber);
    }
    {
        std::string sLocalMode;
        BOOL bLocalMode = FALSE;
        auto result = helper::GetString(doseelement, LOCALMODE, sLocalMode);
        if (!result)
        {
            assert(FALSE);
            bLocalMode = FALSE;
            LOGERROR("Not found in RecipefileXml : " << LOCALMODE << "using default = FALSE");
        }
        else
        {
            bLocalMode = c_YesNoMap.get(sLocalMode);
        }
        cfg.SetLocalMode(bLocalMode);
    }

    {
        float32_t setpoint = 0.0F;
        auto result = helper::GetNumber(doseelement, SETPOINT, setpoint);
        if (!result)
        {
            assert(FALSE);
            LOGERROR("Not found in RecipefileXml : " << SETPOINT << "using default = 5.0");
            setpoint = 5.0F;
        }
        cfg.SetSetpoint(setpoint);
    }
    {
        float32_t percentage = 0.0F;
        auto result = helper::GetNumber(doseelement, PERCENTAGE, percentage);
        if (!result)
        {
            assert(FALSE);
            LOGERROR("Not found in RecipefileXml : " << PERCENTAGE << "using default = 10.0");
            percentage = 10.0F;
        }
        cfg.SetRecipePercentage(percentage);
    }
    {
        std::string temp;
        BOOL bRegenerat = FALSE;
        auto result = helper::GetString(doseelement, REGENERAT, temp);
        if (! result)
        {
            assert(FALSE);
            LOGERROR("Not found in RecipefileXml : " << REGENERAT << "using default = FALSE");
            bRegenerat = FALSE;
        }
        else
        {
            bRegenerat = c_YesNoMap.get(temp);
        }
        cfg.SetRegenerat(bRegenerat);
    }
    {
        float32_t refillMax = 0.0F;
        auto result = helper::GetNumber(doseelement, REFILLMAX, refillMax);
        if ( ! result)
        {
            assert(FALSE);
            refillMax = 15.0F;
            LOGERROR("Not found in RecipefileXml : " << REFILLMAX << "using default = " << refillMax);
        }
        cfg.SetRefillMax(refillMax);
    }
    {
        float32_t refillMin = 0.0F;
        auto result = helper::GetNumber(doseelement, REFILLMIN, refillMin);
        if (!result)
        {
            assert(FALSE);
            refillMin = 10.0F;
            LOGERROR("Not found in RecipefileXml : " << REFILLMIN << "using default = " << refillMin);
        }
        cfg.SetRefillMin(refillMin);
    }
    {
        float32_t refillAlarmLimit = 0.0F;
        auto result = helper::GetNumber(doseelement, REFILLALARMLIMIT, refillAlarmLimit);
        if (!result)
        {
            assert(FALSE);
            refillAlarmLimit = 1.0F;
            LOGERROR("Not found in RecipefileXml : " << REFILLALARMLIMIT << "using default = " << refillAlarmLimit);
        }
        cfg.SetRefillAlarmLimit(refillAlarmLimit);
    }
    {
        float32_t doseperformance = 0.0F;
        auto result = helper::GetNumber(doseelement, DOSEPERFORMANCE, doseperformance);
        if (!result)
        {
            assert(FALSE);
            doseperformance = 1.0F;
            LOGERROR("Not found in RecipefileXml : " << DOSEPERFORMANCE << "using default = " << doseperformance);
        }
        cfg.SetDosePerformance(doseperformance);
    }
    return cfg;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRecipefileXml::Load(const std::string& szfilename)
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
    auto& field = Get();
    field.Clear();

    {
        std::string sName;
        auto result = helper::GetString(*config, NAME, sName);
        if (!result)
        {
            assert(FALSE);
            sName = "TEST";
            LOGERROR("Not found in RecipefileXml : " << NAME << "using default = " << sName);
        }
        field.SetName(sName);
    }

    {
        uint64_t sQMNumber;
        auto result = helper::GetNumber(*config, QMNUMBER, sQMNumber);
        if (!result)
        {
            assert(FALSE);
            sQMNumber = 123456789;
            LOGERROR("Not found in RecipefileXml : " << QMNUMBER << "using default = " << sQMNumber);
        }
        field.SetQMNumber(sQMNumber);
    }

    {
        float32_t setpoint = 0.0F;
        auto result = helper::GetNumber(*config, SETPOINT, setpoint);
        if (!result)
        {
            assert(FALSE);
            setpoint = 100.0F;
            LOGERROR("Not found in RecipefileXml : " << SETPOINT << "using default = " << setpoint);
        }
        field.SetLineSetpoint(setpoint);
    }
    auto units = config->FirstChildElement(UNITS);
    if (!units)
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " " << UNITS);
        doc.Clear();
        return FALSE;
    }
    for (auto doseelement = units->FirstChildElement(DOSE); doseelement != nullptr; doseelement = doseelement->NextSiblingElement())
    {
        field.AddItem(GetEntry(*doseelement));
    }
    doc.Clear();
    return TRUE;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRecipefileXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    const auto& field = Get();

    std::stringstream str;
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));

    auto el = doc.NewElement(NAME);
    el->SetText(field.GetName().c_str());
    config->InsertEndChild(el);

    el = doc.NewElement(QMNUMBER);
    el->SetText(field.GetQMNumber());
    config->InsertEndChild(el);

    el = doc.NewElement(SETPOINT);
    helper::SetFloatRound(*el, field.GetLineSetpoint());
    config->InsertEndChild(el);

    auto units = doc.NewElement(UNITS);
    config->InsertEndChild(units);
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




