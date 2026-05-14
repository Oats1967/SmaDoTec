//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LogoConfigXml
///
/// @file   LogoConfigXml.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cstring>
#include "BASE/types.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/LogoConfigXml.h"
#include "Logger/include/Logger.h"

STATIC_KEY(CONFIGURATION, "LogoConfig")
STATIC_KEY(HEADERVERSION, ("Version"))
STATIC_KEY(UNITS, "Units")
STATIC_KEY(DOSE, "Dose")
STATIC_KEY(ID, "ID")
STATIC_KEY(RELEASE, "Release")
STATIC_KEY(RUNMONITORING, "RunMonitoring")
STATIC_KEY(RUNMONITORINGWAITTIME, "RunMonitoringWaitTime")
STATIC_KEY(LOCKING, "Locking")
STATIC_KEY(INPUTSIGNAL, "InputSignal")
STATIC_KEY(RUNCONTROL, "RunControl")
STATIC_KEY(ENABLE, "Enable")
STATIC_KEY(SAMPLETIME, "SampleTime")
STATIC_KEY(INTERVALS, "Intervals")
STATIC_KEY(SECTION, "Section")
STATIC_KEY(STARTDRIVECOMMAND, "StartDriveCommand")
STATIC_KEY(ENDDRIVECOMMAND, "EndDriveCommand")
STATIC_KEY(SERVICEMODE, "ServiceMode")


const static int32_t c_HeaderVersion = 1;
static int32_t g_HeaderVersion = 0;

using namespace tinyxml2;

