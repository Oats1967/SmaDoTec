#include <memory>
#include <string>
#include "BASE/Utils/public/Memory.h"
#include "BASE/Utils/public/LogItemList.h"
#include "BASE/Utils/public/RecItemList.h"
#include "BASE/Utils/public/UserRightsList.h"
#include "BASE/Utils/public/RecipeItemList.h"
#include "RemoteServiceS7.h"
#include "DoseServer.h"
#include "BASE/Task/public/Task.h"
#include "BASE/include/AlarmField.h"
#include "BASE/Utils/public/xml/SystemAlarmXml.h"
#include "BASE/Utils/public/xml/SystemConfigXml.h"
#include "BASE/Utils/public/xml/LogoConfigXml.h"
#include "BASE/Utils/public/xml/EasyControlConfigXml.h"
#include "BASE/Utils/public/xml/TouchkeyboardXml.h"
#include "BASE/Utils/public/xml/TrendGraphicXml.h"
#include "BASE/Utils/public/xml/RemoteServerXml.h"
#include "BASE/Utils/public/xml/DoseServerXml.h"
#include "BASE/Utils/public/xml/OpenModbusXml.h"
#include "BASE/Utils/public/xml/ProfibusTableXml.h"
#include "BASE/Utils/public/xml/DBTableXml.h"
#include "BASE/Utils/public/xml/S7DaveXml.h"
#include "BASE/Utils/public/xml/S7SnapXml.h"
#include "BASE/BoolParser/SimpleParser.h"


#define DEFAULTSZCONFIGPATH "C://DEVELOP//Cplusplus//EasyTouch//S7RemoteConfig.xml"
#define DEFAULTSERVERCONFIGPATH "C://DEVELOP//Cplusplus//EasyTouch//DoseServer//X64//Debug"


int testRecipelist()
{
    const std::string myrecipe("C:\\temp\\test\\MyRecipe.rzp");

    base::utils::CRecipeItemList aList;

    std::string szLogPath = "C://temp//test";
    aList.SetPath(szLogPath);

    base::utils::CRecipeItem A;
    base::utils::CRecipeItem B;

    aList.AddItem(A);
    aList.AddItem(B);
    aList.Save(myrecipe);
    aList.Load(myrecipe);
    return 0;
}


int testloglist()
{
    base::utils::CLogItemList aList;

    std::string szLogPath = "C://temp//test";
    aList.SetPath(szLogPath);

    base::utils::CLogItem A;
    base::utils::CLogItem B;

    aList.AddItem(A);
    //aList.SaveLastItem();
    aList.AddItem(B);
    //aList.SaveLastItem();
    aList.Save();
    aList.AddItem(B);
    aList.AddItem(A);
    aList.SaveLastItem();

    aList.Load();
    return 0;
}


int testUserlist()
{
    const std::string filename("C:\\temp\\test\\MyUser.usr");

    base::utils::CUserRightsList aList;

    std::string szLogPath = "C://DEVELOP//Cplusplus//EasyTouch//remote//config";
    aList.SetPath(szLogPath);
    aList.SetFilename("UserConfig.usr");
    aList.Create();
    aList.Save();
    //aList.Load(filename);
    return 0;
}


