//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module GenericRemoteHandler
///
/// @file   GenericRemoteHandler.cpp
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
#include "BASE/Utils/public/GenericRemoteHandler.h"
#include "BASE/Utils/public/xml/GenericRemoteXml.h"
#include "Logger/include/Logger.h"

using namespace std::placeholders;
using namespace base;

#define EDITITEM(_a, _func) 	{ _a, std::bind(&CGenericRemoteHandler::_func, this, _1) }

namespace base
{
//*********************************************************************************************************
//*********************************************************************************************************
    CGenericRemoteHandler::CGenericRemoteHandler() :
        m_Host2DeviceMap{      EDITITEM(RemoteCategory::C_PLACEHOLDER , WritePlaceHolder),
                               EDITITEM(RemoteCategory::W_FEEDERTYPE , WriteFeedingType),
                               EDITITEM(RemoteCategory::W_SETPOINT , WriteSetpoint),
                               EDITITEM(RemoteCategory::W_MASSFLOW , WriteMassflow),
                               EDITITEM(RemoteCategory::W_NETWEIGHT, WriteActualWeight),
                               EDITITEM(RemoteCategory::W_TOTALIZER, WriteTotalizer),
                               EDITITEM(RemoteCategory::W_RECIPE_PERCENTAGE, WriteRecipePercentage),
                               EDITITEM(RemoteCategory::W_DEVIATION, WriteDeviation),
                               EDITITEM(RemoteCategory::W_ENCODERROTATION, WriteActualEncoderRotation),
                               EDITITEM(RemoteCategory::W_NOMINALENCODERROTATION, WriteNominalEncoderRotation),
                               EDITITEM(RemoteCategory::W_ACTUALDOSEPERFORMANCE, WriteActualDosePerformance),
                               EDITITEM(RemoteCategory::W_DOSEPERFORMANCE, WriteDosePerformance),
                               EDITITEM(RemoteCategory::W_DRIVECOMMAND, WriteDriveCommand),
                               EDITITEM(RemoteCategory::W_TARAWEIGHT, WriteTaraWeight),
                               EDITITEM(RemoteCategory::W_EMPTYFEEDERSPEED, WriteEmptyFeederSpeed),
                               EDITITEM(RemoteCategory::W_REFILLTIME, WriteRefillTime),
                               EDITITEM(RemoteCategory::W_REFILLLIMITMIN, WriteRefillLimitMin),
                               EDITITEM(RemoteCategory::W_REFILLLIMITMAX, WriteRefillLimitMax),
                               EDITITEM(RemoteCategory::W_REFILLLIMITALARM, WriteRefillLimitAlarm),
                               EDITITEM(RemoteCategory::W_LCCORRECTIONFACTOR, WriteLCCorrectionFactor),
                               EDITITEM(RemoteCategory::W_CALIB_COUNT, WriteCalibCount),
                               EDITITEM(RemoteCategory::W_CALIB_DRIVECOMMAND1, WriteCalibDriveCommand1),
                               EDITITEM(RemoteCategory::W_CALIB_DOSEPERFORMANCE1, WriteCalibDosePerformance1),
                               EDITITEM(RemoteCategory::W_CALIB_DURATION1, WriteCalibDuration1),
                               EDITITEM(RemoteCategory::W_CALIB_DRIVECOMMAND2, WriteCalibDriveCommand2),
                               EDITITEM(RemoteCategory::W_CALIB_DOSEPERFORMANCE2, WriteCalibDosePerformance2),
                               EDITITEM(RemoteCategory::W_CALIB_DURATION2, WriteCalibDuration2),
                               EDITITEM(RemoteCategory::W_CALIB_DRIVECOMMAND3, WriteCalibDriveCommand3),
                               EDITITEM(RemoteCategory::W_CALIB_DOSEPERFORMANCE3, WriteCalibDosePerformance3),
                               EDITITEM(RemoteCategory::W_CALIB_DURATION3, WriteCalibDuration3),
                               EDITITEM(RemoteCategory::W_CALIB_DRIVECOMMAND4, WriteCalibDriveCommand4),
                               EDITITEM(RemoteCategory::W_CALIB_DOSEPERFORMANCE4, WriteCalibDosePerformance4),
                               EDITITEM(RemoteCategory::W_CALIB_DURATION4, WriteCalibDuration4),
                               EDITITEM(RemoteCategory::W_CALIB_DRIVECOMMAND5, WriteCalibDriveCommand5),
                               EDITITEM(RemoteCategory::W_CALIB_DOSEPERFORMANCE5, WriteCalibDosePerformance5),
                               EDITITEM(RemoteCategory::W_CALIB_DURATION5, WriteCalibDuration5),
                               EDITITEM(RemoteCategory::W_CALIB_DRIVECOMMAND6, WriteCalibDriveCommand6),
                               EDITITEM(RemoteCategory::W_CALIB_DOSEPERFORMANCE6, WriteCalibDosePerformance6),
                               EDITITEM(RemoteCategory::W_CALIB_DURATION6, WriteCalibDuration6),
                               EDITITEM(RemoteCategory::W_CALIB_DRIVECOMMAND7, WriteCalibDriveCommand7),
                               EDITITEM(RemoteCategory::W_CALIB_DOSEPERFORMANCE7, WriteCalibDosePerformance7),
                               EDITITEM(RemoteCategory::W_CALIB_DURATION7, WriteCalibDuration7),
                               EDITITEM(RemoteCategory::W_CALIB_DRIVECOMMAND8, WriteCalibDriveCommand8),
                               EDITITEM(RemoteCategory::W_CALIB_DOSEPERFORMANCE8, WriteCalibDosePerformance8),
                               EDITITEM(RemoteCategory::W_CALIB_DURATION8, WriteCalibDuration8),
                               EDITITEM(RemoteCategory::W_CALIB_DRIVECOMMAND9, WriteCalibDriveCommand9),
                               EDITITEM(RemoteCategory::W_CALIB_DOSEPERFORMANCE9, WriteCalibDosePerformance9),
                               EDITITEM(RemoteCategory::W_CALIB_DURATION9, WriteCalibDuration9),
                               EDITITEM(RemoteCategory::W_CALIB_DRIVECOMMAND10, WriteCalibDriveCommand10),
                               EDITITEM(RemoteCategory::W_CALIB_DOSEPERFORMANCE10, WriteCalibDosePerformance10),
                               EDITITEM(RemoteCategory::W_CALIB_DURATION10, WriteCalibDuration10),

                               EDITITEM(RemoteCategory::W_SAMPLEINTERVAL, WritePidSampleInterval),
                               EDITITEM(RemoteCategory::W_PADDLESPEED, WritePaddleSpeed),
                               EDITITEM(RemoteCategory::W_PIDGAIN, WritePidGain),
                               EDITITEM(RemoteCategory::W_GATEFILTER, WriteGateFilter),
                               EDITITEM(RemoteCategory::W_MASSFLOWFILTER, WriteMassflowFilter),
                               EDITITEM(RemoteCategory::W_ALARMNOISELIMIT, WriteAlarmNoiseLimit),
                               EDITITEM(RemoteCategory::W_ALARMSTATUS, WriteAlarmStatus),
                               EDITITEM(RemoteCategory::W_PROCESSSTATUS, WriteProcessStatus),
                               EDITITEM(RemoteCategory::W_COUNTER, WriteCounter),
                               EDITITEM(RemoteCategory::W_LINESETPOINT, WriteLineSetpoint),
                               EDITITEM(RemoteCategory::W_LINEALARMSTATUS, WriteLineAlarmStatus),
                               EDITITEM(RemoteCategory::W_LINEPROCESSSTATUS, WriteLineProcessStatus),


    }
        , m_Device2HostMap{ EDITITEM(RemoteCategory::C_PLACEHOLDER , ReadPlaceHolder),
                            EDITITEM(RemoteCategory::R_CONTROL, ReadControlCmd),

                            EDITITEM(RemoteCategory::R_DOSECMDSTART, ReadDoseCmdStart),
                            EDITITEM(RemoteCategory::R_DOSECMDSTOP, ReadDoseCmdStop),
                            EDITITEM(RemoteCategory::R_DOSECMDTOTCLEAR, ReadDoseCmdTotClear),
                            EDITITEM(RemoteCategory::R_DOSECMDALARMCLEAR, ReadDoseCmdAlarmClear),
                            EDITITEM(RemoteCategory::R_DOSECMDALARMACK, ReadDoseCmdAlarmAck),
                            EDITITEM(RemoteCategory::R_DOSECMDCALIBRATE, ReadDoseCmdCalibrate),
                            EDITITEM(RemoteCategory::R_DOSECMDTARE, ReadDoseCmdTare),
                            EDITITEM(RemoteCategory::R_DOSECMDREFILLOFF, ReadDoseCmdRefillOff),
                            EDITITEM(RemoteCategory::R_DOSECMDREFILLON, ReadDoseCmdRefillOn),
                            EDITITEM(RemoteCategory::R_DOSECMDINLINE, ReadDoseCmdInLine),
                            EDITITEM(RemoteCategory::R_DOSECMDLOCAL, ReadDoseCmdLocal),
                            EDITITEM(RemoteCategory::R_DOSECMDGRAV, ReadDoseCmdGrav),
                            EDITITEM(RemoteCategory::R_DOSECMDVOL, ReadDoseCmdVol),
                            EDITITEM(RemoteCategory::R_DOSECMDREFILLRELEASE, ReadDoseCmdRefillRelease),

                            EDITITEM(RemoteCategory::R_SETPOINT, ReadSetpoint),
                            EDITITEM(RemoteCategory::R_TARAWEIGHT, ReadTaraWeight),
                            EDITITEM(RemoteCategory::R_EMPTYFEEDERSPEED, ReadEmptyFeederSpeed),
                            EDITITEM(RemoteCategory::R_RECIPE_PERCENTAGE, ReadRecipePercentage),
                            EDITITEM(RemoteCategory::R_REFILLTIME, ReadRefillTime),
                            EDITITEM(RemoteCategory::R_REFILLLIMITMIN, ReadRefillLimitMin),
                            EDITITEM(RemoteCategory::R_REFILLLIMITMAX, ReadRefillLimitMax),
                            EDITITEM(RemoteCategory::R_REFILLLIMITALARM, ReadRefillLimitAlarm),
                            EDITITEM(RemoteCategory::R_LCCORRECTIONFACTOR, ReadLCCorrectionFactor),
                            EDITITEM(RemoteCategory::R_DOSEPERFORMANCE, ReadDosePerformance),
                            EDITITEM(RemoteCategory::R_CALIB_COUNT, ReadCalibCount),
                            EDITITEM(RemoteCategory::R_CALIB_DRIVECOMMAND1, ReadCalibDriveCommand1),
                            EDITITEM(RemoteCategory::R_CALIB_DOSEPERFORMANCE1, ReadCalibDosePerformance1),
                            EDITITEM(RemoteCategory::R_CALIB_DURATION1, ReadCalibDuration1),
                            EDITITEM(RemoteCategory::R_CALIB_DRIVECOMMAND2, ReadCalibDriveCommand2),
                            EDITITEM(RemoteCategory::R_CALIB_DOSEPERFORMANCE2, ReadCalibDosePerformance2),
                            EDITITEM(RemoteCategory::R_CALIB_DURATION2, ReadCalibDuration2),
                            EDITITEM(RemoteCategory::R_CALIB_DRIVECOMMAND3, ReadCalibDriveCommand3),
                            EDITITEM(RemoteCategory::R_CALIB_DOSEPERFORMANCE3, ReadCalibDosePerformance3),
                            EDITITEM(RemoteCategory::R_CALIB_DURATION3, ReadCalibDuration3),
                            EDITITEM(RemoteCategory::R_CALIB_DRIVECOMMAND4, ReadCalibDriveCommand4),
                            EDITITEM(RemoteCategory::R_CALIB_DOSEPERFORMANCE4, ReadCalibDosePerformance4),
                            EDITITEM(RemoteCategory::R_CALIB_DURATION4, ReadCalibDuration4),
                            EDITITEM(RemoteCategory::R_CALIB_DRIVECOMMAND5, ReadCalibDriveCommand5),
                            EDITITEM(RemoteCategory::R_CALIB_DOSEPERFORMANCE5, ReadCalibDosePerformance5),
                            EDITITEM(RemoteCategory::R_CALIB_DURATION5, ReadCalibDuration5),
                            EDITITEM(RemoteCategory::R_CALIB_DRIVECOMMAND6, ReadCalibDriveCommand6),
                            EDITITEM(RemoteCategory::R_CALIB_DOSEPERFORMANCE6, ReadCalibDosePerformance6),
                            EDITITEM(RemoteCategory::R_CALIB_DURATION6, ReadCalibDuration6),
                            EDITITEM(RemoteCategory::R_CALIB_DRIVECOMMAND7, ReadCalibDriveCommand7),
                            EDITITEM(RemoteCategory::R_CALIB_DOSEPERFORMANCE7, ReadCalibDosePerformance7),
                            EDITITEM(RemoteCategory::R_CALIB_DURATION7, ReadCalibDuration7),
                            EDITITEM(RemoteCategory::R_CALIB_DRIVECOMMAND8, ReadCalibDriveCommand8),
                            EDITITEM(RemoteCategory::R_CALIB_DOSEPERFORMANCE8, ReadCalibDosePerformance8),
                            EDITITEM(RemoteCategory::R_CALIB_DURATION8, ReadCalibDuration8),
                            EDITITEM(RemoteCategory::R_CALIB_DRIVECOMMAND9, ReadCalibDriveCommand9),
                            EDITITEM(RemoteCategory::R_CALIB_DOSEPERFORMANCE9, ReadCalibDosePerformance9),
                            EDITITEM(RemoteCategory::R_CALIB_DURATION9, ReadCalibDuration9),
                            EDITITEM(RemoteCategory::R_CALIB_DRIVECOMMAND10, ReadCalibDriveCommand10),
                            EDITITEM(RemoteCategory::R_CALIB_DOSEPERFORMANCE10, ReadCalibDosePerformance10),
                            EDITITEM(RemoteCategory::R_CALIB_DURATION10, ReadCalibDuration10),

                            EDITITEM(RemoteCategory::R_ALARMMASSFLOWHIGH, ReadAlarmMassflowHigh),
                            EDITITEM(RemoteCategory::R_ALARMMASSFLOWLOW, ReadAlarmMassflowLow),
                            EDITITEM(RemoteCategory::R_ALARMDRIVECOMMANDHIGH, ReadAlarmDriveCommandHigh),
                            EDITITEM(RemoteCategory::R_ALARMDRIVECOMMANDLOW, ReadAlarmDriveCommandLow),
                            EDITITEM(RemoteCategory::R_ALARMREACTIONDELAY, ReadAlarmReactionDelay),
                            EDITITEM(RemoteCategory::R_ALARMSTARTREACTIONDELAY, ReadAlarmStartReactionDelay),
                            EDITITEM(RemoteCategory::R_STARTUPDELAY, ReadStartupDelay),
                            EDITITEM(RemoteCategory::R_SAMPLEINTERVAL, ReadSampleInterval),
                            EDITITEM(RemoteCategory::R_PADDLESPEED, ReadPaddleSpeed),
                            EDITITEM(RemoteCategory::R_REFILLFEEDERSPEED, ReadRefillFeederSpeed),

                            EDITITEM(RemoteCategory::R_PIDGAIN, ReadPidGain),
                            EDITITEM(RemoteCategory::R_GATEFILTER, ReadGateFilter),
                            EDITITEM(RemoteCategory::R_MASSFLOWFILTER, ReadMassflowFilter),
                            EDITITEM(RemoteCategory::R_ALARMNOISELIMIT, ReadAlarmNoiseLimit),
                            EDITITEM(RemoteCategory::R_LINESETPOINT, ReadLineSetpoint),
                            EDITITEM(RemoteCategory::R_LINEPERCENTAGE, ReadLinePercentage),
                            EDITITEM(RemoteCategory::R_LINECONTROL, ReadLineControl),
                            EDITITEM(RemoteCategory::R_LINECMDSTART, ReadLineCmdStart),
                            EDITITEM(RemoteCategory::R_LINECMDSTOP, ReadLineCmdStop),
                            EDITITEM(RemoteCategory::R_LINECMDTOTCLEAR, ReadLineCmdTotClear),
                            EDITITEM(RemoteCategory::R_MASK, ReadMask)
        }
    , m_bInputOffline{ FALSE }
{
}
//*********************************************************************************************************
//*********************************************************************************************************
CGenericRemoteHandler::~CGenericRemoteHandler()
{
    Close();
}
//*********************************************************************************************************
//*********************************************************************************************************
void CGenericRemoteHandler::ReadAll()
{
    // Buffer is filled
    for (const auto& item : m_ReadBlockMap)
    {
        for (const auto& block : item.second)
        {
            try
            {
                (void)m_Device2HostMap[item.first](block);
            }
            catch (...)
            {
                LOGERROR("Item not found = " << _S32(item.first));
            }
        }
    }
}
//*********************************************************************************************************
//*********************************************************************************************************
void CGenericRemoteHandler::WriteAll()
{
    for (const auto& item : m_WriteBlockMap)
    {
        for (const auto& block : item.second)
        {
            try
            {
                (void)m_Host2DeviceMap[item.first](block);
            }
            catch (...)
            {
                LOGERROR("Item not found = " << _S32(item.first));
            }
        }
    }
}

//*****************************************************************************************************************
//*****************************************************************************************************************
RemoteHandlerError CGenericRemoteHandler::Execute()
{
    RemoteHandlerError readresult = RemoteHandlerError::eOK;
    if ( ! m_bInputOffline)
    {
        readresult = ReadBuffer();
        ReadAll();
    }
    WriteAll();
    RemoteHandlerError writeresult = Flush();
    return (readresult != RemoteHandlerError::eOK) ? readresult : ((writeresult != RemoteHandlerError::eOK) ? writeresult : RemoteHandlerError::eOK);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
RemoteHandlerError CGenericRemoteHandler::Flush()
{
    RemoteHandlerError result = RemoteHandlerError::eOK;
    for (const auto& item : m_WriteBlockStream)
    {
        auto bErg = WriteData(item.baseaddress, item.offset, item.bytelength, item.pData);
        if (bErg == FALSE)
        {
            LOGERROR("Error reading  ( " << item.baseaddress << " , " << item.offset << " , " << item.bytelength << " )");
            result = RemoteHandlerError::eWriteError;
        }
    }
    return result;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
RemoteHandlerError CGenericRemoteHandler::ReadBuffer()
{
    RemoteHandlerError result = RemoteHandlerError::eOK;
    for (const auto& item : m_ReadBlockStream)
    {
        auto bErg = ReadData(item.baseaddress, item.offset, item.bytelength, item.pData);
        if (bErg == FALSE)
        {
            LOGERROR("Error reading  ( " << item.baseaddress << " , " << item.offset << " , " << item.bytelength << " )");
            result = RemoteHandlerError::eReadError;
        }
    }
    return result;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CGenericRemoteHandler::CreateBlockMap(const base::BlockDescriptorVec& rList, const RemoteDirection dir, RemoteCategoryReadWriteBlockMap& rBlockMap)
{
    rBlockMap.clear();
    for (const auto& item : rList)
    {
        if (item.m_Direction == dir)
        {
            auto it = rBlockMap.find(item.m_Category);
            if (it == rBlockMap.end())
            {
                rBlockMap.insert({ item.m_Category , BlockDescriptorVec{ item } });
            }
            else
            {
                it->second.push_back( item );
            }
        }
    }
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CGenericRemoteHandler::SortSegments(BlockDescriptorPointerVec& rVec)
{
    const int32_t veccount = _S32(rVec.size());
    for (int32_t k = 0; k < veccount - 1; k++)
    {
        int32_t m = k;
        for (int32_t j = k + 1; j < veccount; j++)
        {
            if (rVec[j]->m_StartOffset < rVec[m]->m_StartOffset)
            {
                m = j;
            }
        }
        if (m != k)
        {
            auto temp = rVec[k];
            rVec[k] = rVec[m];
            rVec[m] = temp;
        }
    }
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void  CGenericRemoteHandler::MergeSegments(BlockDescriptorPointerVec& rVec, BlockStream& memoryBank)
{
    int32_t k = 0;
    const int32_t veccount = _S32(rVec.size());
    while ( k < veccount)
    {
        uint32_t totalbyteLength = rVec[k]->m_ByteLength;
        uint32_t kStartOffset = rVec[k]->m_StartOffset + rVec[k]->m_ByteLength;
        int32_t j = k + 1;
        for ( ; j < veccount; j++)
        {
            if (rVec[j]->m_StartOffset != kStartOffset)
            {
                break;
            }
            kStartOffset    += rVec[j]->m_ByteLength;
            totalbyteLength += rVec[j]->m_ByteLength;
        }
        Memory aMem{ rVec[k]->m_BaseAddress,
                      rVec[k]->m_StartOffset,
                      m_Memorybank.alloc(totalbyteLength),
                      totalbyteLength };

        assert(aMem.baseaddress == rVec[k]->m_BaseAddress);
        assert(aMem.offset == rVec[k]->m_StartOffset);
        assert(aMem.bytelength == totalbyteLength);
        assert(aMem.pData != nullptr);

        uint8_t* pBuffer = aMem.pData;
        for (int32_t i = k; i < j; i++)
        {
            rVec[i]->m_pData = pBuffer;
            pBuffer += rVec[i]->m_ByteLength;
        }
        memoryBank.emplace_back(aMem);
        k = j;
    }
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CGenericRemoteHandler::CreateAllocMap(RemoteCategoryReadWriteBlockMap& rList, BlockStream& blockstream)
{
    RemoteBaseAddressBlockDescriptorMap addressMap; // { { baseaddress }, &bl1, , &bl2 ..., , &blN }

    for (auto& listitem : rList)
    {
        for (auto& block : listitem.second)
        {
            auto it = addressMap.find(block.m_BaseAddress);
            if (it == addressMap.end())
            {
                addressMap.insert({ block.m_BaseAddress , BlockDescriptorPointerVec { &block } });
            }
            else
            {
                it->second.push_back(&block);
            }
        }
    }
    for (auto& item : addressMap)
    {
        // First = Basisadresse, third = vector
        SortSegments(item.second);
        MergeSegments(item.second, blockstream);
    }
}
//*****************************************************************************************************************
//*****************************************************************************************************************
uint32_t CGenericRemoteHandler::InitByteLength(BlockDescriptorVec& rList)
{
    uint32_t totaleBytelength = 0;
    for (auto& item : rList)
    {
        assert(item.m_pData == nullptr);
        if (item.m_WordAlign)
        {
            item.m_BaseAddress <<= 1U;
            item.m_StartOffset <<= 1U;
        }
        item.m_pData = nullptr; 
        item.m_ByteLength = item.m_Elements * base::CGenericRemoteConfig :: getSize(item.m_FieldType);
        totaleBytelength += item.m_ByteLength;
    }
    return totaleBytelength;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
BOOL CGenericRemoteHandler::Open(const std::string& szFilename)
{
    // xmlRead
    xml::CGenericRemoteXml aXml;

    auto result = aXml.Load(szFilename);
    if (result)
    {
        auto& rCfg = aXml.Get();

        m_ProcessControl = rCfg.GetProcessControlDescriptor();
        m_ProcessStatus  = rCfg.GetProcessStatusDescriptor();
        m_AlarmStatus    = rCfg.GetAlarmStatusDescriptor();
        m_LineProcessControl = rCfg.GetLineProcessControlDescriptor();
        m_LineProcessStatus = rCfg.GetLineProcessStatusDescriptor();
        m_LineAlarmStatus = rCfg.GetLineAlarmStatusDescriptor();
        m_MaskDescriptor = rCfg.GetMaskDescriptor();

        auto& rList = rCfg.GetList();
        uint32_t totallentgh = InitByteLength(rList);
        m_Memorybank.init(totallentgh);

        CreateBlockMap(rList, RemoteDirection::Read, m_ReadBlockMap);
        CreateAllocMap(m_ReadBlockMap, m_ReadBlockStream);

        CreateBlockMap(rList, RemoteDirection::Write, m_WriteBlockMap);
        CreateAllocMap(m_WriteBlockMap, m_WriteBlockStream);
    }
    return result;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
BOOL CGenericRemoteHandler::Close()
{
    m_ReadBlockStream.clear();
    m_WriteBlockStream.clear();
    m_Memorybank.close();
    return TRUE;
}




};