namespace base
{
namespace xml
{
//*********************************************************************************************************************
//*********************************************************************************************************************
void CLogoConfigXml::SetEntry(XMLDocument& doc, XMLNode& config, const int32_t index, const CLogoDoseConfig& cfg) const
{
    auto doseel = doc.NewElement(DOSE);
    config.InsertEndChild(doseel);

    std::string szComment = " +++ Dosing unit = " + std::to_string(index + 1) + " +++ ";
    doseel->InsertEndChild(doc.NewComment(szComment.c_str()));

    auto el = doc.NewElement(LOCKING);
    el->SetText(c_LogoLockingMap.get(cfg.GetLocking()).c_str());
    doseel->InsertEndChild(el);

    el = doc.NewElement(RELEASE);
    el->SetText(cfg.GetSimpleParser().GetOrgString().c_str());
    doseel->InsertEndChild(el);

    auto& rC = cfg.GetRunControl();
    auto runC = doc.NewElement(RUNCONTROL);

    el = doc.NewElement(ENABLE);
    el->SetText(c_YesNoMap.get(rC.GetEnable()).c_str());
    runC->InsertEndChild(el);
    if (rC.GetEnable())
    {
        el = doc.NewElement(INPUTSIGNAL);
        el->SetText(c_LogoInputSignalMap.get(rC.GetInputSignal()).c_str());
        runC->InsertEndChild(el);

        el = doc.NewElement(SAMPLETIME);
        el->SetText(rC.GetSampleTime());
        runC->InsertEndChild(el);

        auto count = rC.GetCount();
        if (count > 0)
        {
            auto intervals = doc.NewElement(INTERVALS);
            for (int32_t kIndex = 0; kIndex < _S32(count); kIndex++)
            {
                auto sector = doc.NewElement(SECTION);
                const auto& rItem = rC.Get(kIndex);

                el = doc.NewElement(STARTDRIVECOMMAND);
                el->SetText(rItem.m_StartDriveCommand);
                sector->InsertEndChild(el);

                el = doc.NewElement(ENDDRIVECOMMAND);
                el->SetText(rItem.m_EndDriveCommand);
                sector->InsertEndChild(el);

                el = doc.NewElement(SAMPLETIME);
                el->SetText(rItem.m_SampleTime);
                sector->InsertEndChild(el);

                intervals->InsertEndChild(sector);
            }
            runC->InsertEndChild(intervals);
        }
    }
    doseel->InsertEndChild(runC);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLogoConfigXml::LoadNewRunControl(const tinyxml2::XMLElement& runC, base::CRunControl& runControl) const
{
    {
        std::string sEnable;
        BOOL bEnable = FALSE;
        auto result = helper::GetString(runC, ENABLE, sEnable);
        if (!result)
        {
            assert(FALSE);
            LOGERROR("Not found : " << ENABLE << " using default = FALSE");
            bEnable = FALSE;
        }
        else
        {
            bEnable = c_YesNoMap.get(sEnable);
        }
        runControl.SetEnable(bEnable);
    }
    if (runControl.GetEnable())
    {
        {
            std::string sInputSignal;
            base::eLogoInputSignal inputSignal = base::eLogoInputSignal::eInputSignalNone;
            auto result = helper::GetString(runC, INPUTSIGNAL, sInputSignal);
            if (!result)
            {
                assert(FALSE);
                LOGERROR("Not found : " << INPUTSIGNAL << " using default = NotUsed");
                inputSignal = base::eLogoInputSignal::eInputSignalNone;
            }
            else
            {
                inputSignal = c_LogoInputSignalMap.get(sInputSignal);
            }
            runControl.SetInputSignal(inputSignal);
        }
        {
            int32_t sSampleTime = 0;
            auto result = helper::GetNumber(runC, SAMPLETIME, sSampleTime);
            if (!result)
            {
                assert(FALSE);
                LOGERROR("Not found : " << SAMPLETIME << " using default = 5");
                sSampleTime = 5;
            }
            else
            {
                sSampleTime = RANGE(sSampleTime, 0, 100);
            }
            runControl.SetSampleTime(_U32(sSampleTime));
        }
        auto intervals = runC.FirstChildElement(INTERVALS);
        if (intervals)
        {
            for (auto sector = intervals->FirstChildElement(SECTION); sector != nullptr; sector = sector->NextSiblingElement())
            {
                base::RunControlSection aSection;
                {
                    int32_t StartDriveCommand = 0;
                    auto result = helper::GetNumber(*sector, STARTDRIVECOMMAND, StartDriveCommand);
                    if (!result)
                    {
                        assert(FALSE);
                        LOGERROR("Not found : " << STARTDRIVECOMMAND << " using default = 0");
                        StartDriveCommand = 0;
                    }
                    else
                    {
                        StartDriveCommand = RANGE(StartDriveCommand, 0, 100);
                    }
                    aSection.m_StartDriveCommand = _U32(StartDriveCommand);
                }
                {
                    int32_t EndDriveCommand = 0;
                    auto result = helper::GetNumber(*sector, ENDDRIVECOMMAND, EndDriveCommand);
                    if (!result)
                    {
                        assert(FALSE);
                        LOGERROR("Not found : " << ENDDRIVECOMMAND << " using default = 0");
                        EndDriveCommand = 0;
                    }
                    else
                    {
                        EndDriveCommand = RANGE(EndDriveCommand, 0, 100);
                    }
                    aSection.m_EndDriveCommand = _U32(EndDriveCommand);
                }
                {
                    int32_t SampleTime = 0;
                    auto result = helper::GetNumber(*sector, SAMPLETIME, SampleTime);
                    if (!result)
                    {
                        assert(FALSE);
                        LOGERROR("Not found : " << SAMPLETIME << " using default = 5");
                        SampleTime = 5;
                    }
                    else
                    {
                        SampleTime = RANGE(SampleTime, 0, 100);
                    }
                    aSection.m_SampleTime = _U32(SampleTime);
                }
                runControl.Add(aSection);
            }
        }
    }
    return TRUE;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLogoConfigXml::LoadRunControl(const tinyxml2::XMLElement& doseelement, base::CRunControl& runControl) const
{
    // RunControl
    BOOL result = FALSE;
    auto runC = doseelement.FirstChildElement(RUNCONTROL);
    if (runC)
    {
        result = LoadNewRunControl(*runC, runControl);
    }
    else
    {
        assert(FALSE);
    }
    return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
base::CLogoDoseConfig CLogoConfigXml:: GetEntry(const tinyxml2::XMLElement& doseelement) const
{
    CLogoDoseConfig cfg;

    if (m_SaveLocking != eLogoLocking::eLockingNone)
    {
        cfg.SetLocking(m_SaveLocking);
    }
    else
    {
        std::string sLocking;
        base::eLogoLocking locking = base::eLogoLocking::eLockingNone;
        auto result = helper::GetString(doseelement, LOCKING, sLocking);
        if (!result)
        {
            LOGERROR("Not found : " << LOCKING << " using default = NotUsed");
            locking = base::eLogoLocking::eLockingNone;
        }
        else
        {
            locking = c_LogoLockingMap.get(sLocking);
        }
        cfg.SetLocking(locking);
    }
    {
        std::string     sReleaseString;
        auto result = helper::GetString(doseelement, RELEASE, sReleaseString);
        if (! result)
        {
            assert(FALSE);
            sReleaseString = "DIO";
            LOGERROR("Not found : " << RELEASE << " using default = DI0");
        }
        auto& rSimpleParser = cfg.GetSimpleParser();
        rSimpleParser.SetOrgString(sReleaseString);
    }
    LoadRunControl(doseelement, cfg.GetRunControl());
    return cfg;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLogoConfigXml::Load(const std::string& szfilename)
{
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
    {
        auto result = base::xml::helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
        if (!result)
        {
            LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
            g_HeaderVersion = -1;
        }
    }

    {
        std::string sLocking;
        m_SaveLocking = eLogoLocking::eLockingNone;
        auto result = helper::GetString(*config, LOCKING, sLocking);
        if (result)
        {
            m_SaveLocking = c_LogoLockingMap.get(sLocking);
        }
    }

    auto& field = Get();
    {
        std::string     sReleaseString;
        auto result = helper::GetString(*config, SERVICEMODE, sReleaseString);
        if (!result)
        {
            assert(FALSE);
            LOGERROR("Not found : " << SERVICEMODE << " using default = SU");
            sReleaseString = "SU";
            g_HeaderVersion = -1;
        }
        auto& rSimpleParser = field.GetServiceMode();
        rSimpleParser.SetOrgString(sReleaseString);
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
    for  (auto doseelement = units->FirstChildElement(DOSE); doseelement != nullptr; doseelement = doseelement->NextSiblingElement(), index++)
    {
        field.Set(index, GetEntry(*doseelement));
    }
    doc.Clear();
    if ((m_SaveLocking != eLogoLocking::eLockingNone) ||
        ( g_HeaderVersion != c_HeaderVersion ))
    {
        Save(szfilename);
    }
    return TRUE;
}

//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CLogoConfigXml::Save(const std::string& szfilename)
{
    XMLDocument doc;

    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    assert(config);
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    assert(header);
    header->ToElement()->SetText(c_HeaderVersion);

    const char* szHeader = {
#include "LogoConfigHeader.def"
    };
    config->InsertEndChild(doc.NewComment(szHeader));

    const auto& field = Get();
    auto sv = config->InsertEndChild(doc.NewElement(SERVICEMODE));
    assert(sv);
    sv->ToElement()->SetText(field.GetServiceMode().GetOrgString().c_str());

    auto units = config->InsertEndChild(doc.NewElement(UNITS));
    assert(units);
    for (uint32_t index = 0; index < field.GetCount(); index++)
    {
        SetEntry(doc, *units, index, field.Get(index) );
    }
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




