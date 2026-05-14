//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module GenericRemoteHandler
///
/// @file   GenericRemoteHandler.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <cassert>
#include <functional>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include "BASE/types.h"
#include "BASE/include/GenericRemoteConfig.h"

#define DECLARE_WRITEFUNC(__Name) virtual BOOL Write ## __Name( const base::BlockDescriptor&) = 0;
#define DECLARE_READFUNC(__Name) virtual BOOL Read ## __Name( const base::BlockDescriptor&) = 0;

namespace base
{
class CMemoryBank
{
    uint8_t *m_buffer;
    uint32_t m_length;
    uint32_t m_counter;
public:
    CMemoryBank() : m_buffer{ nullptr }
                    , m_length { 0 }
                    , m_counter{ 0 }
    {}
    ~CMemoryBank()
    {
        close();
    }
    CMemoryBank(const CMemoryBank&) = delete;
    CMemoryBank& operator = (const CMemoryBank&) = delete;
    CMemoryBank(CMemoryBank&&) = delete;

    void init(uint32_t length, uint8_t initvalue = 0)
    {
        close();
        assert(m_buffer == nullptr);
        assert(m_length == 0);
        assert(m_counter == 0);
        uint32_t l = ((length >> 10) + 1) << 10;
        m_buffer = new uint8_t[l];
        memset(m_buffer, initvalue, l);
        m_length = length;
        m_counter = 0;
    }

    void close()
    {
        if (m_buffer)
        {
            delete[] m_buffer;
            m_buffer = nullptr;
        }
        m_length = 0;
        m_counter = 0;
    }

    uint8_t* alloc(const uint32_t length)
    {
        assert(m_counter + length <= m_length);
        uint8_t* p = &m_buffer[m_counter];
        m_counter += length;
        return p;
    }
};

enum class RemoteHandlerError : int32_t
{
    eOK = 0,
    eReadError,
    eWriteError
};

class CGenericRemoteHandler
{
    using RemoteCallBack = std::function<BOOL(const base::BlockDescriptor&) >;

    using BlockDescriptorPointerVec = std::vector<BlockDescriptor*>;
    using RemoteBaseAddressBlockDescriptorMap = std::map<uint32_t, BlockDescriptorPointerVec>;

    struct Memory
    {
        uint32_t baseaddress;
        uint32_t offset;
        uint8_t* pData;
        uint32_t bytelength;
    };
    using BlockStream = std::vector<Memory>;

    BlockStream m_ReadBlockStream;
    BlockStream m_WriteBlockStream;
    CMemoryBank m_Memorybank;
    BOOL        m_bInputOffline;

protected:
    using RemoteCategoryCallBackMap = std::map< base::RemoteCategory, RemoteCallBack>;
    using RemoteCategoryReadWriteBlockMap = std::map< RemoteCategory, BlockDescriptorVec>;

    RemoteCategoryCallBackMap       m_Host2DeviceMap;     // Host   -> Device
    RemoteCategoryCallBackMap       m_Device2HostMap;    // Device -> Host
    RemoteCategoryReadWriteBlockMap m_ReadBlockMap;
    RemoteCategoryReadWriteBlockMap m_WriteBlockMap;
    ProcessControlDescriptor        m_ProcessControl;
    ProcessStatusDescriptor         m_ProcessStatus;
    AlarmStatusDescriptor           m_AlarmStatus;
    LineProcessControlDescriptor    m_LineProcessControl;
    LineProcessStatusDescriptor     m_LineProcessStatus;
    LineAlarmStatusDescriptor       m_LineAlarmStatus;
    MaskDescriptor                  m_MaskDescriptor;

private:
    static void SortSegments(BlockDescriptorPointerVec& rVec);
    static void CreateBlockMap(const base::BlockDescriptorVec& rList, const base::RemoteDirection dir, RemoteCategoryReadWriteBlockMap& rBlockMap);
    static uint32_t InitByteLength(BlockDescriptorVec& rList);

    void CreateAllocMap(RemoteCategoryReadWriteBlockMap& rList, BlockStream&);
    void MergeSegments(BlockDescriptorPointerVec& rVec, BlockStream&);

protected:
    void ReadAll();
    void WriteAll();
    RemoteHandlerError ReadBuffer();
    RemoteHandlerError Flush();