int testReclist()
{
    base::utils::CRecItemList aList;

    std::string szLogPath = "C://temp//test";
    aList.SetPath(szLogPath);
    aList.SetMaxItems(4);

    base::utils::CRecItem A;
    for (int32_t i = 0; i < 4; i++)
    {
#if 0
        A.Set(.SetDosierungRotationalSpeed(i, 10*i + 2.0F);
        A.SetDosierungGewicht(i, 10 * i + 3.0F);
        A.SetDosierungMassflow(i, 10 * i + 4.0F);
        A.SetDosierungQMaxFaktor(i, 10 * i + 5.0F);
        A.SetDosierungSollwert(i, 10 * i + 6.0F);
        A.SetDosierungStellbefehl(i, 10 * i + 7.0F);
        A.SetDosierungVerbrauch(i, 10 * i + 8.0F);
#endif
    }
    base::utils::CRecItem B;
    for (int32_t i = 0; i < 4; i++)
    {
#if 0
        B.SetDosierungRotationalSpeed(i, 100 * i + 2.0F);
        B.SetDosierungGewicht(i, 100 * i + 3.0F);
        B.SetDosierungMassflow(i, 100 * i + 4.0F);
        B.SetDosierungQMaxFaktor(i, 100 * i + 5.0F);
        B.SetDosierungSollwert(i, 100 * i + 6.0F);
        B.SetDosierungStellbefehl(i, 100 * i + 7.0F);
        B.SetDosierungVerbrauch(i, 100 * i + 8.0F);
#endif
    }
    aList.AddItem(A);
    //aList.SaveLastItem();
    //aList.AddItem(B);
    aList.Save();
    aList.Load();
    return 0;
}
//******************************************************************************************
//******************************************************************************************
void testDoseServer()
{
    static const std::string szConfigPath = "C:/DEVELOP/Cplusplus/EasyTouch/remote/config/EasyControlConfig.xml";

    IDoseServerInterface* pInterface;

    base::xml::CEasyControlConfigXml xml;
    xml.Load(szConfigPath);
    GetDoseServerInterface(&pInterface);

    pInterface->Open(xml.Get().m_DoseServerFile);
    pInterface->Start();

    getchar();
    //base::task::Sleep(5000);
    pInterface->Close();
}



//******************************************************************************************
//******************************************************************************************
void testSystemAlarmXml()
{
    static const std::string szPath = "C:/DEVELOP/Cplusplus/EasyTouch/remote/config/SystemAlarm.xml";
    static const std::string szPathNew = "C:/temp/test/SystemAlarm.xml";

    base::xml::CSystemAlarmXml xml;
    xml.Load(szPath);
/*
    for (int32_t i = 0; i < _S32(base::eAlarmError::ERROR_DOSE_MAX); i++)
    {
        aField.Set(static_cast<base::eAlarmError>(i), base::eAlarmCategory::category0, TRUE);
        aField.Set(static_cast<base::eAlarmError>(i), base::eAlarmCategory::category1, TRUE);
        aField.Set(static_cast<base::eAlarmError>(i), base::eAlarmCategory::category2, TRUE);
        aField.Set(static_cast<base::eAlarmError>(i), base::eAlarmCategory::category3, TRUE);
    }
*/
    xml.Save(szPathNew);
}
//******************************************************************************************
//******************************************************************************************
void testSystemConfigXml()
{
    static const std::string szConfigPath = "C:/DEVELOP/Cplusplus/EasyTouch/remote/config/EasyControlConfig.xml";
    static const std::string szPathNew = "C:/temp/test/SystemConfig.xml";

    base::xml::CEasyControlConfigXml xmlconfig;
    xmlconfig.Load(szConfigPath);

    base::xml::CSystemConfigXml xml;
    xml.Load(xmlconfig.Get().m_SystemConfigFile);
    xml.Save(szPathNew);
}
//******************************************************************************************
//******************************************************************************************
void testRemoteServerXml()
{
    static const std::string szConfigPath = "C:/DEVELOP/Cplusplus/EasyTouch/remote/config/EasyControlConfig.xml";
    static const std::string szRemotePathDest = "C:/temp/test/RemoteServer.xml";

    base::xml::CEasyControlConfigXml xmlconfig;
    xmlconfig.Load(szConfigPath);

    base::xml::CDoseServerXml xmlDose;
    xmlDose.Load(xmlconfig.Get().m_DoseServerFile);

    base::xml::CRemoteServerXml xmlRemote;
    xmlRemote.Load(xmlDose.Get().m_path2RemoteServer);
    xmlRemote.Save(szRemotePathDest);
}
//******************************************************************************************
//******************************************************************************************
void testDoseServerXml()
{
    static const std::string szConfigPath = "C:/DEVELOP/Cplusplus/EasyTouch/remote/config/EasyControlConfig.xml";
    static const std::string szDosePathDest = "C:/temp/test/DoseServer.xml";

    base::xml::CEasyControlConfigXml xmlconfig;
    xmlconfig.Load(szConfigPath);

    base::xml::CDoseServerXml xml;
    xml.Load(xmlconfig.Get().m_DoseServerFile);
    xml.Save(szDosePathDest);
}
//******************************************************************************************
//******************************************************************************************
void testTouchkeyboardConfigXml()
{
    static const std::string szPath = "C:/DEVELOP/Cplusplus/EasyTouch/remote/config/EasyControlConfig.xml";
    static const std::string szPathNew = "C:/temp/test/EasyControlConfig.xml";
    static const std::string szTKPathNew = "C:/temp/test/Touchkeyboard.xml";

    base::xml::CEasyControlConfigXml xml;
    xml.Load(szPath);
//    xml.Save(szPathNew);
    base::xml::CTouchkeyboardXml tkxml;
    tkxml.Load(xml.Get().m_TouckKeyboardConfigFile);
    tkxml.Save(szTKPathNew);
//    xml.Save(szPathNew);
}
//******************************************************************************************
//******************************************************************************************
void testTrendGraphicXml()
{
    static const std::string szPath = "C:/DEVELOP/Cplusplus/EasyTouch/remote/config/EasyControlConfig.xml";
    static const std::string szPathNew = "C:/temp/test/EasyControlConfig.xml";
    static const std::string szTKPathNew = "C:/temp/test/Touchkeyboard.xml";

    base::xml::CEasyControlConfigXml xml;

    xml.Load(szPath);
    base::xml::CTrendGraphicXml tkxml;
    tkxml.Load(xml.Get().m_TrendGraphicFile);
    tkxml.Save(szTKPathNew);
}
//******************************************************************************************
//******************************************************************************************
void testTableProfibusXml()
{
    base::xml::CEasyControlConfigXml xml;

    static const std::string szConfigPath = "C:/DEVELOP/Cplusplus/EasyTouch/remote/config/TableProfibus.xml";
    static const std::string szRemotePathDest = "C:/temp/test/TableProfibus.xml";

    base::xml::CProfibusTableXml xmlconfig;
    xmlconfig.Load(szConfigPath);

    xmlconfig.Save(szRemotePathDest);
}
//******************************************************************************************
//******************************************************************************************
void testDBTableXml()
{
    static const std::string szConfigPath = "C:/DEVELOP/Cplusplus/EasyTouch/remote/config/TableS7Leistritz.xml";
    static const std::string szRemotePathDest = "C:/temp/test/TableS7Leistritz.xml";

    base::xml::CDBTableXml xmlconfig;
    xmlconfig.Load(szConfigPath);

    xmlconfig.Save(szRemotePathDest);
}
//******************************************************************************************
//******************************************************************************************
void testS7SnapXml()
{
    static const std::string szConfigPath = "C:/DEVELOP/Cplusplus/EasyTouch/remote/config/DeviceS7300Snap.xml";
    static const std::string szRemotePathDest = "C:/temp/test/DeviceS7300Snap.xml";

    base::xml::CS7SnapXml xmlconfig;
    xmlconfig.Load(szConfigPath);

    xmlconfig.Save(szRemotePathDest);
}
//******************************************************************************************
//******************************************************************************************
void testS7DaveXml()
{
    static const std::string szConfigPath = "C:/DEVELOP/Cplusplus/EasyTouch/remote/config/DeviceS7300Dave.xml";
    static const std::string szRemotePathDest = "C:/temp/test/DeviceS7300Dave.xml";

    base::xml::CS7DaveXml xmlconfig;
    xmlconfig.Load(szConfigPath);

    xmlconfig.Save(szRemotePathDest);
}

void testParser()
{
    static const std::string szEasyControlConfigXml = "C:/DEVELOP/Cplusplus/EasyControl/remote/config/EasyControl.xml";

    base::xml::CEasyControlConfigXml xmlconfig;
    auto result = xmlconfig.Load(szEasyControlConfigXml);
    static const std::string szEasyControlConfigXmlNew = "C:/temp/test/EasyControl.xml";
    result = xmlconfig.Save(szEasyControlConfigXmlNew);

    base::xml::CDoseServerXml xmlDoseServer;
    result = xmlDoseServer.Load(xmlconfig.Get().m_DoseServerFile);
    static const std::string szDoseServerXmlNew = "C:/temp/test/DoseServer.xml";
    xmlDoseServer.Save(szDoseServerXmlNew);

    base::xml::CSystemConfigXml xmlSystemConfig;
    result = xmlSystemConfig.Load(xmlconfig.Get().m_SystemConfigFile);
    static const std::string szSystemConfigXmlNew = "C:/temp/test/SystemConfig.xml";
    xmlSystemConfig.Save(szSystemConfigXmlNew);

    base::xml::CLogoConfigXml xml; 
    result = xml.Load(xmlDoseServer.Get().m_path2LogoFile);
    static const std::string szLogoConfigXmlNew = "C:/temp/test/LogoConfig.xml";
    xml.Save(szLogoConfigXmlNew);

#if 0
    base::xml::CSystemConfigXml xml;

    BOOL boolresult = FALSE;
    simpleparser::evaluate(szLogoString, refmap, boolresult);
    //simpleparser::evaluate(szLogoString, refmap);
#endif
}



int main()
{
    //testHopper();
    //testDoseCurve();
    //testParser();
    //Command test;
    //testMassflowmemory();
    //testReclist();
    //testloglist();
    //testRecipelist();
    //testUserlist();

    // testTrendGraphic();
    //testEasyControlConfig();
    // testSystemAlarm();
    //testSystemConfig();
    //testSystemConfigXml();
    // testDBTableXml();
    //testS7SnapXml();
    //testS7DaveXml();
        //testTableProfibusXml();
    //testSystemAlarmXml();
    //testRemoteServerXml();
    //testDoseServerXml();
#if 0
    std::shared_ptr<int> k = nullptr;
    try
    {
        std::weak_ptr<int> w = k;
    }
    catch (...)
    {
        int k = 0;
    }

    k.reset(new int(5));

    std::shared_ptr<int> p = w.lock();

    std::cout << "w(count) = " << w.use_count();



    remote::CRemoteServiceInterface* pInterface;
    IDoseServerInterface* pDoseServerInterface;
    remote::RemoteServiceConfig aCfg;;
    DEVICECFG aDoseServerCfg;
    base::utils::baseZeroMemory(aDoseServerCfg);

    aDoseServerCfg.sItems = 2U;
    aDoseServerCfg.ui[0] = base::eDoseType::DOSETYPE_LWS;
    aDoseServerCfg.ui[1] = base::eDoseType::DOSETYPE_LWS;
    aDoseServerCfg.wi[0] = base::eLcTypes::LC_ETC;
    aDoseServerCfg.wi[1] = base::eLcTypes::LC_ETC;

    //base::utils::baseZeroMemory(aCfg);
   // (void)GetDoseServerInterface(&pDoseServerInterface);
    //pDoseServerInterface->Open(aDoseServerCfg);
    //pDoseServerInterface->Start();
    //pDoseServerInterface->Save(DEFAULTSERVERCONFIGPATH);
    //pDoseServerInterface->Load(DEFAULTSERVERCONFIGPATH);


    aCfg.iProtokoll = remote::RemoteServerProtocol::REMOTE_PROTOCOL_S7DAVE;
    //aCfg.pRemoteInterface = pDoseServerInterface->GetRemoteInterface();
    strcpy_s(aCfg.aDetail.aS7Cfg.szPath2CfgXml, DEFAULTSZCONFIGPATH);

    std::string sz;
    sz.append(aCfg.aDetail.aS7Cfg.szPath2CfgXml);

    GetRemoteServiceS7Interface(&pInterface);
    pInterface->Open(aCfg);
    pInterface->Close();
//    pDoseServerInterface->Close();
#endif
      return 0;

}