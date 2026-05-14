//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DeviceSimulator
///
/// @file   DeviceSimulator.cpp
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
#include <algorithm>
#include "include/DeviceSimulator.h"
#include "BASE/Utils/public/GenericRemoteHandler.h"
#include "BASE/Utils/public/xml/GenericRemoteXml.h"
#include "BASE/Utils/public/xml/S7SimulatorXml.h"
#include "BASE/include/FieldTypeStack.h"
#include "Logger/include/Logger.h"



#define __STACKFIELD(__a) CFieldTypeStack aStack((__a).m_pData, (__a).m_ByteLength, (__a).m_FieldType, (__a).m_bigEndian, (__a).m_Scale);
#define EDITITEM(_a, _func) 	{ _a, std::bind(&CDeviceSimulator::_func, this, _1) }

using namespace base;
using namespace std::placeholders;

CDeviceSimulator::CDeviceSimulator() : remote::IDeviceInterface()
, m_MapTable {
        EDITITEM(RemoteCategory::W_MASSFLOW, WriteMassflow),
        EDITITEM(RemoteCategory::W_NETWEIGHT, WriteActualWeight),
        EDITITEM(RemoteCategory::W_TOTALIZER, WriteTotalizer),
        EDITITEM(RemoteCategory::W_RECIPE_PERCENTAGE, WriteRecipePercentage),
        EDITITEM(RemoteCategory::W_ENCODERROTATION, WriteActualEncoderRotation),
        EDITITEM(RemoteCategory::W_NOMINALENCODERROTATION, WriteNominalEncoderRotation),
        EDITITEM(RemoteCategory::W_ACTUALDOSEPERFORMANCE, WriteActualDosePerformance),
        EDITITEM(RemoteCategory::W_REFILLLIMITALARM, WriteRefillLimitMinMin),
        EDITITEM(RemoteCategory::W_DRIVECOMMAND, WriteDriveCommand),
        EDITITEM(RemoteCategory::W_TARAWEIGHT, WriteTaraWeight),
        EDITITEM(RemoteCategory::W_SAMPLEINTERVAL, WritePidSampleInterval),
        EDITITEM(RemoteCategory::W_PIDGAIN, WritePidGain),
        EDITITEM(RemoteCategory::W_MASSFLOWFILTER, WriteMassflowFilter),
        EDITITEM(RemoteCategory::W_ALARMNOISELIMIT, WriteAlarmNoiseLimit),
        EDITITEM(RemoteCategory::W_ALARMSTATUS, WriteAlarmStatus),
        EDITITEM(RemoteCategory::W_PROCESSSTATUS, WriteProzessStatus),
        EDITITEM(RemoteCategory::W_COUNTER, WriteCounter),
        EDITITEM(RemoteCategory::R_CONTROL, ReadControlCmd),
        EDITITEM(RemoteCategory::R_SETPOINT, ReadSetpoint),
        EDITITEM(RemoteCategory::R_DOSEPERFORMANCE, ReadDosePerformance),
        EDITITEM(RemoteCategory::R_REFILLLIMITMIN, ReadRefillLimitMin),
        EDITITEM(RemoteCategory::R_REFILLLIMITMAX, ReadRefillLimitMax),
        EDITITEM(RemoteCategory::R_ALARMMASSFLOWHIGH, ReadAlarmMassflowHigh),
        EDITITEM(RemoteCategory::R_ALARMMASSFLOWLOW, ReadAlarmMassflowLow),
        EDITITEM(RemoteCategory::R_ALARMDRIVECOMMANDHIGH, ReadAlarmDriveCommandHigh),
        EDITITEM(RemoteCategory::R_ALARMDRIVECOMMANDLOW, ReadAlarmDriveCommandLow),
        EDITITEM(RemoteCategory::R_REFILLTIME, ReadRefillTime),
        EDITITEM(RemoteCategory::R_REFILLLIMITALARM, ReadRefillLimitAlarm),
        EDITITEM(RemoteCategory::R_ALARMREACTIONDELAY, ReadAlarmReactionDelay),
        EDITITEM(RemoteCategory::R_ALARMSTARTREACTIONDELAY, ReadAlarmStartReactionDelay),
        EDITITEM(RemoteCategory::R_STARTUPDELAY, ReadStartupDelay),
        EDITITEM(RemoteCategory::R_SAMPLEINTERVAL, ReadSampleInterval),
        EDITITEM(RemoteCategory::R_PIDGAIN, ReadPidGain),
        EDITITEM(RemoteCategory::R_MASSFLOWFILTER, ReadMassflowFilter),
        EDITITEM(RemoteCategory::R_ALARMNOISELIMIT, ReadAlarmNoiseLimit)
    }
{
}

