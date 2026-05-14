//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SystemBackupXml
///
/// @file   SystemBackupXml.cpp
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
#include "BASE/Utils/public/StringUtil.h"
#include "BASE/utils/private/xml/HelperXml.h"
#include "BASE/Utils/public/xml/SystemBackupXml.h"
#include "Logger/include/Logger.h"


#define STR_(X) #X
#define STR(X) STR_(X)
#define STATIC_K(_a) static const char_t* _a = STR(_a);

STATIC_KEY(CONFIGURATION,"SystemBackup");
STATIC_KEY(HEADERVERSION, "Version")
STATIC_KEY(UNITS, "Units");
STATIC_KEY(DOSE, "Dose");
STATIC_KEY(LINE, "Line");


STATIC_K(RecipeName)
STATIC_K(ANNumber)
STATIC_K(RecipeSetpoint)
STATIC_K(RegeneratPercentage)
STATIC_K(Setpoint)
STATIC_K(Hysteresis)
STATIC_K(MinTotband)
STATIC_K(FilterTime)
STATIC_K(ExtSetpointScale)
STATIC_K(ExtSetpointOffset)
STATIC_K(RampStep);
STATIC_K(RampDelay);
STATIC_K(TotalizerPulseStep);
STATIC_K(TotalizerPulseDuration);


STATIC_K(Name);						// Name
STATIC_K(QMNumber)  			// QM-Nummer
STATIC_K(ID)  			// QM-Nummer
STATIC_K(FeedingType);
STATIC_K(DoseType);
STATIC_K(Percentage);			// Nomineller Prozentwert
STATIC_K(NominalSetpoint);		// Nomineller Sollwert in kg/h
STATIC_K(MaxSetpoint);			// Max. Durchsatz fuer Anzeige
STATIC_K(MaxRotationalSpeed);		// Maximaldrehzahl
STATIC_K(MassflowFilter);			// Istwertfilter
STATIC_K(RefillLimitMin);			// Min-Wert
STATIC_K(RefillLimitMax);			// Maximal  zulaessiges Gewicht
STATIC_K(RefillLimitMinMin);		// Untere Refill Begrenzung
STATIC_K(RefillTime);				// Max. Befuellzeit
STATIC_K(RefillSwitchDelay);	    // Umschaltverzoegerung
STATIC_K(RefillDebounceMax);				// RefillDebounceMax
STATIC_K(RefillDebounceMin);				// RefillDebounceMin
STATIC_K(RefillFeederEmptyStart);		// MinDurationActive