    DECLARE_WRITEFUNC(PlaceHolder)
    DECLARE_WRITEFUNC(FeedingType)
    DECLARE_WRITEFUNC(Setpoint)
    DECLARE_WRITEFUNC(Massflow)
    DECLARE_WRITEFUNC(ActualWeight)
    DECLARE_WRITEFUNC(Totalizer)
    DECLARE_WRITEFUNC(RecipePercentage)
    DECLARE_WRITEFUNC(Deviation)
    DECLARE_WRITEFUNC(ActualEncoderRotation)
    DECLARE_WRITEFUNC(NominalEncoderRotation)
    DECLARE_WRITEFUNC(ActualDosePerformance)
    DECLARE_WRITEFUNC(DosePerformance)
    DECLARE_WRITEFUNC(DriveCommand)
    DECLARE_WRITEFUNC(TaraWeight)
    DECLARE_WRITEFUNC(EmptyFeederSpeed)
    DECLARE_WRITEFUNC(RefillTime)
    DECLARE_WRITEFUNC(RefillLimitMin)
    DECLARE_WRITEFUNC(RefillLimitMax)
    DECLARE_WRITEFUNC(RefillLimitAlarm)
    DECLARE_WRITEFUNC(LCCorrectionFactor)
    DECLARE_WRITEFUNC(CalibCount)
    DECLARE_WRITEFUNC(CalibDriveCommand1)
    DECLARE_WRITEFUNC(CalibDosePerformance1)
    DECLARE_WRITEFUNC(CalibDuration1)
    DECLARE_WRITEFUNC(CalibDriveCommand2)
    DECLARE_WRITEFUNC(CalibDosePerformance2)
    DECLARE_WRITEFUNC(CalibDuration2)
    DECLARE_WRITEFUNC(CalibDriveCommand3)
    DECLARE_WRITEFUNC(CalibDosePerformance3)
    DECLARE_WRITEFUNC(CalibDuration3)
    DECLARE_WRITEFUNC(CalibDriveCommand4)
    DECLARE_WRITEFUNC(CalibDosePerformance4)
    DECLARE_WRITEFUNC(CalibDuration4)
    DECLARE_WRITEFUNC(CalibDriveCommand5)
    DECLARE_WRITEFUNC(CalibDosePerformance5)
    DECLARE_WRITEFUNC(CalibDuration5)
    DECLARE_WRITEFUNC(CalibDriveCommand6)
    DECLARE_WRITEFUNC(CalibDosePerformance6)
    DECLARE_WRITEFUNC(CalibDuration6)
    DECLARE_WRITEFUNC(CalibDriveCommand7)
    DECLARE_WRITEFUNC(CalibDosePerformance7)
    DECLARE_WRITEFUNC(CalibDuration7)
    DECLARE_WRITEFUNC(CalibDriveCommand8)
    DECLARE_WRITEFUNC(CalibDosePerformance8)
    DECLARE_WRITEFUNC(CalibDuration8)
    DECLARE_WRITEFUNC(CalibDriveCommand9)
    DECLARE_WRITEFUNC(CalibDosePerformance9)
    DECLARE_WRITEFUNC(CalibDuration9)
    DECLARE_WRITEFUNC(CalibDriveCommand10)
    DECLARE_WRITEFUNC(CalibDosePerformance10)
    DECLARE_WRITEFUNC(CalibDuration10)

    DECLARE_WRITEFUNC(PidSampleInterval)
    DECLARE_WRITEFUNC(PaddleSpeed)
    DECLARE_WRITEFUNC(RefillFeederSpeed)
    DECLARE_WRITEFUNC(GateFilter)

    DECLARE_WRITEFUNC(PidGain)
    DECLARE_WRITEFUNC(MassflowFilter)
    DECLARE_WRITEFUNC(AlarmNoiseLimit)
    DECLARE_WRITEFUNC(AlarmStatus)
    DECLARE_WRITEFUNC(ProcessStatus)
    DECLARE_WRITEFUNC(Counter)
    DECLARE_WRITEFUNC(LineSetpoint)
    DECLARE_WRITEFUNC(LineAlarmStatus)
    DECLARE_WRITEFUNC(LineProcessStatus)

    DECLARE_READFUNC(PlaceHolder)
    DECLARE_READFUNC(ControlCmd)

    DECLARE_READFUNC(DoseCmdStart)
    DECLARE_READFUNC(DoseCmdStop)
    DECLARE_READFUNC(DoseCmdTotClear)
    DECLARE_READFUNC(DoseCmdAlarmClear)
    DECLARE_READFUNC(DoseCmdAlarmAck)
    DECLARE_READFUNC(DoseCmdCalibrate)
    DECLARE_READFUNC(DoseCmdTare)
    DECLARE_READFUNC(DoseCmdRefillOff)
    DECLARE_READFUNC(DoseCmdRefillOn)
    DECLARE_READFUNC(DoseCmdInLine)
    DECLARE_READFUNC(DoseCmdLocal)
    DECLARE_READFUNC(DoseCmdGrav)
    DECLARE_READFUNC(DoseCmdVol)
    DECLARE_READFUNC(DoseCmdRefillRelease)

