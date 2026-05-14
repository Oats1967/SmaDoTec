//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteInterface
///
/// @file   RemoteInterface.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "BASE/Utils/public/Memory.h"
#include <cassert>
#include "types.h"
#include "RemoteInterface.h"
#include "Logger.h"

namespace remote
{
static const uint16_t kMaxCount = 6U;

class CRemoteImpl : public CRemoteInterface
{
    float32_t m_LineSetPoint;
    BOOL m_CmdStart[kMaxCount];
    BOOL m_CmdStop[kMaxCount];
    BOOL m_CmdTotalizer[kMaxCount];
    BOOL m_CmdClearAlarm[kMaxCount];
    BOOL m_CmdCalibrate[kMaxCount];
    BOOL m_CmdTare[kMaxCount];
    BOOL m_CmdStopRefill[kMaxCount];
    BOOL m_CmdStartRefill[kMaxCount];
    BOOL m_CmdVolumetric[kMaxCount];
    BOOL m_CmdLocal[kMaxCount];
    BOOL m_CmdLine[kMaxCount];
    BOOL m_CmdGravimetric[kMaxCount];
    BOOL m_CmdAlarmAcknowledge[kMaxCount];
    float32_t m_Massflow[kMaxCount];
    float32_t m_TotalizerValue[kMaxCount];
    float32_t m_SetPoint[kMaxCount];
    float32_t m_Weight[kMaxCount];
    float32_t m_QMaxfactor[kMaxCount];
    float32_t m_DriveCommand[kMaxCount];
    float32_t m_ActualDoseperformance[kMaxCount];
    DoseType m_Type[kMaxCount];
    float32_t m_RefillMinMin[kMaxCount];
    float32_t m_RefillMin[kMaxCount];
    float32_t m_RefillMax[kMaxCount];
    ProcessStatus m_ProcessStatus[kMaxCount];
    AlarmStatus m_AlarmStatus[kMaxCount];
    float32_t m_NominalPercentage[kMaxCount];

public:
    CRemoteImpl() : CRemoteInterface()
    {
        m_LineSetPoint = 0.0F;
        base::utils::baseZeroMemory(m_CmdStart);
        base::utils::baseZeroMemory(m_CmdStop);
        base::utils::baseZeroMemory(m_CmdTotalizer);
        base::utils::baseZeroMemory(m_CmdClearAlarm);
        base::utils::baseZeroMemory(m_CmdCalibrate);
        base::utils::baseZeroMemory(m_CmdTare);
        base::utils::baseZeroMemory(m_CmdStopRefill);
        base::utils::baseZeroMemory(m_CmdStartRefill);
        base::utils::baseZeroMemory(m_Massflow);
        base::utils::baseZeroMemory(m_TotalizerValue);
        base::utils::baseZeroMemory(m_SetPoint);
        base::utils::baseZeroMemory(m_Weight);
        base::utils::baseZeroMemory(m_QMaxfactor);
        base::utils::baseZeroMemory(m_DriveCommand);
        base::utils::baseZeroMemory(m_Type);
        base::utils::baseZeroMemory(m_RefillMinMin);
        base::utils::baseZeroMemory(m_RefillMin);
        base::utils::baseZeroMemory(m_RefillMax);
        base::utils::baseZeroMemory(m_ProcessStatus);
        base::utils::baseZeroMemory(m_AlarmStatus);
        base::utils::baseZeroMemory(m_NominalPercentage);
    }

    ~CRemoteImpl() override = default;
    CRemoteImpl(const CRemoteImpl&) = delete;
    CRemoteImpl& operator=(const CRemoteImpl&) = delete;

    int32_t getMaxDoseCount(uint32_t& value)
    {
        value = kMaxCount;
        return 0;
    }

    int32_t setDoseCmdStart(const int32_t index, const BOOL value) override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdStart[index] = value;
        std::string ret = (value) ? "TRUE" : "FALSE";
        LOGDEBUG("setDoseCmdStart, index = " << index << " value = " << ret);
        return 0;
    }