BOOL CDeviceSimulator::WriteFeedingType(const base::BlockDescriptor& ) { return TRUE; }
BOOL CDeviceSimulator::WriteMassflow(const base::BlockDescriptor& ) { return TRUE; }
BOOL CDeviceSimulator :: WriteActualWeight(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteTotalizer(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteRecipePercentage(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteActualEncoderRotation(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteNominalEncoderRotation(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteActualDosePerformance(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteRefillLimitMinMin(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteDriveCommand(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteTaraWeight(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WritePidSampleInterval(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WritePidGain(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteMassflowFilter(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteAlarmNoiseLimit(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteAlarmStatus(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteProzessStatus(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: WriteCounter(const base::BlockDescriptor&) { return TRUE; }




BOOL CDeviceSimulator :: ReadSetpoint(const base::BlockDescriptor& rBlock) 
{ 
    auto& list = m_Simcfg.GetList();
    auto it = std::find_if(list.cbegin(), list.cend(), [field = rBlock.m_FieldType](const auto& item)
    {
        return item.m_Category == RemoteCategory::R_SETPOINT;
    });

    if (it != list.cend())
    {
        __STACKFIELD(rBlock)

        const auto& rvec = it->m_Data;
        const int32_t count = __min(rBlock.m_Elements, it->m_Elements);
        assert(it->m_Elements == rvec.size());
        for (int32_t index = 0; index < count; index++)
        {
            switch (rvec[index].type)
            {
                case RemoteFieldType::FLOAT32:
                    aStack.push(rvec[index].value.valF32);
                    break;
                default:
                    assert(FALSE);
                    break;
            }
        }
    }
    return TRUE; 
}

BOOL CDeviceSimulator :: ReadControlCmd(const base::BlockDescriptor&) { return TRUE; }

BOOL CDeviceSimulator :: ReadDosePerformance(const base::BlockDescriptor& rBlock)
{
    auto& list = m_Simcfg.GetList();
    auto it = std::find_if(list.cbegin(), list.cend(), [field = rBlock.m_FieldType](const auto& item)
    {
        return item.m_Category == RemoteCategory::R_DOSEPERFORMANCE;
    });

    if (it != list.cend())
    {
        __STACKFIELD(rBlock)

            const auto& rvec = it->m_Data;
        const int32_t count = __min(rBlock.m_Elements, it->m_Elements);
        assert(it->m_Elements == rvec.size());
        for (int32_t index = 0; index < count; index++)
        {
            switch (rvec[index].type)
            {
            case RemoteFieldType::FLOAT32:
                aStack.push(rvec[index].value.valF32);
                break;
            default:
                assert(FALSE);
                break;
            }
        }
    }
    return TRUE;
}

BOOL CDeviceSimulator :: ReadRefillLimitMin(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadRefillLimitMax(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadRefillLimitAlarm(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadAlarmMassflowHigh(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadAlarmMassflowLow(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadAlarmDriveCommandHigh(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadAlarmDriveCommandLow(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadRefillTime(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadAlarmReactionDelay(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator ::ReadAlarmStartReactionDelay(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadStartupDelay(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadSampleInterval(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadPidGain(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadMassflowFilter(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadAlarmNoiseLimit(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadPaddleSpeed(const base::BlockDescriptor&) { return TRUE; }
BOOL CDeviceSimulator :: ReadRefillFeederSpeed(const base::BlockDescriptor&) { return TRUE; }


//**********************************************************************************************************************
//**********************************************************************************************************************
BOOL CDeviceSimulator::Open(const std::string& _szPath2Device)
{
    base::xml::CS7SimulatorXml xmlbase;

    auto result = xmlbase.Load(_szPath2Device);
    if (result)
    {
        base::xml::CGenericRemoteXml xmlTable;
        result = xmlTable.Load(xmlbase.Get().GetTableFile());
        assert(result);
        m_Tablecfg = xmlTable.Get();
#if 0
        base::xml::CSimValuesXml xmlSimValues;
        result = xmlSimValues.Load(xmlbase.Get().GetValueFile());
        assert(result);
        m_Simcfg = xmlSimValues.Get();
#endif
    }
    return TRUE;
}
//**********************************************************************************************************************
//**********************************************************************************************************************
BOOL CDeviceSimulator::Close()
{
    return TRUE;
}
//***************************************************************************************************
//***************************************************************************************************
static uint32_t getSize(const RemoteFieldType field)
{
    static const uint32_t size[10] = { sizeof(uint8_t), sizeof(uint16_t),
                                       sizeof(uint32_t), sizeof(uint64_t),
                                       sizeof(int8_t), sizeof(int16_t),
                                       sizeof(int32_t), sizeof(int64_t),
                                       sizeof(float32_t), sizeof(float64_t) };
    return size[_S32(field)];
}
//**********************************************************************************************************************
//**********************************************************************************************************************
BOOL CDeviceSimulator::GetBlockDescriptor(int32_t DBnum, int32_t offset, base::BlockDescriptor& rBlock)
{
    BOOL result = FALSE;

    auto& rList = m_Tablecfg.GetList();
    auto it = std::find_if(rList.cbegin(), rList.cend(), [&DBnum, &offset](const auto& rItem)
        {
            return (rItem.m_BaseAddress == DBnum) && (rItem.m_StartOffset == offset);
        });

    if (it != rList.cend())
    {
        rBlock = *it;
        rBlock.m_ByteLength = rBlock.m_Elements * getSize(rBlock.m_FieldType);
        result = TRUE;
    }
    return result;
 }
//**********************************************************************************************************************
//**********************************************************************************************************************
int32_t CDeviceSimulator::ExecuteData(int32_t DBnum, int32_t offset, int32_t len, uint8_t* ulBuffer)
{
    base::BlockDescriptor block;

    uint8_t* pBuffer = (uint8_t*)ulBuffer;
    uint32_t totalbyteswritten = 0;
    while (totalbyteswritten < _U32(len))
    {
        auto result = GetBlockDescriptor(DBnum, offset, block);
        assert(result);
        if (!result)
            break;

        block.m_pData = &pBuffer[totalbyteswritten];
        try
        {
            m_MapTable[block.m_Category](block);
        }
        catch (...)
        {
            assert(FALSE);
            LOGERROR("Blockdescriptor not found ! DB = " << DBnum << ", Offset = " << offset);
        }
        totalbyteswritten += block.m_ByteLength;
        offset += block.m_ByteLength;
    }
    return 0;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
int32_t CDeviceSimulator::WriteData(int32_t DBnum, int32_t offset, int32_t len,const void* ulBuffer)
{
    auto result = ExecuteData(DBnum, offset, len, (uint8_t*)ulBuffer);
    LOGDEBUG("Writing DB = " << std::dec << DBnum);
    return result;
}
//**********************************************************************************************************************
//**********************************************************************************************************************
int32_t CDeviceSimulator::ReadData(int32_t DBnum, int32_t offset, int32_t len, void* ulBuffer)
{
    auto result = ExecuteData(DBnum, offset, len, (uint8_t*)ulBuffer);
    LOGDEBUG("Reading DB = " << std::dec << DBnum);
    return result;
}