    DECLARE_READFUNC(Setpoint)
    DECLARE_READFUNC(TaraWeight)
    DECLARE_READFUNC(EmptyFeederSpeed)
    DECLARE_READFUNC(RecipePercentage)
    DECLARE_READFUNC(RefillTime)
    DECLARE_READFUNC(RefillLimitMin)
    DECLARE_READFUNC(RefillLimitMax)
    DECLARE_READFUNC(RefillLimitAlarm)
    DECLARE_READFUNC(LCCorrectionFactor)
    DECLARE_READFUNC(CalibCount)
    DECLARE_READFUNC(CalibDriveCommand1)
    DECLARE_READFUNC(CalibDosePerformance1)
    DECLARE_READFUNC(CalibDuration1)
    DECLARE_READFUNC(CalibDriveCommand2)
    DECLARE_READFUNC(CalibDosePerformance2)
    DECLARE_READFUNC(CalibDuration2)
    DECLARE_READFUNC(CalibDriveCommand3)
    DECLARE_READFUNC(CalibDosePerformance3)
    DECLARE_READFUNC(CalibDuration3)
    DECLARE_READFUNC(CalibDriveCommand4)
    DECLARE_READFUNC(CalibDosePerformance4)
    DECLARE_READFUNC(CalibDuration4)
    DECLARE_READFUNC(CalibDriveCommand5)
    DECLARE_READFUNC(CalibDosePerformance5)
    DECLARE_READFUNC(CalibDuration5)
    DECLARE_READFUNC(CalibDriveCommand6)
    DECLARE_READFUNC(CalibDosePerformance6)
    DECLARE_READFUNC(CalibDuration6)
    DECLARE_READFUNC(CalibDriveCommand7)
    DECLARE_READFUNC(CalibDosePerformance7)
    DECLARE_READFUNC(CalibDuration7)
    DECLARE_READFUNC(CalibDriveCommand8)
    DECLARE_READFUNC(CalibDosePerformance8)
    DECLARE_READFUNC(CalibDuration8)
    DECLARE_READFUNC(CalibDriveCommand9)
    DECLARE_READFUNC(CalibDosePerformance9)
    DECLARE_READFUNC(CalibDuration9)
    DECLARE_READFUNC(CalibDriveCommand10)
    DECLARE_READFUNC(CalibDosePerformance10)
    DECLARE_READFUNC(CalibDuration10)
    DECLARE_READFUNC(DosePerformance)
    DECLARE_READFUNC(AlarmMassflowHigh)
    DECLARE_READFUNC(AlarmMassflowLow)
    DECLARE_READFUNC(AlarmDriveCommandHigh)
    DECLARE_READFUNC(AlarmDriveCommandLow)
    DECLARE_READFUNC(AlarmReactionDelay)
    DECLARE_READFUNC(AlarmStartReactionDelay)
    DECLARE_READFUNC(StartupDelay)
    DECLARE_READFUNC(SampleInterval)
    DECLARE_READFUNC(PaddleSpeed)
    DECLARE_READFUNC(RefillFeederSpeed)
    DECLARE_READFUNC(GateFilter)
    DECLARE_READFUNC(PidGain)
    DECLARE_READFUNC(MassflowFilter)
    DECLARE_READFUNC(AlarmNoiseLimit)
    DECLARE_READFUNC(LineSetpoint)
    DECLARE_READFUNC(LinePercentage)
    DECLARE_READFUNC(LineControl)
    DECLARE_READFUNC(LineCmdStart)
    DECLARE_READFUNC(LineCmdStop)
    DECLARE_READFUNC(LineCmdTotClear)
    DECLARE_READFUNC(Mask)

    virtual BOOL ReadData(uint32_t, uint32_t, uint32_t, uint8_t*)  = 0;
    virtual BOOL WriteData(uint32_t, uint32_t, uint32_t, const uint8_t*) = 0;


public:
    CGenericRemoteHandler();
    CGenericRemoteHandler(const CGenericRemoteHandler&) = delete;
    CGenericRemoteHandler(CGenericRemoteHandler&&) = delete;
    CGenericRemoteHandler& operator = (const CGenericRemoteHandler&) = delete;
    virtual ~CGenericRemoteHandler();

    virtual RemoteHandlerError Execute();
    virtual BOOL Open(const std::string& szFilename);
    virtual BOOL Close();
    void SetInputOffline(BOOL bOffline)
    {    m_bInputOffline = bOffline;    }
    BOOL IsInputOffline(void) const
    {    return m_bInputOffline;    }
};

};