    int32_t setDoseCmdStop(const int32_t index, const BOOL value)  override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdStop[index] = value;
        return 0;
    }

    int32_t setDoseCmdClearTotalizer(const int32_t index, const BOOL value)  override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdTotalizer[index] = value;
        return 0;
    }

    int32_t setDoseCmdClearAlarm(const int32_t index, const BOOL value) override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdClearAlarm[index] = value;
        return 0;
    }

    int32_t setDoseCmdCalibrate(const int32_t index, const BOOL value) override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdCalibrate[index] = value;
        return 0;
    }

    int32_t setDoseCmdTare(const int32_t index, const BOOL value) override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdTare[index] = value;
        return 0;
    }


    int32_t setDoseCmdStopRefill(const int32_t index, const BOOL value) override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdStopRefill[index] = value;
        return 0;
    }

    int32_t setDoseCmdStartRefill(const int32_t index, const BOOL value) override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdStartRefill[index] = value;
        return 0;
    }

    int32_t setDoseCmdVolumetric(const int32_t index, const BOOL value) override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdVolumetric[index] = value;
        return 0;
    }

    int32_t setDoseCmdLine(const int32_t index, const BOOL value) override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdLine[index] = value;
        return 0;
    }

    int32_t setDoseCmdLocal(const int32_t index, const BOOL value) override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdLocal[index] = value;
        return 0;
    }

    int32_t setDoseCmdAlarmAcknowledge(const int32_t index, const BOOL value) override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdAlarmAcknowledge[index] = value;
        return 0;
    }

    int32_t setDoseCmdGravimetric(const int32_t index, const BOOL value) override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        m_CmdGravimetric[index] = value;
        return 0;
    }

    //-----------------------------------------------------------------------------------------------
    // read values
    int32_t getLineSetPoint(float32_t& value) override
    {
        value = m_LineSetPoint;
        LOGDEBUG("getLineSetPoint, value = " << value);
        return 0;
    }


    int32_t getDoseNominalPercentage(const int32_t index, float32_t& value) override
    {
        assert(index >= 0 && index < _S32(kMaxCount));
        value = m_NominalPercentage[index];
        LOGDEBUG("getDoseNominalPercentage, index = " << index << " value = " << value);
        return 0;
    }

     int32_t getDoseMassflow(const int32_t index, float32_t& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_Massflow[index];
         LOGDEBUG("getDoseMassflow, index = " << index << " value = " << value);
         return 0;
     }

     int32_t getDoseTotalizer(const int32_t index, float32_t& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_TotalizerValue[index];
         LOGDEBUG("getDoseTotalizer, index = " << index << " value = " << value);
         return 0;
     }

     int32_t getDoseWeight(const int32_t index, float32_t& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_Weight[index];
         return 0;
     }

     int32_t getDoseActualDoseFactor(const int32_t index, float32_t& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_ActualDoseperformance[index];
         return 0;
     }

     
     int32_t getDoseAlarmStatus(const int32_t index, AlarmStatus& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_AlarmStatus[index];
         LOGDEBUG("getDoseAlarmStatus, index = " << index << " value = " << value);
         return 0;
     }

     int32_t getDoseProcessStatus(const int32_t index, ProcessStatus& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_ProcessStatus[index];
         LOGDEBUG("getDoseProcessStatus, index = " << index << " value = " << value);
         return 0;
     }


     int32_t getDoseType(const int32_t index, DoseType& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_Type[index];
         LOGDEBUG("getDoseType, index = " << index << " value = " << value);
         return 0;
     }

     
     int32_t getDoseDriveCommand(const int32_t index, float32_t& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_DriveCommand[index];
         LOGDEBUG("getDoseDriveCommand, index = " << index << " value = " << value);
         return 0;
     }


     //-----------------------------------------------------------------------------------------------
     // read & write values
     int32_t setDoseSetPoint(const int32_t index, const float32_t value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         m_SetPoint[index] = value;
         LOGDEBUG("setDoseSetPoint, index = " << index << " value = " << value);
         return 0;
     }

     int32_t getDoseSetPoint(const int32_t index, float32_t& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_SetPoint[index];
         LOGDEBUG("getDoseSetPoint, index = " << index << " value = " << value);
         return 0;
     }


     int32_t setDoseQMaxfactor(const int32_t index, const float32_t value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         m_DriveCommand[index] = value;
         LOGDEBUG("setDoseQMaxfactor, index = " << index << " value = " << value);
         return 0;
     }

     int32_t getDoseQMaxfactor(const int32_t index, float32_t& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_DriveCommand[index];
         LOGDEBUG("getDoseQMaxfactor, index = " << index << " value = " << value);
         return 0;
     }


     int32_t setDoseGatefilter(const int32_t index, const float32_t value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         m_DriveCommand[index] = value;
         LOGDEBUG("setDoseGatefilter, index = " << index << " value = " << value);
         return 0;
     }

     int32_t getDoseGatefilter(const int32_t index, float32_t& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_DriveCommand[index];
         LOGDEBUG("getDoseGatefilter, index = " << index << " value = " << value);
         return 0;
     }

     int32_t setDoseBeltLoadSetPoint(const int32_t index, const float32_t value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         m_DriveCommand[index] = value;
         LOGDEBUG("setDoseBeltLoadSetPoint, index = " << index << " value = " << value);
         return 0;
     }

     int32_t getDoseBeltLoadSetPoint(const int32_t index, float32_t& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_DriveCommand[index];
         LOGDEBUG("getDoseBeltLoadSetPoint, index = " << index << " value = " << value);
         return 0;
     }


     int32_t setDoseVolumetricChangeValue(const int32_t index, const float32_t value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         m_DriveCommand[index] = value;
         LOGDEBUG("setDoseVolumetricChangeValue, index = " << index << " value = " << value);
         return 0;
     }

     int32_t getDoseVolumetricChangeValue(const int32_t index, float32_t& value) override
     {
         assert(index >= 0 && index < _S32(kMaxCount));
         value = m_DriveCommand[index];
         LOGDEBUG("getDoseVolumetricChangeValue, index = " << index << " value = " << value);
         return 0;
     }


      int32_t setDoseRefillLimitMin(const int32_t index, const float32_t value)  override
      {
          assert(index >= 0 && index < _S32(kMaxCount));
          m_DriveCommand[index] = value;
          LOGDEBUG("setDoseRefillLimitMin, index = " << index << " value = " << value);
          return 0;
      }

      int32_t getDoseRefillLimitMin(const int32_t index, float32_t& value) override
      {
          assert(index >= 0 && index < _S32(kMaxCount));
          value = m_DriveCommand[index];
          LOGDEBUG("getDoseRefillLimitMin, index = " << index << " value = " << value);
          return 0;
      }

      int32_t setDoseRefillLimitMax(const int32_t index, const float32_t value) override
      {
          assert(index >= 0 && index < _S32(kMaxCount));
          m_DriveCommand[index] = value;
          LOGDEBUG("setDoseRefillLimitMax, index = " << index << " value = " << value);
          return 0;
      }

      int32_t getDoseRefillLimitMax(const int32_t index, float32_t& value) override
      {
          assert(index >= 0 && index < _S32(kMaxCount));
          value = m_DriveCommand[index];
          LOGDEBUG("getDoseRefillLimitMax, index = " << index << " value = " << value);
          return 0;
      }


      int32_t setDoseRefillLimitAlarm(const int32_t index, const float32_t value) override
      {
          assert(index >= 0 && index < _S32(kMaxCount));
          m_DriveCommand[index] = value;
          LOGDEBUG("setDoseRefillLimitAlarm, index = " << index << " value = " << value);
          return 0;
      }

      int32_t getDoseRefillLimitAlarm(const int32_t index, float32_t& value) override
      {
          assert(index >= 0 && index < _S32(kMaxCount));
          value = m_DriveCommand[index];
          LOGDEBUG("getDoseRefillLimitAlarm, index = " << index << " value = " << value);
          return 0;
      }



};

};


