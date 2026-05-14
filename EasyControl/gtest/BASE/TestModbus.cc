#include "pch.h"
#include <cassert>
#include "remote/include/RemoteHandler.h"
#include "remote/include/DeviceInterface.h"
#include "remote/include/RemoteInterface.h"
#include "BASE/Utils/public/xml/GenericRemoteXml.h"

using namespace remote;
using namespace base;

using ::testing::_;
using ::testing::Invoke;

class CMockDevice : public remote::IDeviceInterface
{
private:
    int32_t Write(int32_t DBnum, int32_t offset, int32_t len, const void* buffer)
    {
        printf("Test !!!\n");
        return 0;
    }
    int32_t Read(int32_t DBnum, int32_t offset, int32_t len,  void* buffer)
    {
        if (DBnum == 254 && offset == 0)
        {
            assert(len == 40);
            uint16_t value = 1U; // start
            uint16_t* wBuffer = (uint16_t*)buffer;
            const int32_t wLen = len / 2;
            for (int32_t i = 0; i < wLen; i++)
            {
                *wBuffer++ = value;
            }
        }
        else
        {
            memset(buffer, 0, len);
        }
        return 0;
    }

public:
    MOCK_METHOD1(Open, BOOL(const std::string& _path2Config));
    MOCK_METHOD0(Close, BOOL());
    MOCK_METHOD4(WriteData, int32_t(int32_t DBnum, int32_t offset, int32_t len, const void* buffer));
    MOCK_METHOD4(ReadData, int32_t(int32_t DBnum, int32_t offset, int32_t len,  void* buffer));

    void delegate ()
    {
        ON_CALL(*this, ReadData(_,_,_,_)).WillByDefault(Invoke([this](int32_t DBnum, int32_t offset, int32_t len, void* buffer)
            { return Read(DBnum, offset, len, buffer);   }));

        ON_CALL(*this, WriteData(_, _, _, _)).WillByDefault(Invoke([this](int32_t DBnum, int32_t offset, int32_t len, const void* buffer) 
                { return Write(DBnum, offset, len, buffer);   }));
    };
};


class CMockRemote : public remote::IRemoteInterface
{
public:
    MOCK_METHOD1(getMaxDoseCount, int32_t(uint32_t& value));
    MOCK_METHOD1(setLineCmdAlarm, int32_t(const int32_t index));
    MOCK_METHOD0(setLineCmdAlarm03Quit, int32_t());
    MOCK_METHOD0(setLineCmdAlarm1Quit, int32_t());
    MOCK_METHOD0(setLineCmdClearTotalizer, int32_t());
    MOCK_METHOD0(setLineCmdRampUp, int32_t());
    MOCK_METHOD0(setLineCmdRampDown, int32_t());
    MOCK_METHOD0(setLineCmdExtLineSetpointOn, int32_t());
    MOCK_METHOD0(setLineCmdExtLineSetpointOff, int32_t());
    MOCK_METHOD0(setLineCmdExtStart, int32_t());
    MOCK_METHOD0(setLineCmdExtStop, int32_t());
    MOCK_METHOD1(getLineAlarmStatus, int32_t(LineAlarmStatus& rStatus));
    MOCK_METHOD1(getLineProcessStatus, int32_t(LineProcessStatus& rStatus));
    MOCK_METHOD2(getLineMassflow, int32_t(const base::eFeederType feeder, float32_t& value));
    MOCK_METHOD2(getLineTotalizer, int32_t(const base::eFeederType feeder, float32_t& value));
    MOCK_METHOD1(getLineProductionTime, int32_t(uint32_t & value));
    MOCK_METHOD1(setLineRecipeName, int32_t(const std::string & value));
    MOCK_METHOD1(getLineRecipeName, int32_t(std::string & value));
    MOCK_METHOD1(setLineRecipeQMNumber, int32_t(const std::string & value));
    MOCK_METHOD1(getLineRecipeQMNumber, int32_t(std::string & value));
    MOCK_METHOD1(setLineRecipeSetpoint, int32_t(const float32_t value));
    MOCK_METHOD1(getLineRecipeSetpoint, int32_t(float32_t & value));
    MOCK_METHOD1(setLineRegeneratPercentage, int32_t(const float32_t value));
    MOCK_METHOD1(getLineRegeneratPercentage, int32_t(float32_t & value));
    MOCK_METHOD1(setLineMaxSetpoint, int32_t(const float32_t value));
    MOCK_METHOD1(getLineMaxSetpoint, int32_t(float32_t & value));
    MOCK_METHOD1(setLineSetpoint, int32_t(const float32_t value));
    MOCK_METHOD1(getLineSetpoint, int32_t(float32_t & value));
    MOCK_METHOD1(setLineRampStep, int32_t(const float32_t value));
    MOCK_METHOD1(getLineRampStep, int32_t(float32_t & value));
    MOCK_METHOD1(setLineRampScale, int32_t(const float32_t value));
    MOCK_METHOD1(getLineRampScale, int32_t(float32_t & value));
    MOCK_METHOD1(setLineRampDelay, int32_t(const uint32_t value));
    MOCK_METHOD1(getLineRampDelay, int32_t(uint32_t & value));
    MOCK_METHOD1(setLineRampHysteresis, int32_t(const float32_t value));
    MOCK_METHOD1(getLineRampHysteresis, int32_t(float32_t & value));
    MOCK_METHOD1(setLineMinTotband, int32_t(const float32_t value));
    MOCK_METHOD1(getLineMinTotband, int32_t(float32_t & value));
    MOCK_METHOD1(setLineSetpointLimitGrav, int32_t(const float32_t value));
    MOCK_METHOD1(getLineSetpointLimitGrav, int32_t(float32_t & value));
    MOCK_METHOD1(setLineFilterTime, int32_t(const uint32_t value));
    MOCK_METHOD1(getLineFilterTime, int32_t(uint32_t & value));
    MOCK_METHOD1(setLineExtLineSetpointInvers, int32_t(const BOOL value));
    MOCK_METHOD1(getLineExtLineSetpointInvers, int32_t(BOOL & value));
    MOCK_METHOD1(setLineServiceMode, int32_t(const BOOL value));
    MOCK_METHOD1(getLineServiceMode, int32_t(BOOL & value));