STATIC_K(NominalAgitator);		    // Rührwerk Soll-DriveCommand
STATIC_K(RefillFeederNominalSpeed);	    // RefillFeeder
STATIC_K(MinSetpointChange);		// Mindest Sollwertänderung
STATIC_K(StartupDelay);			// StartupDelay
STATIC_K(StartupRamp);			// StartupRamp
STATIC_K(LCCorrectionFactor);		// Korrekturwert Waage
STATIC_K(LCTaraWeight);			// Taraweight
STATIC_K(EncoderMonitor);			// RotationalSpeedueberwachung 0 ... 1000
STATIC_K(AlarmNoiseLimit);		//Waagenstoergrenze Alarmgrenze
STATIC_K(AlarmReactionDelay);
STATIC_K(AlarmStartReactionDelay);
STATIC_K(AlarmMassflowLow);
STATIC_K(AlarmMassflowHigh);
STATIC_K(AlarmDriveCommandHigh);
STATIC_K(AlarmDriveCommandLow);
STATIC_K(AlarmDosePerformance);
STATIC_K(AlarmMaxBatchTime);
STATIC_K(MaxDriveCommandChange);
STATIC_K(Regenerat);
STATIC_K(WbfBeltLoadSetpoint);		// BeltLoadSetpoint in kg/m
STATIC_K(WbfBeltLoadVolSwitch);				// Umschaltgrenze f. volumetrisch
STATIC_K(WbfReduction);				// Getriebeuntersetzung
STATIC_K(WbfWeighingLine);			// Laenge des Wiegebereichs
STATIC_K(WbfWheelSize);				// WheelSize
STATIC_K(WbfTareDriveCommand);		// DriveCommand fuer Tarierung
STATIC_K(WbfTareMeasurementTime);		// Messzeit fuer Tarierung
STATIC_K(WbfAlarmMinBeltLoad);		// Alarm minimale Bandlast
STATIC_K(WbfAlarmMaxBeltLoad);		// Alarm maximale Bandlast
STATIC_K(WbfMinDriveCommand);			// minimum drive command
STATIC_K(WbfWeighingFull);
STATIC_K(PidPropGainGross);			// Prop. Konstante
STATIC_K(PidSampleInterval);			// PID-SampleInterval
STATIC_K(PidPropGainFine);			// Feinverstaerkung
STATIC_K(PidPropGainSwitchGrossFine);		// Umschalten grob auf fein
STATIC_K(PidIntegralGain);
STATIC_K(PidGatefilter);
STATIC_K(PidDriveCommandInv);
STATIC_K(IfsReduceFactor);
STATIC_K(IfsGainFactor);
STATIC_K(IfsFeederOverflowTimeOut);
STATIC_K(IfsFeederEmptyTimeOut);
STATIC_K(IfsStepTimeGain);
STATIC_K(IfsStepTimeReduce);
STATIC_K(IfsSetpointOverflow);
STATIC_K(IfsDebounceMax);
STATIC_K(IfsDebounceMin);

STATIC_K(SteepnessMassflow);
STATIC_K(EmptyFeederRuntime);
STATIC_K(EmptyFeederSpeed);
STATIC_K(EmptyFeederRestriction);
STATIC_K(CalibDriveCommand);
STATIC_K(CalibDosePerformance);
STATIC_K(CalibMeasureTime);


const static int32_t c_HeaderVersion = 1;
static int32_t g_HeaderVersion = 0;


using namespace tinyxml2;

