//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SimS7.c
///
/// @file   SimS7.c
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
#include <functional>
#include <map>
#include "BASE/types.h"
#include "remote/include/DeviceInterface.h"
#include "BASE/include/GenericRemoteConfig.h"
#include "BASE/include/SimValuesConfig.h"


class CDeviceSimulator : public remote::IDeviceInterface
{
    using RemoteCallBack = std::function<BOOL(const base::BlockDescriptor&) >;
    using RemoteCategoryCallBackMap = std::map< base::RemoteCategory, RemoteCallBack>;

private:
    base::CGenericRemoteConfig m_Tablecfg;
    base::CSimValuesConfig     m_Simcfg;
    RemoteCategoryCallBackMap  m_MapTable;

protected:
    virtual BOOL WriteFeedingType(const base::BlockDescriptor&);
    virtual BOOL WriteMassflow(const base::BlockDescriptor&);
    virtual BOOL WriteActualWeight(const base::BlockDescriptor&);
    virtual BOOL WriteTotalizer(const base::BlockDescriptor&);
    virtual BOOL WriteRecipePercentage(const base::BlockDescriptor&);
    virtual BOOL WriteActualEncoderRotation(const base::BlockDescriptor&);
    virtual BOOL WriteNominalEncoderRotation(const base::BlockDescriptor&);
    virtual BOOL WriteActualDosePerformance(const base::BlockDescriptor&);
    virtual BOOL WriteRefillLimitMinMin(const base::BlockDescriptor&);
    virtual BOOL WriteDriveCommand(const base::BlockDescriptor&);
    virtual BOOL WriteTaraWeight(const base::BlockDescriptor&);
    virtual BOOL WritePidSampleInterval(const base::BlockDescriptor&);
    virtual BOOL WritePidGain(const base::BlockDescriptor&);
    virtual BOOL WriteMassflowFilter(const base::BlockDescriptor&);
    virtual BOOL WriteAlarmNoiseLimit(const base::BlockDescriptor&);
    virtual BOOL WriteAlarmStatus(const base::BlockDescriptor&);
    virtual BOOL WriteProzessStatus(const base::BlockDescriptor&);
    virtual BOOL WriteCounter(const base::BlockDescriptor&);

    virtual BOOL ReadSetpoint(const base::BlockDescriptor&);
    virtual BOOL ReadControlCmd(const base::BlockDescriptor&);
    virtual BOOL ReadDosePerformance(const base::BlockDescriptor&);
    virtual BOOL ReadRefillLimitMin(const base::BlockDescriptor&);
    virtual BOOL ReadRefillLimitMax(const base::BlockDescriptor&);
    virtual BOOL ReadRefillLimitAlarm(const base::BlockDescriptor&);
    virtual BOOL ReadAlarmMassflowHigh(const base::BlockDescriptor&);
    virtual BOOL ReadAlarmMassflowLow(const base::BlockDescriptor&);
    virtual BOOL ReadAlarmDriveCommandHigh(const base::BlockDescriptor&);
    virtual BOOL ReadAlarmDriveCommandLow(const base::BlockDescriptor&);
    virtual BOOL ReadRefillTime(const base::BlockDescriptor&);
    virtual BOOL ReadAlarmReactionDelay(const base::BlockDescriptor&);
    virtual BOOL ReadAlarmStartReactionDelay(const base::BlockDescriptor&);
    virtual BOOL ReadStartupDelay(const base::BlockDescriptor&);
    virtual BOOL ReadSampleInterval(const base::BlockDescriptor&);
    virtual BOOL ReadPidGain(const base::BlockDescriptor&);
    virtual BOOL ReadMassflowFilter(const base::BlockDescriptor&);
    virtual BOOL ReadAlarmNoiseLimit(const base::BlockDescriptor&);
    virtual BOOL ReadRefillFeederSpeed(const base::BlockDescriptor&);
    virtual BOOL ReadPaddleSpeed(const base::BlockDescriptor&);


    BOOL GetBlockDescriptor(int32_t DBnum, int32_t offset, base::BlockDescriptor& rBlock);
    int32_t ExecuteData(int32_t DBnum, int32_t offset, int32_t len, uint8_t* ulBuffer);

public:
    CDeviceSimulator();
    ~CDeviceSimulator() override
    {  Close();   }

    BOOL Open(const std::string& _szPath2Device) override;
    BOOL Close() override;

    int32_t WriteData(int32_t DBnum, int32_t offset, int32_t len, const void* buffer) override;
    int32_t ReadData(int32_t DBnum, int32_t offset, int32_t len, void* buffer) override;

};