    MOCK_METHOD0(setDoseCmdLCSuspend, int32_t());
    MOCK_METHOD0(setDoseCmdLCResume, int32_t());
    MOCK_METHOD1(setDoseCmdLCPrepareWriteEEprom, int32_t(const int32_t index));
    MOCK_METHOD2(setDoseCmdLCEnableEEpromWriteProtection, int32_t(const int32_t index, const BOOL enable));
    MOCK_METHOD1(setDoseCmdLCRegisterSCL, int32_t(const int32_t index));
    MOCK_METHOD1(setDoseCmdLCSetAddress, int32_t(const int32_t index));
    MOCK_METHOD1(setDoseCmdLCClrAddress, int32_t(const int32_t index));

    // LC read
    MOCK_METHOD2(getDoseLCActualWeight, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(getDoseLCRawWeight, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(getDoseLCMeanWeight, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(getDoseLCActive, int32_t(const int32_t index, BOOL & value));
    MOCK_METHOD2(getDoseLCActualNoise, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD1(getDoseLCStatus, int32_t(std::string & value));

    // LC read & write
    MOCK_METHOD2(setDoseLCTaraWeight, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseLCTaraWeight, int32_t(const int32_t index, float32_t& value));
    MOCK_METHOD2(setDoseLCCorrectionFactor, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseLCCorrectionFactor, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseLCWaagenbereich, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseLCWaagenbereich, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD1(setDoseCmdStart, int32_t(const int32_t index));
    MOCK_METHOD1(setDoseCmdStop, int32_t(const int32_t index));

    // Calibration Start
    MOCK_METHOD1(setDoseCmdCalibStart, int32_t(const int32_t index));
    MOCK_METHOD1(setDoseCmdCalibStop, int32_t(const int32_t index));

    // Tarierung start
    MOCK_METHOD1(setDoseCmdTareStart, int32_t(const int32_t index));
    MOCK_METHOD1(setDoseCmdTareStop, int32_t(const int32_t index));

    // Refill start
    MOCK_METHOD1(setDoseCmdRefillStart, int32_t(const int32_t index));
    MOCK_METHOD1(setDoseCmdRefillStop, int32_t(const int32_t index));
    MOCK_METHOD2(setDoseCmdRefillRelease, int32_t(const int32_t index, const BOOL value));

    // Volumetric/gravimetrisch
    MOCK_METHOD1(setDoseCmdVolumetric, int32_t(const int32_t index));
    MOCK_METHOD1(setDoseCmdGravimetric, int32_t(const int32_t index));

    MOCK_METHOD1(setDoseCmdLine, int32_t(const int32_t index));
    MOCK_METHOD1(setDoseCmdLocal, int32_t(const int32_t index));

    MOCK_METHOD1(setDoseCmdLineModeEnable, int32_t(const int32_t index));
    MOCK_METHOD1(setDoseCmdLineModeDisable, int32_t(const int32_t index));

    // Verbrauch loeschen
    MOCK_METHOD1(setDoseCmdClearTotalizer, int32_t(const int32_t index));
    MOCK_METHOD1(setDoseCmdAlarmClear, int32_t(const int32_t index));
    MOCK_METHOD1(setDoseCmdAlarmAcknowledge, int32_t(const int32_t index));
    MOCK_METHOD2(getDoseMassflow, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(getDoseActualDoseperformance, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(getDoseTotalizer, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD3(getDoseAlarmStatus, int32_t(const int32_t index, const base::eAlarmLevel level, base::eAlarmErrorBits & pulStatus));
    MOCK_METHOD2(getDoseProcessStatus, int32_t(const int32_t index, ProcessStatus & pulStatus));
    MOCK_METHOD2(getDoseType, int32_t(const int32_t index, base::eDoseType & value));
    MOCK_METHOD2(getDoseActualDriveCommand, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(getDoseActualAgitatorValue, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(getDoseWbfBeltLoadMassflow, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(getDoseWbfBeltLoadActualVelocity, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(getDoseRefillState, int32_t(const int32_t index, RefillStatus & value));
    MOCK_METHOD2(getDoseActualRotationalSpeed, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(getDoseActualEncoderState, int32_t(const int32_t index, base::eEncoderState & value));
    MOCK_METHOD2(getDosePidGatefilterState, int32_t(const int32_t index, base::eGatefilter & value));
    MOCK_METHOD2(setDoseName, int32_t(const int32_t index, const std::string & value));
    MOCK_METHOD2(getDoseName, int32_t(const int32_t index, std::string & value));
    MOCK_METHOD2(setDoseQMNumber, int32_t(const int32_t index, const std::string & value));
    MOCK_METHOD2(getDoseQMNumber, int32_t(const int32_t index, std::string & value));
    MOCK_METHOD2(setDoseFeederType, int32_t(const int32_t index, const base::eFeederType value));
    MOCK_METHOD2(getDoseFeederType, int32_t(const int32_t index, base::eFeederType & value));
    MOCK_METHOD2(setDoseSetpoint, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseSetpoint, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseNominalPercentage, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseNominalPercentage, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseVolumetricChangeValue, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseVolumetricChangeValue, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD4(setDoseRefillLimit, int32_t(const int32_t index, const float32_t fMin, const float32_t fMax, const float32_t fMinMIn));
    MOCK_METHOD4(getDoseRefillLimit, int32_t(const int32_t index, float32_t & fMin, float32_t & fMax, float32_t & fMinMIn));
    MOCK_METHOD2(setDoseRefillLimitAlarm, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseRefillLimitAlarm, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseRefillLimitMin, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseRefillLimitMin, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseRefillLimitMax, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseRefillLimitMax, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseRefillTime, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseRefillTime, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseRefillSwitchDelay, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseRefillSwitchDelay, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseAlarmDriveCommandHigh, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseAlarmDriveCommandHigh, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseAlarmDriveCommandLow, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseAlarmDriveCommandLow, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseAlarmMassflowHigh, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseAlarmMassflowHigh, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseAlarmMassflowLow, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseAlarmMassflowLow, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseAlarmReactionDelay, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseAlarmReactionDelay, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseAlarmDosePerformance, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseAlarmDosePerformance, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseAlarmStartReactionDelay, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseAlarmStartReactionDelay, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseAlarmWbfMinBeltLoad, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseAlarmWbfMinBeltLoad, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseAlarmWbfMaxBeltLoad, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseAlarmWbfMaxBeltLoad, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseAlarmNoiseLimit, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2( getDoseAlarmNoiseLimit, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD4(setDoseAlarmCfgBit, int32_t(const int32_t index, const base::eAlarmCategory k, const base::eAlarmError iBit, const BOOL b));
    MOCK_METHOD4(getDoseAlarmCfgBit, int32_t(const int32_t index, const base::eAlarmCategory k, const base::eAlarmError iBit, BOOL & value));
    MOCK_METHOD2(setDoseCalibCfg, int32_t(const int32_t index, const base::calib::CalibType & value));
    MOCK_METHOD2(getDoseCalibCfg, int32_t(const int32_t index, base::calib::CalibType & value));
    MOCK_METHOD2(setDoseCalibActiveIndex, int32_t(const int32_t index, const int32_t & value));
    MOCK_METHOD2(getDoseCalibActiveIndex, int32_t(const int32_t index, int32_t & value));
    MOCK_METHOD2(setDoseCalibError, int32_t(const int32_t index, const base::calib::eCalibrationError & value));
    MOCK_METHOD2(getDoseCalibError, int32_t(const int32_t index, base::calib::eCalibrationError & value));
    MOCK_METHOD2(setDoseTaraError, int32_t(const int32_t index, const base::tara::eTaraError & value));
    MOCK_METHOD2(getDoseTaraError, int32_t(const int32_t index, base::tara::eTaraError & value));
    MOCK_METHOD2(setPidSampleInterval, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getPidSampleInterval, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseGatefilter, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseGatefilter, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseMassflowFilter, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseMassflowFilter, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDosePidPropGainGross, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDosePidPropGainGross, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDosePidPropGainFine, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDosePidPropGainFine, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDosePidPropGainSwitchGrossFine, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDosePidPropGainSwitchGrossFine, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDosePidIntegralGain, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDosePidIntegralGain, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseStartupRamp, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseStartupRamp, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseMaxDriveCommandChange, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseMaxDriveCommandChange, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD3(setDoseDosePerformance, int32_t(const int32_t index, const uint32_t timeindex, const float32_t value));
    MOCK_METHOD3(getDoseDosePerformance, int32_t(const int32_t index, const uint32_t timeindex, float32_t & value));
    MOCK_METHOD2(setDoseMaxSetpoint, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseMaxSetpoint, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseMaxRotationalSpeed, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseMaxRotationalSpeed, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseEncoderMonitor, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseEncoderMonitor, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseStartupDelay, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseStartupDelay, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseMassflowSteepness, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseMassflowSteepness, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseRegenerat, int32_t(const int32_t index, const BOOL value));
    MOCK_METHOD2(getDoseRegenerat, int32_t(const int32_t index, BOOL & value));
    MOCK_METHOD2(setDoseDriveCommandInverse, int32_t(const int32_t index, const BOOL value));
    MOCK_METHOD2(getDoseDriveCommandInverse, int32_t(const int32_t index, BOOL & value));
    MOCK_METHOD2(setDoseAgitatorSetpoint, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseAgitatorSetpoint, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseVolDriveCommandCorrection, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseVolDriveCommandCorrection, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseWbfBeltLoadSetpoint, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseWbfBeltLoadSetpoint, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseWbfReduction, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseWbfReduction, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseWbfWeighingLine, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseWbfWeighingLine, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseWbfWheelSize, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseWbfWheelSize, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseWbfTareDriveCommand, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseWbfTareDriveCommand, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseWbfTareTime, int32_t(const int32_t index, const uint32_t value));
    MOCK_METHOD2(getDoseWbfTareTime, int32_t(const int32_t index, uint32_t & value));
    MOCK_METHOD2(setDoseWbfBeltLoadVolSwitch, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseWbfBeltLoadVolSwitch, int32_t(const int32_t index, float32_t & value));
    MOCK_METHOD2(setDoseWbfVerwiegungFull, int32_t(const int32_t index, const BOOL value));
    MOCK_METHOD2(getDoseWbfVerwiegungFull, int32_t(const int32_t index, BOOL & value));
    MOCK_METHOD2(setDoseWbfMinDriveCommand, int32_t(const int32_t index, const float32_t value));
    MOCK_METHOD2(getDoseWbfMinDriveCommand, int32_t(const int32_t index, float32_t & value));


    void delegate()
    {
        ON_CALL(*this, getDoseMassflow(_, _)).WillByDefault(Invoke(
            [=](const int32_t index, float32_t& value) { value = _F32(index); return 0;  }));

        ON_CALL(*this, getMaxDoseCount(_)).WillByDefault(Invoke(
            [=](uint32_t& value) { value = 20U; return 0;  }));

        ON_CALL(*this, setDoseCmdStart(_)).WillByDefault(Invoke(
            [=](const int32_t index) { return 0;  }));

    };

};
 

namespace
{
    TEST(TestModbus, XmlFile)
    {
        base::xml::CGenericRemoteXml xml;

        auto result = xml.Load("C://EasyControl//config//RemoteTable.xml");
        EXPECT_TRUE(result);
        result = xml.Save("C://EasyControl//config//RemoteTable2.xml");
        EXPECT_TRUE(result);
    }

    TEST(TestModbus, Execute)
    {
        CMockDevice aMockDevice;
        aMockDevice.delegate();

        CMockRemote aMockRemote;
        aMockRemote.delegate();

        CRemoteHandler aHandler;

        aHandler.RegisterDeviceInterface(aMockDevice);
        aHandler.RegisterRemoteInterface(aMockRemote);

        std::string szFilename("C://EasyControl//config//RemoteTable.xml");

        auto result = aHandler.Open(szFilename);
        EXPECT_EQ(result, TRUE);

        EXPECT_CALL(aMockRemote, setDoseCmdStart(_)).Times(20);
        aHandler.Execute();

        EXPECT_TRUE(TRUE);
    }
}