namespace base
{
namespace xml
{

#define SETKEY(doseel, _a) \
{ \
    auto el = doc.NewElement(_a); \
    el->SetText(cfg._a); \
    doseel->InsertEndChild(el); \
}

#define SETKEY_STRING(doseel, _a) \
{ \
    auto el = doc.NewElement(_a); \
    el->SetText(cfg._a.c_str()); \
    doseel->InsertEndChild(el); \
}
#define SETKEY_MAP(doseel, m, _a) \
{ \
    auto el = doc.NewElement(_a); \
    el->SetText(m.get(cfg._a).c_str()); \
    doseel->InsertEndChild(el); \
}

#define GETNUMBER(doseel, _a) \
{ \
    auto result = helper::GetNumber(doseel, _a, cfg._a); \
    if (!result)\
    { \
       assert(FALSE); \
       LOGERROR("Not found in SystemBackupXml : " << _a); \
    } \
}

#define GETNUMBER_MAP(doseel, _m, _a) \
{ \
    std::string sTemp; \
    auto result = helper::GetString(doseel, _a, sTemp); \
    if (result) \
    { \
        cfg._a = _m.get(sTemp); \
    } \
    else \
    {\
        assert(FALSE); \
        LOGERROR("Not found in SystemBackupXml : " << _a); \
    } \
}


#define GETSTRING(doseel, _a) \
{ \
    auto result = helper::GetString(doseel, _a, cfg._a); \
    if (!result) \
    { \
        assert(FALSE); \
        LOGERROR("Not found in SystemBackupXml : " << _a); \
    }\
}

//*********************************************************************************************************************
//*********************************************************************************************************************
void CSystemBackupXml::SetEntry(const base::CLineBackupConfig& cfg, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config) const
{
    auto linel = doc.NewElement(LINE);
    config.InsertEndChild(linel);
    SETKEY_STRING(linel, RecipeName);
    SETKEY_STRING(linel, ANNumber);
    SETKEY(linel, MaxSetpoint);
    SETKEY(linel, QMNumber);
    SETKEY(linel, RecipeSetpoint);
    SETKEY(linel, RegeneratPercentage);
    SETKEY(linel, Setpoint);
    SETKEY(linel, Percentage);
    SETKEY(linel, Hysteresis);
    SETKEY(linel, MinTotband);
    SETKEY(linel, FilterTime);
    SETKEY(linel, ExtSetpointScale);
    SETKEY(linel, ExtSetpointOffset);
    SETKEY(linel, RampStep);
    SETKEY(linel, RampDelay);
    SETKEY(linel, TotalizerPulseStep);
    SETKEY(linel, TotalizerPulseDuration);

}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CSystemBackupXml::SetEntry(const int32_t , const base::DoseBackupConfig& cfg, tinyxml2::XMLDocument& doc, tinyxml2::XMLNode& config) const
{
    auto doseel = doc.NewElement(DOSE);
    config.InsertEndChild(doseel);
    SETKEY_STRING(doseel, Name);
    SETKEY(doseel, ID);
    SETKEY_MAP(doseel, c_DoseTypeMap, DoseType);
    SETKEY(doseel, QMNumber);
    SETKEY_MAP(doseel, c_FeedingTypeMap, FeedingType);
    SETKEY(doseel, Percentage);
    SETKEY(doseel, NominalSetpoint);
    SETKEY(doseel, MaxSetpoint);
    SETKEY(doseel, MaxRotationalSpeed);
    SETKEY(doseel, MassflowFilter);
    SETKEY(doseel, RefillLimitMin);
    SETKEY(doseel, RefillLimitMax);
    SETKEY(doseel, RefillLimitMinMin);
    SETKEY(doseel, RefillTime);
    SETKEY(doseel, RefillSwitchDelay);
    SETKEY(doseel, RefillDebounceMax);
    SETKEY(doseel, RefillDebounceMin);
    SETKEY(doseel, RefillFeederEmptyStart);
    SETKEY(doseel, NominalAgitator);
    SETKEY(doseel, RefillFeederNominalSpeed);
    SETKEY(doseel, MinSetpointChange);

    SETKEY(doseel, StartupDelay);
    SETKEY(doseel, StartupRamp);
    if (__ISLOADCELLTYPE(cfg.DoseType))
    {
        SETKEY(doseel, LCCorrectionFactor);
        SETKEY(doseel, LCTaraWeight);
    }
    SETKEY(doseel, EncoderMonitor);
    SETKEY(doseel, AlarmNoiseLimit);
    SETKEY(doseel, AlarmReactionDelay);
    SETKEY(doseel, AlarmStartReactionDelay);
    SETKEY(doseel, AlarmMassflowLow);
    SETKEY(doseel, AlarmMassflowHigh);
    SETKEY(doseel, AlarmDriveCommandHigh);
    SETKEY(doseel, AlarmDriveCommandLow);
    SETKEY(doseel, AlarmDosePerformance);
    SETKEY(doseel, AlarmMaxBatchTime);
    SETKEY(doseel, MaxDriveCommandChange);
    SETKEY(doseel, Regenerat);
    if (__ISBELTWEIGHERTYPE(cfg.DoseType))
    {
        SETKEY(doseel, WbfBeltLoadSetpoint);		// BeltLoadSetpoint in kg/m
        SETKEY(doseel, WbfBeltLoadVolSwitch);				// Umschaltgrenze f. volumetrisch
        SETKEY(doseel, WbfReduction);				// Getriebeuntersetzung
        SETKEY(doseel, WbfWeighingLine);			// Laenge des Wiegebereichs
        SETKEY(doseel, WbfWheelSize);				// WheelSize
        SETKEY(doseel, WbfTareDriveCommand);		// DriveCommand fuer Tarierung
        SETKEY(doseel, WbfTareMeasurementTime);		// Messzeit fuer Tarierung
        SETKEY(doseel, WbfAlarmMinBeltLoad);		// Alarm minimale Bandlast
        SETKEY(doseel, WbfAlarmMaxBeltLoad);		// Alarm maximale Bandlast
        SETKEY(doseel, WbfMinDriveCommand);			// minimum drive command
        SETKEY(doseel, WbfWeighingFull);
    }
    SETKEY(doseel, PidPropGainGross);			// Prop. Konstante
    SETKEY(doseel, PidSampleInterval);			// PID-SampleInterval
    SETKEY(doseel, PidPropGainFine);			// Feinverstaerkung
    SETKEY(doseel, PidPropGainSwitchGrossFine);		// Umschalten grob auf fein
    SETKEY(doseel, PidIntegralGain);
    SETKEY(doseel, PidGatefilter);
    SETKEY(doseel, PidDriveCommandInv);

    if (__ISIFSTYPE(cfg.DoseType))
    {
        SETKEY(doseel, IfsReduceFactor);
        SETKEY(doseel, IfsGainFactor);
        SETKEY(doseel, IfsFeederOverflowTimeOut);
        SETKEY(doseel, IfsFeederEmptyTimeOut);
        SETKEY(doseel, IfsStepTimeGain);
        SETKEY(doseel, IfsStepTimeReduce);
        SETKEY(doseel, IfsSetpointOverflow);
        SETKEY(doseel, IfsDebounceMax);
        SETKEY(doseel, IfsDebounceMin);
    }

    SETKEY(doseel, SteepnessMassflow);
    SETKEY(doseel, EmptyFeederRuntime);
    SETKEY(doseel, EmptyFeederSpeed);

    // Calibration items
    { 
        auto el = doc.NewElement(CalibDriveCommand);
        el->SetText(cfg.calibType.fDriveCommand[0]);
        doseel->InsertEndChild(el);
    }
    {
        auto el = doc.NewElement(CalibDosePerformance);
        el->SetText(cfg.calibType.fDosePerformance[0]);
        doseel->InsertEndChild(el);
    }
    {
        auto el = doc.NewElement(CalibMeasureTime);
        el->SetText(cfg.calibType.lMeasureTime[0]);
        doseel->InsertEndChild(el);
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************
 void CSystemBackupXml::GetEntry(const tinyxml2::XMLElement& lineel, base::CLineBackupConfig& cfg) const
{
     GETSTRING(lineel, RecipeName);
     GETSTRING(lineel, ANNumber);
     GETNUMBER(lineel, MaxSetpoint);
     GETNUMBER(lineel, QMNumber);
     GETNUMBER(lineel, RecipeSetpoint);
     GETNUMBER(lineel, RegeneratPercentage);
     GETNUMBER(lineel, Setpoint);
     GETNUMBER(lineel, Percentage);
     GETNUMBER(lineel, Hysteresis);
     GETNUMBER(lineel, MinTotband);
     GETNUMBER(lineel, FilterTime);
     GETNUMBER(lineel, ExtSetpointScale);
     GETNUMBER(lineel, ExtSetpointOffset);
     GETNUMBER(lineel, RampStep);
     GETNUMBER(lineel, RampDelay);
     GETNUMBER(lineel, TotalizerPulseStep);
     GETNUMBER(lineel, TotalizerPulseDuration);
 }

//*********************************************************************************************************************
//*********************************************************************************************************************
void CSystemBackupXml::GetEntry(const tinyxml2::XMLElement& doseelement, base::DoseBackupConfig& cfg) const
{
    GETSTRING(doseelement, Name);
    GETNUMBER(doseelement, ID);
    GETNUMBER_MAP(doseelement, c_DoseTypeMap, DoseType);
    GETNUMBER(doseelement, QMNumber);
    GETNUMBER_MAP(doseelement, c_FeedingTypeMap, FeedingType);
    GETNUMBER(doseelement, Percentage);
    GETNUMBER(doseelement, NominalSetpoint);
    GETNUMBER(doseelement, MaxSetpoint);
    GETNUMBER(doseelement, MaxRotationalSpeed);
    GETNUMBER(doseelement, MassflowFilter);
    GETNUMBER(doseelement, RefillLimitMin);
    GETNUMBER(doseelement, RefillLimitMax);
    GETNUMBER(doseelement, RefillLimitMinMin);
    GETNUMBER(doseelement, RefillTime);
    GETNUMBER(doseelement, RefillSwitchDelay);	    // Umschaltverzoegerung
    GETNUMBER(doseelement, RefillDebounceMax);
    GETNUMBER(doseelement, RefillDebounceMin);
    GETNUMBER(doseelement, RefillFeederEmptyStart);
    GETNUMBER(doseelement, NominalAgitator);
    GETNUMBER(doseelement, MinSetpointChange);
    GETNUMBER(doseelement, StartupDelay);			// StartupDelay
    GETNUMBER(doseelement, StartupRamp);			// StartupRamp
    if (__ISLOADCELLTYPE(cfg.DoseType))
    {
        GETNUMBER(doseelement, LCCorrectionFactor);		// Korrekturwert Waage
        GETNUMBER(doseelement, LCTaraWeight);			// Taraweight
    }
    GETNUMBER(doseelement, EncoderMonitor);			// RotationalSpeedueberwachung 0 ... 1000
    GETNUMBER(doseelement, AlarmNoiseLimit);		//Waagenstoergrenze Alarmgrenze
    GETNUMBER(doseelement, AlarmReactionDelay);
    GETNUMBER(doseelement, AlarmStartReactionDelay);
    GETNUMBER(doseelement, AlarmMassflowLow);
    GETNUMBER(doseelement, AlarmMassflowHigh);
    GETNUMBER(doseelement, AlarmDriveCommandHigh);
    GETNUMBER(doseelement, AlarmDriveCommandLow);
    GETNUMBER(doseelement, AlarmDosePerformance);
    GETNUMBER(doseelement, AlarmMaxBatchTime);
    GETNUMBER(doseelement, MaxDriveCommandChange);
    GETNUMBER(doseelement, Regenerat);
    if (__ISBELTWEIGHERTYPE(cfg.DoseType))
    {
        GETNUMBER(doseelement, WbfBeltLoadSetpoint);		// BeltLoadSetpoint in kg/m
        GETNUMBER(doseelement, WbfBeltLoadVolSwitch);				// Umschaltgrenze f. volumetrisch
        GETNUMBER(doseelement, WbfReduction);				// Getriebeuntersetzung
        GETNUMBER(doseelement, WbfWeighingLine);			// Laenge des Wiegebereichs
        GETNUMBER(doseelement, WbfWheelSize);				// WheelSize
        GETNUMBER(doseelement, WbfTareDriveCommand);		// DriveCommand fuer Tarierung
        GETNUMBER(doseelement, WbfTareMeasurementTime);		// Messzeit fuer Tarierung
        GETNUMBER(doseelement, WbfAlarmMinBeltLoad);		// Alarm minimale Bandlast
        GETNUMBER(doseelement, WbfAlarmMaxBeltLoad);		// Alarm maximale Bandlast
        GETNUMBER(doseelement, WbfMinDriveCommand);			// minimum drive command
        GETNUMBER(doseelement, WbfWeighingFull);
    }
    GETNUMBER(doseelement, PidPropGainGross);			// Prop. Konstante
    GETNUMBER(doseelement, PidSampleInterval);			// PID-SampleInterval
    GETNUMBER(doseelement, PidPropGainFine);			// Feinverstaerkung
    GETNUMBER(doseelement, PidPropGainSwitchGrossFine);		// Umschalten grob auf fein
    GETNUMBER(doseelement, PidIntegralGain);
    GETNUMBER(doseelement, PidGatefilter);
    GETNUMBER(doseelement, PidDriveCommandInv);


    if (__ISIFSTYPE(cfg.DoseType))
    {
        GETNUMBER(doseelement, IfsReduceFactor);
        GETNUMBER(doseelement, IfsGainFactor);
        GETNUMBER(doseelement, IfsFeederOverflowTimeOut);
        GETNUMBER(doseelement, IfsFeederEmptyTimeOut);
        GETNUMBER(doseelement, IfsStepTimeGain);
        GETNUMBER(doseelement, IfsStepTimeReduce);
        GETNUMBER(doseelement, IfsSetpointOverflow);
        GETNUMBER(doseelement, IfsDebounceMax);
        GETNUMBER(doseelement, IfsDebounceMin);
    }


    GETNUMBER(doseelement, SteepnessMassflow);
    GETNUMBER(doseelement, EmptyFeederRuntime);
    GETNUMBER(doseelement, EmptyFeederSpeed);


    // Calibration items
    {
        auto result = helper::GetNumber(doseelement, CalibDriveCommand, cfg.calibType.fDriveCommand[0]);
        if ( ! result)
        {
            assert(FALSE);
            LOGERROR("Not found in SystemBackupXml : " << CalibDriveCommand);
        }
    }
    {
        auto result = helper::GetNumber(doseelement, CalibDosePerformance, cfg.calibType.fDosePerformance[0]);
        if ( ! result)
        {
            assert(FALSE);
            LOGERROR("Not found in SystemBackupXml : " << CalibDosePerformance);
        }
    }
    {
        auto result = helper::GetNumber(doseelement, CalibMeasureTime, cfg.calibType.lMeasureTime[0]);
        if (!result)
        {
            assert(FALSE);
            LOGERROR("Not found in SystemBackupXml : " << CalibMeasureTime);
        }
        cfg.calibType.count = 1;
    }
}


//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CSystemBackupXml::Load(const std::string& szfilename)
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
    auto result = base::xml::helper::GetNumber(*config, HEADERVERSION, g_HeaderVersion);
    if (!result)
    {
        LOGERROR("Error in Xml-file: " << szfilename << " " << HEADERVERSION);
        g_HeaderVersion = -1;
    }
    auto& rSystem = Get();
    auto line = config->FirstChildElement(LINE);
    if (line)
    {
        GetEntry(*line, rSystem.m_Line);
    }
    auto units = config->FirstChildElement(UNITS);
    if (!units)
    {
        assert(FALSE);
        LOGERROR("Error in Xml-file: " << szfilename << " " << UNITS);
        doc.Clear();
        return FALSE;
    }
    auto& field = rSystem.m_Feeder;
    field.clear();
    for  (auto doseelement = units->FirstChildElement(DOSE); doseelement != nullptr; doseelement = doseelement->NextSiblingElement())
    {
        base::DoseBackupConfig cfg;
        GetEntry(*doseelement, cfg);
        field.push_back(cfg);
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
BOOL CSystemBackupXml::Save(const std::string& szfilename)
{
    XMLDocument doc;
    doc.InsertFirstChild(doc.NewDeclaration());
    auto config = doc.InsertEndChild(doc.NewElement(CONFIGURATION));
    auto header = config->InsertEndChild(doc.NewElement(HEADERVERSION));
    header->ToElement()->SetText(c_HeaderVersion);

    const auto& rSystem = Get();
    SetEntry(rSystem.m_Line, doc, *config);
    auto units = config->InsertEndChild(doc.NewElement(UNITS));
    const auto& field = rSystem.m_Feeder;
    for (uint32_t index = 0; index < _U32(field.size()); index++)
    {
        SetEntry(index, field[index], doc, *units);
    }
    auto result = doc.SaveFile(szfilename.c_str());
    doc.Clear();
    return (result == XML_SUCCESS);
};

};
};




