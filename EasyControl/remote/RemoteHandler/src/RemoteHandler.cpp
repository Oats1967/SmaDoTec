//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implemenation of module RemoteHandler
///
/// @file   RemoteHandler.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/include/TypeEqual.h"
#include "BASE/Utils/public/BitOp.h"
#include "BASE/include/FieldTypeStack.h"
#include "remote/include/DeviceInterface.h"
#include "remote/include/RemoteInterface.h"
#include "Logger/include/Logger.h"
#include "RemoteHandler.h"


using namespace base;

#define __STACKFIELD(__a, __b) CFieldTypeStack __a((__b).m_pData, (__b).m_Elements * base::CGenericRemoteConfig::getSize((__b).m_FieldType), (__b).m_FieldType, (__b).m_bigEndian, (__b).m_Scale);

namespace remote
{
CRemoteHandler::CRemoteHandler() : CGenericRemoteHandler()
    , m_pDeviceInterface { nullptr }
    , m_pRemoteInterface { nullptr }
{}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::Open(const std::string& szFilename)
{
    auto result =  CGenericRemoteHandler::Open(szFilename);
    if (result)
    {
        if (m_MaskDescriptor.empty())
        {
            // Turn all bit son if no Mask defined
            m_MaskBits.allflags = ~0U;
        }
    }
    return result;
}
//***************************************************************************************************
//***************************************************************************************************
inline std::pair<uint32_t, uint32_t> CRemoteHandler::GetStartMaxcount(const BlockDescriptor& blockdescr)
{
    assert(m_pRemoteInterface);
    assert(blockdescr.m_Elements > 0 && blockdescr.m_Elements < GLOBALDOSEMAXCOUNT);
    uint32_t maxcount = 0;
    m_pRemoteInterface->getMaxDoseCount(maxcount);
    uint32_t startIx = (blockdescr.m_StartIx > 0) ? blockdescr.m_StartIx - 1U : 0;
    maxcount = std::min(maxcount, startIx + blockdescr.m_Elements);
    return std::move(std::make_pair(startIx, maxcount));
}
//***************************************************************************************************
//***************************************************************************************************
template <typename CallableType>
BOOL CRemoteHandler::CallObject(const BlockDescriptor& blockdescr, CallableType func)
{
    __STACKFIELD(aStack, blockdescr)

    if (!m_pRemoteInterface)
    {
        return FALSE;
    }
    auto el = GetStartMaxcount(blockdescr);
    for (auto index = el.first; index < el.second; index++)
    {
        func(_S32(index), aStack);
    }
    return TRUE;
}
//***************************************************************************************************
//***************************************************************************************************
template <typename CallableType>
BOOL CRemoteHandler::CallLineObject(const BlockDescriptor& blockdescr, CallableType func)
{
    __STACKFIELD(aStack, blockdescr)

    if (!m_pRemoteInterface)
    {
        return FALSE;
    }
    func( aStack);
    return TRUE;
}
//***************************************************************************************************
//***************************************************************************************************
template<typename T>
BOOL CRemoteHandler::getData(const BlockDescriptor& blockdescr, getfunc<T> get)
{
    return CallObject(blockdescr, [this, &get](const int32_t index, CFieldTypeStack& aStack)
        {
            T value;
            (m_pRemoteInterface->*get)(index, value);
            aStack.push(value);
        });
}
//***************************************************************************************************
//***************************************************************************************************
template<typename T>
BOOL CRemoteHandler::getDataEx(const BlockDescriptor& blockdescr, getfuncEx<T> get)
{
    return CallObject(blockdescr, [this, &get](const int32_t index, CFieldTypeStack& aStack)
        {
            T value;
            (m_pRemoteInterface->*get)(index, 0, value);
            aStack.push(value);
        });
}
//***************************************************************************************************
//***************************************************************************************************
template<typename T>
BOOL CRemoteHandler::getLineData(const BlockDescriptor& blockdescr, getLinefunc<T> get)
{
    assert(blockdescr.m_Elements == 1);
    return CallObject(blockdescr, [this, &get](const int32_t , CFieldTypeStack& aStack)
        {
            T value = static_cast<T>(0);
            (m_pRemoteInterface->*get)(value);
            aStack.push(value);
        });
}
//**********************************************************************************************
//***************************************************************************************************
template<typename T>
BOOL CRemoteHandler::setData(const BlockDescriptor& blockdescr, setfunc<T> set)
{
    return CallObject(blockdescr, [this, &set](const int32_t index, CFieldTypeStack& aStack)
        {
             (m_pRemoteInterface->*set)(index, aStack.pop<T>());
        });
}
//***************************************************************************************************
//***************************************************************************************************
template<typename T>
BOOL CRemoteHandler::setDataEx(const BlockDescriptor& blockdescr, setfuncEx<T> set)
{
    return CallObject(blockdescr, [this, &set](const int32_t index, CFieldTypeStack& aStack)
        {
             (m_pRemoteInterface->*set)(index, 0, aStack.pop<T>());
        });
}
//**********************************************************************************************
//***************************************************************************************************
template<typename T>
BOOL CRemoteHandler::setLineData(const BlockDescriptor& blockdescr, setLinefunc<T> set)
{
    assert(blockdescr.m_Elements == 1U);
    return CallLineObject(blockdescr, [this, &set](CFieldTypeStack& aStack)
        {
            (m_pRemoteInterface->*set)(aStack.pop<T>());
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WritePlaceHolder(const BlockDescriptor&)
{  return TRUE;}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteFeedingType(const BlockDescriptor& blockdescr)
{   return getData(blockdescr, &IRemoteInterface::getDoseType); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteSetpoint(const BlockDescriptor& blockdescr)
{   return getData(blockdescr, &IRemoteInterface::getDoseSetpoint); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteMassflow(const BlockDescriptor& blockdescr)
{   return getData(blockdescr,&IRemoteInterface::getDoseMassflow); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteActualWeight(const BlockDescriptor& blockdescr)
{   return getData(blockdescr,&IRemoteInterface::getDoseLCActualWeight); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteTotalizer(const BlockDescriptor& blockdescr)
{   return getData(blockdescr, &IRemoteInterface::getDoseTotalizer); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteRecipePercentage(const BlockDescriptor& blockdescr)
{   return getData<float32_t>(blockdescr, &IRemoteInterface::getDosePercentage); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteDeviation(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            float32_t setpoint;
            m_pRemoteInterface->getDoseSetpoint(index, setpoint);
            float32_t massflow;
            m_pRemoteInterface->getDoseMassflow(index, massflow);
            auto deviation = _F32(setpoint - massflow);
            aStack.push(deviation);
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteActualEncoderRotation(const BlockDescriptor& blockdescr)
{   return getData<float32_t>(blockdescr, &IRemoteInterface::getDoseActualRotationalSpeed); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteNominalEncoderRotation(const BlockDescriptor& blockdescr)
{    return getData<float32_t>(blockdescr, &IRemoteInterface::getDoseActualRotationalSpeed);}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteActualDosePerformance(const BlockDescriptor& blockdescr)
{   return getData<float32_t>(blockdescr, &IRemoteInterface::getDoseActualDoseperformance); }
//***************************************************************************************************
BOOL CRemoteHandler::WriteDosePerformance(const BlockDescriptor& blockdescr)
{  return getDataEx<float32_t>(blockdescr, &IRemoteInterface::getDoseDosePerformance); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteDriveCommand(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            float32_t drivecommand = 0.0F;
            m_pRemoteInterface->getDoseActualDriveCommand(index, drivecommand);
            drivecommand *= 100.0F;
            aStack.push(drivecommand);
        });

}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteTaraWeight(const BlockDescriptor& blockdescr)
{  return getData(blockdescr, &IRemoteInterface::getDoseLCTaraWeight); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteRefillTime(const BlockDescriptor& blockdescr)
{   return getData(blockdescr, &IRemoteInterface::getDoseRefillTime); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteRefillLimitAlarm(const BlockDescriptor& blockdescr)
{  return getData(blockdescr, &IRemoteInterface::getDoseRefillLimitAlarm); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteRefillLimitMin(const BlockDescriptor& blockdescr)
{  return getData(blockdescr, &IRemoteInterface::getDoseRefillLimitMin); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteRefillLimitMax(const BlockDescriptor& blockdescr)
{   return getData(blockdescr, &IRemoteInterface::getDoseRefillLimitMax); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteLCCorrectionFactor(const BlockDescriptor& blockdescr)
{   return getData(blockdescr, &IRemoteInterface::getDoseLCCorrectionFactor); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WritePidSampleInterval(const BlockDescriptor& blockdescr)
{  return getData(blockdescr, &IRemoteInterface::getPidSampleInterval); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WritePidGain(const BlockDescriptor& blockdescr)
{   return getData(blockdescr, &IRemoteInterface::getDosePidIntegralGain); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteGateFilter(const BlockDescriptor& blockdescr)
{   return getData(blockdescr, &IRemoteInterface::getDoseGatefilter); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteMassflowFilter(const BlockDescriptor& blockdescr)
{    return getData(blockdescr, &IRemoteInterface::getDoseMassflowFilter); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteAlarmNoiseLimit(const BlockDescriptor& blockdescr)
{  return getData(blockdescr, &IRemoteInterface::getDoseAlarmNoiseLimit); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WritePaddleSpeed(const BlockDescriptor& blockdescr)
{   return getData(blockdescr, &IRemoteInterface::getDoseAgitatorSetpoint);}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteRefillFeederSpeed(const BlockDescriptor& blockdescr)
{   return getData(blockdescr, &IRemoteInterface::getDoseRefillFeederSpeed);}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteEmptyFeederSpeed(const BlockDescriptor& blockdescr)
{    return getData(blockdescr, &IRemoteInterface::getDoseEmptyFeederSpeed); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteAlarmStatus(const BlockDescriptor& blockdescr)
{
    assert(! m_AlarmStatus.empty());
    if (m_AlarmStatus.empty())
    {
        LOGERROR("No description for WriteAlarmStatus!");
        return FALSE;
    }
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            uint32_t status = CreateAlarmStatusBits(index, *m_pRemoteInterface, m_AlarmStatus);
            aStack.push(status);
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteProcessStatus(const BlockDescriptor& blockdescr)
{
    assert(! m_ProcessStatus.empty());
    if (m_ProcessStatus.empty())
    {
        LOGERROR("No description for WriteProcessStatus!");
        return FALSE;
    }
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            base::ProcessStatus processbits;
            m_pRemoteInterface->getDoseProcessStatus(_S32(index), processbits);
            auto status = CreateStatusBits(processbits, m_ProcessStatus);
            aStack.push(status);
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteLineProcessStatus(const BlockDescriptor& blockdescr)
{
    if (m_LineProcessStatus.empty())
    {
        LOGERROR("No description for LineProcessStatus!");
        return FALSE;
    }
    assert(blockdescr.m_Elements == 1);
    return CallObject(blockdescr, [this](const int32_t , CFieldTypeStack& aStack)
        {
            base::LineProcessStatus processbits;
            m_pRemoteInterface->getLineProcessStatus(processbits);
            uint32_t status = CreateLineProcessStatusBits(processbits, m_LineProcessStatus );
            aStack.push(status);
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteLineAlarmStatus(const BlockDescriptor& blockdescr)
{
    if ( m_LineAlarmStatus.empty())
    {
       LOGERROR("No description for LineAlarmStatus!")
       return FALSE;
     }
    assert(blockdescr.m_Elements == 1);
    return CallObject(blockdescr, [this](const int32_t , CFieldTypeStack& aStack)
        {
            base::LineAlarmStatus alarmbits;
            m_pRemoteInterface->getLineAlarmStatus(alarmbits);
            const uint32_t status = CreateLineAlarmStatusBits(alarmbits, m_LineAlarmStatus );
            aStack.push(status);
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteCounter(const BlockDescriptor& blockdescr)
{
    __STACKFIELD(aStack, blockdescr)
    __STACKFIELD(bStack, blockdescr)

    assert(blockdescr.m_Elements == 1);
    auto el = GetStartMaxcount(blockdescr);
    for (auto index = el.first; index < el.second; index++)
    {
        auto counter = aStack.pop<uint32_t>();
        bStack.push(++counter);
    }
    return TRUE;
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteCalibCount(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            base::calib::CalibType calibType;
            m_pRemoteInterface->getDoseCalibCfg(index, calibType);
            aStack.push(calibType.count);
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::getCalibDriveCommand(const int32_t ix, const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this, &ix](const int32_t index, CFieldTypeStack& aStack)
        {
            base::calib::CalibType calibType;
            m_pRemoteInterface->getDoseCalibCfg(index, calibType);
            aStack.push(calibType.fDriveCommand[ix]);
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::getCalibDosePerformance(const int32_t ix, const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this, &ix](const int32_t index, CFieldTypeStack& aStack)
        {
            base::calib::CalibType calibType;
            m_pRemoteInterface->getDoseCalibCfg(index, calibType);
            aStack.push(calibType.fDosePerformance[ix]);
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::getCalibDuration(const int32_t ix, const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this, &ix](const int32_t index, CFieldTypeStack& aStack)
        {
            base::calib::CalibType calibType;
            m_pRemoteInterface->getDoseCalibCfg(index, calibType);
            aStack.push(calibType.lMeasureTime[ix]);
        });
}
#define WRITECALIB(_a) \
    static_assert(((_a)-1) < base::calib::MAXCALIBITEMS, "Error calibindex !"); \
    BOOL CRemoteHandler::WriteCalibDriveCommand ##_a(const BlockDescriptor& blockdescr) \
    {  return getCalibDriveCommand((_a) - 1, blockdescr); } \
    BOOL CRemoteHandler::WriteCalibDosePerformance ##_a(const BlockDescriptor& blockdescr) \
    {  return getCalibDosePerformance((_a)-1, blockdescr); } \
    BOOL CRemoteHandler::WriteCalibDuration ##_a(const BlockDescriptor& blockdescr) \
    {  return getCalibDuration((_a)-1, blockdescr); }

WRITECALIB(1)
WRITECALIB(2)
WRITECALIB(3)
WRITECALIB(4)
WRITECALIB(5)
WRITECALIB(6)
WRITECALIB(7)
WRITECALIB(8)
WRITECALIB(9)
WRITECALIB(10)
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteLineSetpoint(const BlockDescriptor& blockdescr)
{   return getLineData(blockdescr, &IRemoteInterface::getLineSetpoint); }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadControlCmd(const BlockDescriptor& blockdescr)
{
    assert(!m_ProcessControl.empty());
    if (m_ProcessControl.empty())
    {
        LOGERROR("No description for ReadControlCmd!")
        return FALSE;
    }
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto cb = CreateControlBits(m_ProcessControl, aStack.pop<uint32_t>());
            if (cb.flags.start && (!cb.flags.stop))
            {
                m_pRemoteInterface->setDosePBStart(index);
            }
            else if (cb.flags.stop && (!cb.flags.start))
            {
                m_pRemoteInterface->setDosePBStop(index);
            }
            if (cb.flags.alarmclear || cb.flags.alarmack)
            {
                m_pRemoteInterface->setDosePBAlarmClear(index);
            }
            if (cb.flags.grav && (!cb.flags.vol))
            {
                m_pRemoteInterface->setDosePBGravimetric(index);
            }
            if (cb.flags.vol && (!cb.flags.grav))
            {
                m_pRemoteInterface->setDosePBVolumetric(index);
            }
            if ((cb.flags.refillReleaseEnableActive && cb.flags.refillReleaseEnable) ||
                (cb.flags.refillReleaseDisableActive && (!cb.flags.refillReleaseDisable)))
            {
                m_pRemoteInterface->setDoseRefillRelease(index, TRUE);
            }
            else if (cb.flags.refillReleaseDisableActive && (cb.flags.refillReleaseDisable) ||
                (cb.flags.refillReleaseEnableActive && (!cb.flags.refillReleaseEnable)))
            {
                m_pRemoteInterface->setDoseRefillRelease(index, FALSE);
            }
            else if (cb.flags.refillReleaseToggleActive)
            {
                m_pRemoteInterface->setDoseRefillReleaseTrigger(index, cb.flags.refillReleaseToggle);
            }
            if (cb.flags.refillStart && (!cb.flags.refillStop))
            {
                m_pRemoteInterface->setDosePBRefillStart(index);
            }
            if (cb.flags.refillStop && (!cb.flags.refillStart))
            {
                m_pRemoteInterface->setDosePBRefillStop(index);
            }
            if (cb.flags.calib)
            {
                m_pRemoteInterface->setDosePBCalibStart(index);
            }
            if (cb.flags.tare)
            {
                m_pRemoteInterface->setDosePBTareStart(index);
            }
            if (cb.flags.emptyfeederStart && (!cb.flags.emptyfeederStop))
            {
                m_pRemoteInterface->setDosePBEmptyFeederStart(index);
            }
            if (cb.flags.emptyfeederStop && (!cb.flags.emptyfeederStart))
            {
                m_pRemoteInterface->setDosePBEmptyFeederStop(index);
            }
            if (cb.flags.cleartot)
            {
                m_pRemoteInterface->setDosePBClearTotalizer(index);
            }
            if (cb.flags.localmode && (!cb.flags.linemode))
            {
                m_pRemoteInterface->setDosePBLocal(index);
            }
            else if (cb.flags.linemode && (!cb.flags.localmode))
            {
                m_pRemoteInterface->setDosePBInline(index);
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadPlaceHolder(const BlockDescriptor&)
{  return TRUE; }
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdStart(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                base::ProcessStatus ulStatus;
                m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
                if (!(ulStatus.flags.started || ulStatus.flags.running))
                {
                    m_pRemoteInterface->setDosePBStart(index);
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdStop(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                base::ProcessStatus ulStatus;
                m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
                if (ulStatus.flags.started || ulStatus.flags.running)
                {
                    m_pRemoteInterface->setDosePBStop(index);
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdTotClear(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                m_pRemoteInterface->setDosePBClearTotalizer(index);
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdAlarmClear(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                base::ProcessStatus ulStatus;
                m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
                if ( ulStatus.flags.alarmactive )
                {
                    m_pRemoteInterface->setDosePBAlarmClear(index);
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdAlarmAck(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                base::ProcessStatus ulStatus;
                m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
                if (ulStatus.flags.alarmactive)
                {
                    m_pRemoteInterface->setDosePBAlarmAcknowledge(index);
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdCalibrate(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                base::ProcessStatus ulStatus;
                m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
                if ( ! ulStatus.flags.calibrateActive)
                {
                    m_pRemoteInterface->setDosePBCalibStart(index);
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdTare(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                base::ProcessStatus ulStatus;
                m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
                if (!ulStatus.flags.tareActive)
                {
                    m_pRemoteInterface->setDosePBTareStart(index);
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdRefillOff(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                base::ProcessStatus ulStatus;
                m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
                if ( ulStatus.flags.refillActive )
                {
                    m_pRemoteInterface->setDosePBRefillStop(index);
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdRefillOn(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                base::ProcessStatus ulStatus;
                m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
                if ( ! ulStatus.flags.refillActive)
                {
                    m_pRemoteInterface->setDosePBRefillStart(index);
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdInLine(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                base::ProcessStatus ulStatus;
                m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
                if ( ! ulStatus.flags.lineMode)
                {
                    m_pRemoteInterface->setDosePBInline(index);
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdLocal(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                base::ProcessStatus ulStatus;
                m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
                if ( ulStatus.flags.lineMode )
                {
                    m_pRemoteInterface->setDosePBLocal(index);
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdGrav(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                base::ProcessStatus ulStatus;
                m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
                if ( ! ulStatus.flags.gravimetric)
                {
                    m_pRemoteInterface->setDosePBGravimetric(index);
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdVol(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            auto newvalue = aStack.pop<uint8_t>();
            if (newvalue)
            {
                base::ProcessStatus ulStatus;
                m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
                if ( ulStatus.flags.gravimetric )
                {
                    m_pRemoteInterface->setDosePBVolumetric(index);
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDoseCmdRefillRelease(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            BOOL enable = BOOL(aStack.pop<uint8_t>() != 0);

            base::ProcessStatus ulStatus;
            m_pRemoteInterface->getDoseProcessStatus(index, ulStatus);
            if ( BOOL(ulStatus.flags.refillRelease) != enable)
            {
                m_pRemoteInterface->setDoseRefillRelease(index, enable);
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadCalibCount(const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            const uint32_t count = aStack.pop<uint32_t>();

            base::calib::CalibType calibType;
            m_pRemoteInterface->getDoseCalibCfg(index, calibType);
            if (count != calibType.count)
            {
                calibType.count = count;
                m_pRemoteInterface->setDoseCalibCfg(index, calibType);
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::setCalibDriveCommand(const int32_t ix, const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this, &ix](const int32_t index, CFieldTypeStack& aStack)
        {
            const auto newvalue = aStack.pop<float32_t>();

            base::calib::CalibType calibType;
            m_pRemoteInterface->getDoseCalibCfg(index, calibType);
            base::CComparison<float32_t> comp(calibType.fDriveCommand[ix], newvalue);
            if ( ! comp.isEqual())
            {
                calibType.fDriveCommand[ix] = newvalue;
                m_pRemoteInterface->setDoseCalibCfg(index, calibType);
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::setCalibDosePerformance(const int32_t ix, const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this, &ix](const int32_t index, CFieldTypeStack& aStack)
        {
            const auto newvalue = aStack.pop<float32_t>();

            base::calib::CalibType calibType;
            m_pRemoteInterface->getDoseCalibCfg(index, calibType);
            base::CComparison<float32_t> comp(calibType.fDosePerformance[ix], newvalue);
            if (!comp.isEqual())
            {
                calibType.fDosePerformance[ix] = newvalue;
                m_pRemoteInterface->setDoseCalibCfg(index, calibType);
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::setCalibDuration(const int32_t ix, const BlockDescriptor& blockdescr)
{
    return CallObject(blockdescr, [this, &ix](const int32_t index, CFieldTypeStack& aStack)
        {
            const auto newvalue = aStack.pop<uint32_t>();

            base::calib::CalibType calibType;
            m_pRemoteInterface->getDoseCalibCfg(index, calibType);
            base::CComparison<uint32_t> comp(calibType.lMeasureTime[ix], newvalue);
            if ( ! comp.isEqual() )
            {
                calibType.lMeasureTime[ix] = newvalue;
                m_pRemoteInterface->setDoseCalibCfg(index, calibType);
            }
        });
}
#define READCALIB(_a) \
    static_assert(((_a)-1) < base::calib::MAXCALIBITEMS, "Error calibindex !"); \
    BOOL CRemoteHandler::ReadCalibDriveCommand ##_a(const BlockDescriptor& blockdescr) \
    {  return setCalibDriveCommand((_a) - 1, blockdescr); } \
    BOOL CRemoteHandler::ReadCalibDosePerformance ##_a(const BlockDescriptor& blockdescr) \
    {  return setCalibDosePerformance((_a)-1, blockdescr); } \
    BOOL CRemoteHandler::ReadCalibDuration ##_a(const BlockDescriptor& blockdescr) \
    {  return setCalibDuration((_a)-1, blockdescr); }

READCALIB(1)
READCALIB(2)
READCALIB(3)
READCALIB(4)
READCALIB(5)
READCALIB(6)
READCALIB(7)
READCALIB(8)
READCALIB(9)
READCALIB(10)

//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadSetpoint(const BlockDescriptor& blockdescr)
{
    if (! m_MaskBits.flags.bMaskSetpoint)
    {
        LOGDEBUG("Setpoint must be changed locally!");
        return FALSE;
    }
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
            {
                base::ProcessStatus status;
                m_pRemoteInterface->getDoseProcessStatus(index, status);
                if ( ! status.flags.lineMode)
                {
                    m_pRemoteInterface->setDoseSetpoint(index, aStack.pop<float32_t>());
                }
            });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadRecipePercentage(const BlockDescriptor& blockdescr)
{
    if ( ! m_MaskBits.flags.bMaskPercentage)
    {
        LOGDEBUG("TaraWeight must be changed locally!");
        return FALSE;
    }
    return CallObject(blockdescr, [this](const int32_t index, CFieldTypeStack& aStack)
        {
            base::ProcessStatus status;
            m_pRemoteInterface->getDoseProcessStatus(index, status);
            if ( status.flags.lineMode )
            {
                m_pRemoteInterface->setDosePercentage(index, aStack.pop<float32_t>());
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadTaraWeight(const BlockDescriptor& blockdescr)
{  
    if (! m_MaskBits.flags.bMaskTaraWeight)
    {
        LOGDEBUG("TaraWeight must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseLCTaraWeight);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadEmptyFeederSpeed(const BlockDescriptor& blockdescr)
{
    if (!m_MaskBits.flags.bMaskEmpyFeederSpeed)
    {
        LOGDEBUG("TaraWeight must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseEmptyFeederSpeed);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadDosePerformance(const BlockDescriptor& blockdescr)
{    
    if (!m_MaskBits.flags.bMaskDosePerformance)
    {
        LOGDEBUG("DosePerformance must be changed locally!");
        return FALSE;
    }
    return setDataEx(blockdescr, &IRemoteInterface::setDoseDosePerformanceSave);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadRefillLimitMin(const BlockDescriptor& blockdescr)
{ 
    if (!m_MaskBits.flags.bMaskRefillMinLimit)
    {
        LOGDEBUG("RefillMinLimit must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseRefillLimitMin); 
} 
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadRefillLimitMax(const BlockDescriptor& blockdescr)
{   
    if ( ! m_MaskBits.flags.bMaskRefillMaxLimit)
    {
        LOGDEBUG("RefillMaxLimit must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseRefillLimitMax); 
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadRefillLimitAlarm(const BlockDescriptor& blockdescr)
{   
    if ( ! m_MaskBits.flags.bMaskRefillAlarmLimit)
    {
        LOGDEBUG("RefillAlarmLimit must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseRefillLimitAlarm);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadLCCorrectionFactor(const BlockDescriptor& blockdescr)
{    
    if ( ! m_MaskBits.flags.bMaskCorrectionFactor )
    {
        LOGDEBUG("CorrectionFactor must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseLCCorrectionFactor);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadAlarmMassflowHigh(const BlockDescriptor& blockdescr)
{   
    if ( ! m_MaskBits.flags.bMaskMassflowHigh)
    {
        LOGDEBUG("MassflowHigh must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseAlarmMassflowHigh);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadAlarmMassflowLow(const BlockDescriptor& blockdescr)
{   
    if (!m_MaskBits.flags.bMaskMassflowLow)
    {
        LOGDEBUG("MassflowLow must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseAlarmMassflowLow); 
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadAlarmDriveCommandHigh(const BlockDescriptor& blockdescr)
{ 
    if ( ! m_MaskBits.flags.bMaskDriveCommandHigh)
    {
        LOGDEBUG("DriveCommandHigh must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseAlarmDriveCommandHigh);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadAlarmDriveCommandLow(const BlockDescriptor& blockdescr)
{  
    if ( ! m_MaskBits.flags.bMaskDriveCommandLow )
    {
        LOGDEBUG("DriveCommandLow must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseAlarmDriveCommandLow);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadRefillTime(const BlockDescriptor& blockdescr)
{  
    if ( ! m_MaskBits.flags.bMaskRefillTime )
    {
        LOGDEBUG("RefillTime must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseRefillTime); 
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadAlarmReactionDelay(const BlockDescriptor& blockdescr)
{  
    if ( ! m_MaskBits.flags.bMaskAlarmReactionDelay )
    {
        LOGDEBUG("AlarmDelay must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseAlarmReactionDelay);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadAlarmStartReactionDelay(const BlockDescriptor& blockdescr)
{
    if (!m_MaskBits.flags.bMaskAlarmStartReactionDelay)
    {
        LOGDEBUG("AlarmStartReactionDelay must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseAlarmStartReactionDelay);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadStartupDelay(const BlockDescriptor& blockdescr)
{  
    if ( ! m_MaskBits.flags.bMaskStartupDelay )
    {
        LOGDEBUG("StartDelay must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseStartupDelay); 
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadSampleInterval(const BlockDescriptor& blockdescr)
{  
    if ( ! m_MaskBits.flags.bMaskSampleInterval)
    {
        LOGDEBUG("SampleInterval must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setPidSampleInterval); 
}

//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadPidGain(const BlockDescriptor& blockdescr)
{  
    if (!m_MaskBits.flags.bMaskPropGain)
    {
        LOGDEBUG("PidPropGain must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDosePidIntegralGain); 
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadGateFilter(const BlockDescriptor& blockdescr)
{    return setData(blockdescr, &IRemoteInterface::setDoseGatefilter);}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadMassflowFilter(const BlockDescriptor& blockdescr)
{  
    if ( ! m_MaskBits.flags.bMaskMassflowFilter )
    {
        LOGDEBUG("MassflowFilter must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseMassflowFilter); 
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadAlarmNoiseLimit(const BlockDescriptor& blockdescr)
{  
    if ( ! m_MaskBits.flags.bMaskAlarmNoise )
    {
        LOGDEBUG("AlarmNoise must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseAlarmNoiseLimit); 
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadPaddleSpeed(const BlockDescriptor& blockdescr)
{   
    if ( ! m_MaskBits.flags.bMaskPaddleSpeed)
    {
        LOGDEBUG("PaddleSpeed must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseAgitatorSetpoint); 
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadRefillFeederSpeed(const BlockDescriptor& blockdescr)
{
    if (!m_MaskBits.flags.bMaskRefillFeederSpeed)
    {
        LOGDEBUG("RefillFeederSpeed must be changed locally!");
        return FALSE;
    }
    return setData(blockdescr, &IRemoteInterface::setDoseRefillFeederSpeed);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadLineSetpoint(const BlockDescriptor& blockdescr)
{
    if (!m_MaskBits.flags.bMaskLineSetpoint)
    {
        LOGDEBUG("LineSetpoint must be changed locally!");
        return FALSE;
    }
    return setLineData(blockdescr, &IRemoteInterface::setLineSetpoint); 
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadLinePercentage(const BlockDescriptor& blockdescr)
{
    if (!m_MaskBits.flags.bMaskLinePercentage)
    {
        LOGDEBUG("LinePercentage must be changed locally!");
        return FALSE;
    }
    return setLineData(blockdescr, &IRemoteInterface::setLinePercentage); 
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadLineCmdStart(const BlockDescriptor& blockdescr)
{
    return CallLineObject(blockdescr, [this](CFieldTypeStack& aStack)
        {
            base::LineProcessStatus rStatus;
            m_pRemoteInterface->getLineProcessStatus(rStatus);
            if ( ! rStatus.flags.linestart)
            {
                auto newvalue = aStack.pop<uint8_t>();
                if (newvalue)
                {
                    m_pRemoteInterface->setLinePBExtStart();
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadLineCmdStop(const BlockDescriptor& blockdescr)
{
    return CallLineObject(blockdescr, [this](CFieldTypeStack& aStack)
        {
            base::LineProcessStatus rStatus;
            m_pRemoteInterface->getLineProcessStatus(rStatus);
            if (rStatus.flags.linestart)
            {
                auto newvalue = aStack.pop<uint8_t>();
                if (newvalue)
                {
                    m_pRemoteInterface->setLinePBExtStop();
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadLineCmdTotClear(const BlockDescriptor& blockdescr)
{
    return CallLineObject(blockdescr, [this](CFieldTypeStack& aStack)
        {
            base::LineProcessStatus rStatus;
            m_pRemoteInterface->getLineProcessStatus(rStatus);
            if (rStatus.flags.linestart)
            {
                auto newvalue = aStack.pop<uint8_t>();
                if (newvalue)
                {
                    m_pRemoteInterface->setLinePBClearTotalizer();
                }
            }
        });
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadLineControl(const BlockDescriptor& blockdescr)
{
    if ( m_LineProcessControl.empty())
    {
        LOGDEBUG("No description for ReadLineControl!");
        return FALSE;
    }
    assert(blockdescr.m_Elements == 1U);
    return CallLineObject(blockdescr, [this](CFieldTypeStack& aStack)
        {
            auto cb = CreateLineControlBits(m_LineProcessControl, aStack.pop<uint32_t>());

            // Start / Stop
            if (cb.flags.Start && (!cb.flags.Stop))
            {
                m_pRemoteInterface->setLinePBExtStart();
            }
            else if (cb.flags.Stop && (!cb.flags.Start))
            {
                m_pRemoteInterface->setLinePBExtStop();
            }
            else {}

            if (cb.flags.TotalizerClr)
            {
                m_pRemoteInterface->setLinePBClearTotalizer();
            }

            if (cb.flags.Alarm0)
            {
                m_pRemoteInterface->setLineCmdAlarm(0);
            }
            if (cb.flags.Alarm1)
            {
                m_pRemoteInterface->setLineCmdAlarm(1);
            }
            if (cb.flags.Alarm2)
            {
                m_pRemoteInterface->setLineCmdAlarm(2);
            }
            if (cb.flags.Alarm3)
            {
                m_pRemoteInterface->setLineCmdAlarm(3);
            }
            if (cb.flags.AlarmClr03)
            {
                m_pRemoteInterface->setLinePBAlarm03Quit();
            }
            if (cb.flags.AlarmClr1)
            {
                m_pRemoteInterface->setLinePBAlarm1Quit();
            }
#if 0
            if (cb.flags.ExtLineSetpointActive)
            {
                m_pRemoteInterface->setLineExtLineSetpoint(cb.flags.ExtLineSetpoint);
            }
            if (cb.flags.ExtLineSetpointInvActive)
            {
                m_pRemoteInterface->setLineExtLineSetpointInvers(cb.flags.ExtLineSetpointInv);
            }
#endif
        });
}

//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadMask(const BlockDescriptor& blockdescr)
{
    __STACKFIELD(aStack, blockdescr)
     
    m_MaskBits = CreateMaskBits(m_MaskDescriptor, aStack.pop<uint64_t>());
    return TRUE;
}

//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::ReadData(uint32_t baseaddress, uint32_t offset, uint32_t length, uint8_t* pData)
{
    auto result = m_pDeviceInterface->ReadData(baseaddress, offset, length, pData);
    return (result == 0);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CRemoteHandler::WriteData(uint32_t baseaddress, uint32_t offset, uint32_t length, const uint8_t* pData)
{
    auto result = m_pDeviceInterface->WriteData(baseaddress, offset, length, pData);
    return (result == 0);
}
//***************************************************************************************************
//***************************************************************************************************
CRemoteHandler::ControlBits CRemoteHandler::CreateControlBits(const ProcessControlDescriptor& desc, const uint32_t value)
{
    ControlBits cb;

    for (const auto& rCmd : desc)
    {
        switch (rCmd.first)
        {
        case RemoteProcessControl::eStart:
            cb.flags.start = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eStop:
            cb.flags.stop = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eAlarmClr:
            cb.flags.alarmclear = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eAlarmAck:
            cb.flags.alarmack = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eModeGravimetric:
            cb.flags.grav = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eModeVolumetric:
            cb.flags.vol = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eRefillReleaseDisable:
            cb.flags.refillReleaseDisable = ISBIT(value, rCmd.second);
            cb.flags.refillReleaseDisableActive = true;
            break;
        case RemoteProcessControl::eRefillReleaseEnable:
            cb.flags.refillReleaseEnable = ISBIT(value, rCmd.second);
            cb.flags.refillReleaseEnableActive = true;
            break;
        case RemoteProcessControl::eRefillReleaseToggle:
            cb.flags.refillReleaseToggle = ISBIT(value, rCmd.second);
            cb.flags.refillReleaseToggleActive = true;
            break;
        case RemoteProcessControl::eRefillStart:
            cb.flags.refillStart = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eRefillStop:
            cb.flags.refillStop = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eCalibrateStart:
            cb.flags.calib = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eTareStart:
            cb.flags.tare = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eEmptyFeederStart:
            cb.flags.emptyfeederStart = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eEmptyFeederStop:
            cb.flags.emptyfeederStop = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eTotalizerClr:
            cb.flags.cleartot = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eLocalMode:
            cb.flags.localmode = ISBIT(value, rCmd.second);
            break;
        case RemoteProcessControl::eLineMode:
            cb.flags.linemode = ISBIT(value, rCmd.second);
            break;
        default:
            break;
        }
    }
    return cb;
}
//***************************************************************************************************
//***************************************************************************************************
CRemoteHandler::LineControlBits CRemoteHandler::CreateLineControlBits(const LineProcessControlDescriptor& desc, const uint32_t value)
{
    LineControlBits cb;

    for (const auto& rCmd : desc)
    {
        switch (rCmd.first)
        {
        case RemoteLineProcessControl::eAlarm0:
            cb.flags.Alarm0 = ISBIT(value, rCmd.second);
            break;
        case RemoteLineProcessControl::eAlarm1:
            cb.flags.Alarm1 = ISBIT(value, rCmd.second);
            break;
        case RemoteLineProcessControl::eAlarm2:
            cb.flags.Alarm2 = ISBIT(value, rCmd.second);
            break;
        case RemoteLineProcessControl::eAlarm3:
            cb.flags.Alarm3 = ISBIT(value, rCmd.second);
            break;
        case RemoteLineProcessControl::eAlarmClr03:
            cb.flags.AlarmClr03 = ISBIT(value, rCmd.second);
            break;
        case RemoteLineProcessControl::eAlarmClr1:
            cb.flags.AlarmClr1 = ISBIT(value, rCmd.second);
            break;
        case RemoteLineProcessControl::eStart:
            cb.flags.Start = ISBIT(value, rCmd.second);
            break;
        case RemoteLineProcessControl::eStop:
            cb.flags.Stop = ISBIT(value, rCmd.second);
            break;
        case RemoteLineProcessControl::eTotalizerClr:
            cb.flags.TotalizerClr = ISBIT(value, rCmd.second);
            break;
        case RemoteLineProcessControl::eExtLineSetpoint:
            cb.flags.ExtLineSetpointActive = true;
            cb.flags.ExtLineSetpoint = ISBIT(value, rCmd.second);
            break;
        case RemoteLineProcessControl::eExtLineSetpointInv:
            cb.flags.ExtLineSetpointInvActive = true;
            cb.flags.ExtLineSetpointInv = ISBIT(value, rCmd.second);
            break;
        default:
            break;
        }
    }
    return cb;
}
//***************************************************************************************************
//***************************************************************************************************
template <typename T>
CRemoteHandler::MaskBits CRemoteHandler::CreateMaskBits(const MaskDescriptor& desc, const T value)
{
    MaskBits cb;

    cb.allflags = 0;
    for (const auto& rCmd : desc)
    {
        switch (rCmd.first)
        {
        case RemoteMask::eMaskSetpoint:
            cb.flags.bMaskSetpoint = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskDosePerformance:
            cb.flags.bMaskDosePerformance = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskRefillMinLimit:
            cb.flags.bMaskRefillMinLimit = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskRefillMaxLimit:
            cb.flags.bMaskRefillMaxLimit = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskRefillTime:
            cb.flags.bMaskRefillTime = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskRefillAlarmLimit:
            cb.flags.bMaskRefillAlarmLimit = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskMassflowHigh:
            cb.flags.bMaskMassflowHigh = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskMassflowLow:
            cb.flags.bMaskMassflowLow = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskDriveCommandHigh:
            cb.flags.bMaskDriveCommandHigh = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskDriveCommandLow:
            cb.flags.bMaskDriveCommandLow = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskAlarmReactionDelay:
            cb.flags.bMaskAlarmReactionDelay = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskAlarmStartReactionDelay:
            cb.flags.bMaskAlarmStartReactionDelay = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskStartupDelay:
            cb.flags.bMaskStartupDelay = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskSampleInterval:
            cb.flags.bMaskSampleInterval = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskPropGain:
            cb.flags.bMaskPropGain = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskMassflowFilter:
            cb.flags.bMaskMassflowFilter = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskAlarmNoise:
            cb.flags.bMaskAlarmNoise = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskPercentage:
            cb.flags.bMaskPercentage = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskTaraWeight:
            cb.flags.bMaskTaraWeight = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskCorrectionFactor:
            cb.flags.bMaskCorrectionFactor = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskPaddleSpeed:
            cb.flags.bMaskPaddleSpeed = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskLineStart:
            cb.flags.bMaskLineStart = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskLineStop:
            cb.flags.bMaskLineStop = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskLineTotClr:
            cb.flags.bMaskLineTotClr = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskLineSetpoint:
            cb.flags.bMaskLineSetpoint = ISBIT(value, rCmd.second);
            break;

        case RemoteMask::eMaskLinePercentage:
            cb.flags.bMaskLinePercentage = ISBIT(value, rCmd.second);
            break;

        default:
            assert(FALSE);
            break;
        }
    }
    return cb;
}
//***************************************************************************************************
//***************************************************************************************************
uint32_t CRemoteHandler::CreateAlarmStatusBits(const int32_t index, IRemoteInterface& m_rRemoteInterface, const base::AlarmStatusDescriptor& desc)
{
    uint32_t status = 0L;

    base::eAlarmErrorBits alarmbits;
    m_rRemoteInterface.getDoseAlarmStatus(index, base::eAlarmLevel::eAlarm, alarmbits);

    for (const auto& rCmd : desc)
    {
        switch (rCmd.first)
        {
        case RemoteAlarmStatus::eFeederEmpty:
            SETBIT(status, rCmd.second, alarmbits.flags.doseempty);
            break;

        case RemoteAlarmStatus::eRefillTimeout:
            SETBIT(status, rCmd.second, alarmbits.flags.refilltimeout);
            break;

        case RemoteAlarmStatus::eRefillBelowMin:
        {
            float32_t netweight = 0.0F;
            float32_t refillmin = 0.0F;
            m_rRemoteInterface.getDoseLCActualWeight(index, netweight);
            m_rRemoteInterface.getDoseRefillLimitMin(index, refillmin);
            const bool refillminalarm = netweight < refillmin;
            SETBIT(status, rCmd.second, refillminalarm);
        }
        break;

        case RemoteAlarmStatus::eMassflowHigh:
            SETBIT(status, rCmd.second, alarmbits.flags.massflowhigh);
            break;

        case RemoteAlarmStatus::eMassflowLow:
            SETBIT(status, rCmd.second, alarmbits.flags.massflowlow);
            break;

        case RemoteAlarmStatus::eDriveCommandHigh:
            SETBIT(status, rCmd.second, alarmbits.flags.drivecommandhigh);
            break;

        case RemoteAlarmStatus::eDriveCommandLow:
            SETBIT(status, rCmd.second, alarmbits.flags.drivecommandlow);
            break;

        case RemoteAlarmStatus::eLoadCellHigh:
            SETBIT(status, rCmd.second, alarmbits.flags.loadcellhigh);
            break;

        case RemoteAlarmStatus::eLoadCellError:
            SETBIT(status, rCmd.second, alarmbits.flags.loadcellerror);
            break;

        case RemoteAlarmStatus::eLoadCellNoise:
            SETBIT(status, rCmd.second, alarmbits.flags.loadcellnoise);
            break;

        case RemoteAlarmStatus::eDosePerformanceHigh:
            SETBIT(status, rCmd.second, alarmbits.flags.doseperformancehigh);
            break;

        case RemoteAlarmStatus::eDosePerformanceLow:
            SETBIT(status, rCmd.second, alarmbits.flags.doseperformancelow);
            break;

        case RemoteAlarmStatus::eEncoderError:
            SETBIT(status, rCmd.second, alarmbits.flags.encodererror);
            break;

        case RemoteAlarmStatus::eExtAlarm:
            SETBIT(status, rCmd.second, alarmbits.flags.extalarm | alarmbits.flags.icperror);
            break;


        case RemoteAlarmStatus::eAlarmStop:
            SETBIT(status, rCmd.second, alarmbits.flags.alarmstop);
            break;

        case RemoteAlarmStatus::eDriveError:
            SETBIT(status, rCmd.second, alarmbits.flags.driveerroralarm);
            break;

        case RemoteAlarmStatus::eMotorLoad:
            SETBIT(status, rCmd.second, alarmbits.flags.motorloadalarm);
            break;

        case RemoteAlarmStatus::eDriveErrorPaddle:
            SETBIT(status, rCmd.second, alarmbits.flags.driveerrorpaddlealarm);
            break;

        case RemoteAlarmStatus::eDriveErrorRefillFeeder:
            SETBIT(status, rCmd.second, alarmbits.flags.driveerrorrefillfeederalarm);
            break;

        // Line alarms
        case RemoteAlarmStatus::eLineShutDown:
            SETBIT(status, rCmd.second, alarmbits.flags.lineshutdown);
            break;

        case RemoteAlarmStatus::eLineSyncError:
            SETBIT(status, rCmd.second, alarmbits.flags.linesyncerror);
            break;
        case RemoteAlarmStatus::eLineUPSError:
            SETBIT(status, rCmd.second, alarmbits.flags.lineupsalarm);
            break;

        case RemoteAlarmStatus::eLineextalarm:
            SETBIT(status, rCmd.second, alarmbits.flags.lineextalarm);
            break;

        case RemoteAlarmStatus::eLinecustomeralarm:
            SETBIT(status, rCmd.second, alarmbits.flags.linecustomeralarm);
            break;


        default:
            break;
        }
    }
    return status;
}
//***************************************************************************************************
//***************************************************************************************************
uint32_t CRemoteHandler::CreateStatusBits(const base::ProcessStatus& processbits, const base::ProcessStatusDescriptor& desc)
{
    uint32_t status = 0L;

    for (const auto& rCmd : desc)
    {
        switch (rCmd.first)
        {
        case RemoteProcessStatus::eAlarmActive:
            SETBIT(status, rCmd.second, processbits.flags.alarmactive);
            break;
        case RemoteProcessStatus::eCalibrateRun:
            SETBIT(status, rCmd.second, processbits.flags.calibrateActive);
            break;
        case RemoteProcessStatus::eFeederRun:
            SETBIT(status, rCmd.second, processbits.flags.running | processbits.flags.started);
            break;
        case RemoteProcessStatus::eEmptyFeederRun:
            SETBIT(status, rCmd.second, processbits.flags.emptyfeederActive);
            break;
        case RemoteProcessStatus::eTaraRun:
            SETBIT(status, rCmd.second, processbits.flags.tareActive);
            break;
        case RemoteProcessStatus::eGravimetric:
            SETBIT(status, rCmd.second, processbits.flags.gravimetric);
            break;
        case RemoteProcessStatus::eVolumetric:
            SETBIT(status, rCmd.second, !processbits.flags.gravimetric);
            break;
        case RemoteProcessStatus::eLineMode:
            SETBIT(status, rCmd.second, processbits.flags.lineMode);
            break;
        case RemoteProcessStatus::eLocalMode:
            SETBIT(status, rCmd.second, !processbits.flags.lineMode);
            break;
        case RemoteProcessStatus::eDriveRelease:
            SETBIT(status, rCmd.second, processbits.flags.release);
            break;
        case RemoteProcessStatus::eNoAlarmActive:
            SETBIT(status, rCmd.second, !processbits.flags.alarmactive);
            break;
        case RemoteProcessStatus::eRefillActive:
            SETBIT(status, rCmd.second, processbits.flags.refillActive);
            break;
        case RemoteProcessStatus::eRefillDisable:
            SETBIT(status, rCmd.second, !processbits.flags.refillRelease);
            break;
        case RemoteProcessStatus::eRefillEnable:
            SETBIT(status, rCmd.second, processbits.flags.refillRelease);
            break;
        case RemoteProcessStatus::eWeightExceedsMax:
            SETBIT(status, rCmd.second, !processbits.flags.LCActive);
            break;
        default:
            break;
        }
    }
    return status;
}
//***************************************************************************************************
//***************************************************************************************************
uint32_t CRemoteHandler::CreateLineProcessStatusBits(const base::LineProcessStatus& processbits, const base::LineProcessStatusDescriptor& desc)
{
    uint32_t status = 0L;

    for (const auto& rCmd : desc)
    {
        switch (rCmd.first)
        {
        case RemoteLineProcessStatus::eLineStarted:
            SETBIT(status, rCmd.second, processbits.flags.linestart);
            break;
        case RemoteLineProcessStatus::eExtLineSetpoint:
            SETBIT(status, rCmd.second, processbits.flags.StateExtlineSetpoint);
            break;
        case RemoteLineProcessStatus::eExtLineSetpointInv:
            SETBIT(status, rCmd.second, processbits.flags.StateExtLineInvSetpoint);
            break;
        default:
            break;
        }
    }
    return status;
}
//***************************************************************************************************
//***************************************************************************************************
uint32_t CRemoteHandler::CreateLineAlarmStatusBits(const base::LineAlarmStatus& alarmbits, const base::LineAlarmStatusDescriptor& desc)
{
    uint32_t status = 0L;

    for (const auto& rCmd : desc)
    {
        switch (rCmd.first)
        {
        case RemoteLineAlarmStatus::eAlarm0:
            SETBIT(status, rCmd.second, alarmbits.flags.alarm0);
            break;
        case RemoteLineAlarmStatus::eAlarm1:
            SETBIT(status, rCmd.second, alarmbits.flags.alarm1);
            break;
        case RemoteLineAlarmStatus::eAlarm2:
            SETBIT(status, rCmd.second, alarmbits.flags.alarm2);
            break;
        case RemoteLineAlarmStatus::eAlarm3:
            SETBIT(status, rCmd.second, alarmbits.flags.alarm3);
            break;
        default:
            break;
        }
    }
    return status;
}



};
