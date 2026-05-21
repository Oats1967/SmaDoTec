//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LCModuleImpl
///
/// @file   LCModuleImpl.h
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

#include <array>
#include <cassert>
#include "BASE/Base.h"
#include "BASE/include/LCProperty.h"
#include "BASE/include/LCPriorityList.h"
#include "BASE/include/MutexContainer.h"
#include "BASE/Task/public/ThreadModul.h"
#include "BASE/utils/public/Timer.h"
#include "BASE/Utils/public/Memory.h"
#include "BASE/include/SystemConfig.h"
#include "AdsClient/include/AdsTypen.h"
#include "DoseServer/include/LCModuleInterface.h"

namespace loadcell
{
class CLCModuleImpl :public ILCModuleInterface, public base::task::CThreadModul
{
private:
    base::CMutexContainer<base::LCProperty>    m_Container;
    base::CLCPriorityList                      m_PriorityList;
    std::array<uint32_t, _S32(base::LC_PRIORITY::LC_PRIORITY_MAX)> m_LastActiveCell;
    uint32_t             m_LastDeactiveCell;
    uint32_t		     m_LCDosierCount;
    uint32_t		     m_ulStatusCounter;
    base::utils::CTimer  m_aTimer;
    int32_t			     m_sStep;

private:
    void Init(void);
    void InitContainer(void);
    uint32_t StartUp1(void);
    int32_t  Run(void);
    void CheckDeactiveCells(void);
    BOOL CheckWeight(base::LC_PRIORITY sPriority, const uint32_t iStart, const uint32_t iCount);
    void CheckWeight(const int32_t index);
    BOOL SetEnable(const base::eLcType eType, int32_t);
    BOOL SetEnableTyp(int32_t);
    void InitCell(int32_t);
    base::LC_ERRORCODE MoveAddress(int32_t, int32_t iNew);
    base::LC_STATUS GetWeight(const int32_t index, base::LCProperty* pW) const;
    uint64_t getTimeMs();
    BOOL IsPresent(int32_t index) const;
    BOOL IsTypeDetectionDefined(int32_t index) const;
    void SetMaxItems(const base::CSystemConfig& rCfg);

    BOOL SetEnable(const base::eLcType, const int32_t index, base::LCProperty& cell);
    BOOL SetEnableTyp(const int32_t index,  base::LCProperty& cell);
    BOOL SetEnableAuto(const int32_t index, base::LCProperty& cell);
    BOOL SetEnableSingle(const int32_t index, base::LCProperty& cell);

private:
    static BOOL IsStartInit(const base::LCProperty& cell);
    static BOOL IsInitialized(const base::LCProperty& cell);
    static BOOL IsActive(const base::LCProperty& cell);
    static BOOL IsPresent(const base::LCProperty& cell);
    static BOOL IsTypeDetectionDefined(const base::LCProperty& cell);
    static void InitCell(const int32_t index, base::LCProperty& cell);


protected:
    int32_t execute(void) override;
    int32_t GetMaxCount(void) const;
    int32_t GetMaxItems(void) const override;

public:
    CLCModuleImpl();
    virtual ~CLCModuleImpl();

    void SetScaleRange(const int32_t i, const float32_t _fT) override;
    float32_t GetScaleRange(const int32_t i) const override;
    void SetTaraWeight(const int32_t i, const float32_t _fT) override;
    float32_t GetTaraWeight(const int32_t i) const override;
    void SetCorrectionFactor(const int32_t i, const float32_t _fC) override;
    float32_t GetCorrectionFactor(const int32_t i) const  override;
    void SetStatus(const int32_t index, const base::LC_STATUS s) override;
    base::LC_STATUS GetStatus(const int32_t index) const override;
    void SetPriority(const int32_t index, const base::LC_PRIORITY s) override;
    base::LC_PRIORITY GetPriority(const int32_t index) const override;
    BOOL SetNoiseControl(const int32_t index, const int16_t sN) override;
    void GetErrorCounter(const int32_t index, int16_t*) override;
    BOOL IsActive(const int32_t index) const override;
    BOOL IsInitialized(const int32_t index) const override;
    BOOL IsConfigured(const int32_t index) const override;
    BOOL IsEnabled(const int32_t index) const  override;
    BOOL IsStartInit(const int32_t i) const  override;


    base::LC_STATUS GetWeight(const int32_t i, float32_t* f, int16_t* sErrorCounter) const override;
    base::LC_STATUS GetWeight(const int32_t index, float32_t* f, uint64_t* pT, int16_t* sErrorCounter) const override;
    base::LC_STATUS GetRawWeight(const int32_t index, float32_t* f, uint64_t* pT, int16_t* sErrorCounter) const override;
    base::LC_STATUS GetRawWeight(const int32_t i, float32_t* f) const override;

    base::LC_ERRORCODE SetAddress(const int32_t i) override;
    base::LC_ERRORCODE ClrAddress(const int32_t index) override;
    base::LC_ERRORCODE PrepareWriteEEprom(const int32_t i) override;
    base::LC_ERRORCODE EnableEEpromWriteProtection(const int32_t index, const BOOL b) override;
    base::LC_ERRORCODE RegisterLC(const int32_t index) override;
    void SetDetectionType(const int32_t index, base::eLcTypeDetection eTyp) override;
    base::eLcTypeDetection GetDetectionType(int32_t index) const  override;
    void SetType(const int32_t index, base::eLcType eTyp) override;
    base::eLcType GetType(int32_t index) const  override;
    base::eLcSubType GetSubType(int32_t index) const override;
    void SetSubType(int32_t index, const base::eLcSubType eSubTyp) override;

    BOOL IsOpen(void) const override;
    base::LC_ERRORCODE Open(const base::CSystemConfig& rCfg) override;
    base::LC_ERRORCODE Close(void) override;
    void Suspend(void) override;
    void Resume(void) override;
    void CreateStatus(std::string& status) const override;

#ifdef __SIM_WEIGHT_GUI
    void  SetWeight(const int32_t index, const float32_t value) override;
#endif
};
//*******************************************************************************************
//*******************************************************************************************
inline uint64_t CLCModuleImpl :: getTimeMs()
{
    uint64_t ms = 0;
#if _DEBUG
    BOOL result = m_aTimer.Readms(ms);
    assert(result);
#else
    (void)m_aTimer.Readms(ms);
#endif
    return ms;
}
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsOpen(void) const
{
    return CThreadModul::IsOpen();
}
//*******************************************************************************************
//*******************************************************************************************
inline void CLCModuleImpl::Suspend(void)
{
    if ( (! IsSuspended()) && (IsOpen()))
    {
        CThreadModul::suspend();
    }
}
//*******************************************************************************************
//*******************************************************************************************
inline void CLCModuleImpl::Resume(void)
{
    if (IsSuspended())
    {
        CThreadModul::resume();
    }
}
//*******************************************************************************************
//*******************************************************************************************
inline int32_t CLCModuleImpl::GetMaxItems(void) const
{   return m_LCDosierCount; }
//*******************************************************************************************
//*******************************************************************************************
inline int32_t CLCModuleImpl::GetMaxCount(void) const
{    return _S32(m_Container.size()); }
//*******************************************************************************************
//*******************************************************************************************
inline void CLCModuleImpl::SetScaleRange(int32_t index, const float32_t _fW)
{
    assert(index >= 0 && index < m_Container.size());
    std::lock_guard<std::mutex> rLock(m_Container);
    auto& cell = m_Container.get(index);
    cell.fVollLast = _fW;
}
//*******************************************************************************************
//*******************************************************************************************
inline float32_t CLCModuleImpl::GetScaleRange(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    return m_Container.get(index).fVollLast;
}
//*******************************************************************************************
//*******************************************************************************************
inline void CLCModuleImpl::SetTaraWeight(int32_t index, const float32_t _fTara)
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    std::lock_guard<std::mutex> rLock(m_Container);
    auto& cell = m_Container.get(index);
    cell.fTaraWeight = _fTara;
}
//*******************************************************************************************
//*******************************************************************************************
inline float32_t CLCModuleImpl::GetTaraWeight(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    return m_Container.get(index).fTaraWeight;
}
//*******************************************************************************************
//*******************************************************************************************
inline void CLCModuleImpl::SetCorrectionFactor(int32_t index, const float32_t _fC)
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    std::lock_guard<std::mutex> rLock(m_Container);
    auto& cell = m_Container.get(index);
    cell.fCorrectionFactor = _fC;
}
//*******************************************************************************************
//*******************************************************************************************
inline float32_t CLCModuleImpl::GetCorrectionFactor(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    return m_Container.get(index).fCorrectionFactor;
}
//*******************************************************************************************
//*******************************************************************************************
inline void CLCModuleImpl::SetPriority(int32_t index, const base::LC_PRIORITY _sC)
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    std::lock_guard<std::mutex> rLock(m_Container);
    auto& cell = m_Container.get(index);
    cell.sPriority = _sC;
}
//*******************************************************************************************
//*******************************************************************************************
inline base::LC_PRIORITY CLCModuleImpl::GetPriority(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    return m_Container.get(index).sPriority;
}
//*******************************************************************************************
//*******************************************************************************************
inline void CLCModuleImpl::SetStatus(int32_t index, const base::LC_STATUS _sC)
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    std::lock_guard<std::mutex> rLock(m_Container);
    auto& cell = m_Container.get(index);
    cell.eStatus = _sC;
}
//*******************************************************************************************
//*******************************************************************************************
inline base::LC_STATUS CLCModuleImpl::GetStatus(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    return m_Container.get(index).eStatus;
}
//*******************************************************************************************
//*******************************************************************************************
inline void CLCModuleImpl::GetErrorCounter(int32_t index, int16_t* sC)
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    const auto& cell = m_Container.get(index);
    *sC = _S16(cell.sErrorCounter);
}
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsActive(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    const auto& cell = m_Container.get(index);
    return IsActive(cell);
}
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsConfigured(int32_t index) const
{
    auto lcdettype = GetDetectionType(index);
    return __ISLCDETECTIONENABLED(lcdettype);
}
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsEnabled(int32_t index) const
{
    auto lctype = GetType(index);
    return __ISLCENABLED(lctype);
}
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsActive(const base::LCProperty& cell)
{   return (cell.eStatus == base::LC_STATUS::LC_STATUS_OK);}
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsInitialized(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    return m_Container.get(index).bInitialized;
}
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsStartInit(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    return m_Container.get(index).bStartInit;
}
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsStartInit(const base::LCProperty& cell)
{   return cell.bStartInit;}
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsInitialized(const base::LCProperty& cell)
{   return cell.bInitialized; }
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsPresent(const base::LCProperty& cell)
{    return BOOL(cell.eType != base::eLcType::LC_EMPTY); }
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsPresent(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    const auto& cell = m_Container.get(index);
    return IsPresent(cell);
}
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsTypeDetectionDefined(const base::LCProperty& cell)
{
    return __ISLCDETECTIONENABLED(cell.eDetectionType);
}
//*******************************************************************************************
//*******************************************************************************************
inline BOOL CLCModuleImpl::IsTypeDetectionDefined(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    const auto& cell = m_Container.get(index);
    return IsTypeDetectionDefined(cell);
}
//*******************************************************************************************
//*******************************************************************************************
inline base::eLcTypeDetection CLCModuleImpl::GetDetectionType(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    const auto& cell = m_Container.get(index);
    return cell.eDetectionType;
}
//*******************************************************************************************
//*******************************************************************************************
inline void	  CLCModuleImpl::SetDetectionType(int32_t index, base::eLcTypeDetection eTyp)
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    std::lock_guard<std::mutex> rLock(m_Container);
    auto& cell = m_Container.get(index);
    cell.eDetectionType = eTyp;
}
//*******************************************************************************************
//*******************************************************************************************
inline base::eLcType CLCModuleImpl::GetType(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    const auto& cell = m_Container.get(index);
    return cell.eType;
}
//*******************************************************************************************
//*******************************************************************************************
inline void	  CLCModuleImpl::SetType(int32_t index, base::eLcType eTyp)
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    std::lock_guard<std::mutex> rLock(m_Container);
    auto& cell = m_Container.get(index);
    cell.eType = eTyp;
}
//*******************************************************************************************
//*******************************************************************************************
inline base::eLcSubType CLCModuleImpl::GetSubType(int32_t index) const
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    const auto& cell = m_Container.get(index);
    return cell.eSubType;
}
//*******************************************************************************************
//*******************************************************************************************
inline void	  CLCModuleImpl::SetSubType(int32_t index, base::eLcSubType eSubTyp)
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    std::lock_guard<std::mutex> rLock(m_Container);
    auto& cell = m_Container.get(index);
    cell.eSubType = eSubTyp;
}

//*******************************************************************************************
//*******************************************************************************************
inline base::LC_STATUS CLCModuleImpl::GetRawWeight(const int32_t index, float32_t* f) const
{
    assert(f);
    *f = 0.0f;
    assert(IsOpen());
    assert(IsInitialized(index));
#if 0
    if (!IsOpen())
    {
        return base::LC_STATUS::LC_STATUS_UNKNOWN;
    }
#endif
    const auto& cell = m_Container.get(index);
    *f = cell.fRawWeight;
    return 	cell.eStatus;
}


//------------------------------------------------------------------------------------
///  @brief   GetRawWeight
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CLCModuleImpl
///
///  @param[out]  sErrorCounter , int16_t *
///  @param[out] pT , uint64_t*
///  @param[out]  f , float32_t *
///  @param[in] n , int32_t
///  @return LC_STATUS
///
//------------------------------------------------------------------------------------
inline base::LC_STATUS CLCModuleImpl::GetRawWeight(const int32_t index, float32_t* f, uint64_t* pT, int16_t* sErrorCounter) const
{
    assert(f);
    assert(pT);
    assert(sErrorCounter);
    assert(IsOpen());
    assert(IsInitialized(index));
#if 0
    if (!IsOpen())
    {
        *f = 0.0f;
        *sErrorCounter = 0;
        *pT = 0L;
        return base::LC_STATUS::LC_STATUS_UNKNOWN;
    }
#endif
    //std::lock_guard<std::mutex> rLock(m_Container);
    const auto& cell = m_Container.get(index);
    *f = cell.fRawWeight;
    *pT = cell.ulTime;
    *sErrorCounter = _S16(cell.sErrorCounter);
    return 	cell.eStatus;
}



//------------------------------------------------------------------------------------
///  @brief   GetWeight
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CLCModuleImpl
///
///  @param[in]  sErrorCounter , int16_t *
///  @param[in]  f , float32_t *
///  @param[in] n , int32_t
///  @return LC_STATUS
///
//------------------------------------------------------------------------------------
inline base::LC_STATUS CLCModuleImpl::GetWeight(const int32_t index, float32_t* f, int16_t* sErrorCounter) const
{
    assert(f);
    *f = 0.0f;
    *sErrorCounter = 0;
    assert(IsOpen());
    assert(IsInitialized(index));
#if 0
    if (!IsOpen())
    {
        return base::LC_STATUS::LC_STATUS_UNKNOWN;
    }
#endif
    //std::lock_guard<std::mutex> rLock(m_Container);
    const auto& cell = m_Container.get(index);
    *sErrorCounter = _S16(cell.sErrorCounter);
    *f = cell.fWeight;
    return 	cell.eStatus;
}



//------------------------------------------------------------------------------------
///  @brief   GetWeight
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CLCModuleImpl
///
///  @param[in]  sErrorCounter , int16_t *
///  @param[in] pT , uint64_t*
///  @param[in]  f , float32_t *
///  @param[in] n , int32_t
///  @return LC_STATUS
///
//------------------------------------------------------------------------------------
inline base::LC_STATUS CLCModuleImpl::GetWeight(const int32_t index, float32_t* f, uint64_t* pT, int16_t* sErrorCounter) const
{
    assert(f);
    assert(pT);
    assert(sErrorCounter);
    assert(IsOpen());
    assert(IsInitialized(index));
#if 0
    if (!IsOpen())
    {
        *f = 0.0f;
        *sErrorCounter = 0;
        *pT = 0L;
        return base::LC_STATUS::LC_STATUS_UNKNOWN;
    }
#endif
    const auto& cell = m_Container.get(index);
    *f = cell.fWeight;
    *pT = cell.ulTime;
    *sErrorCounter = _S16(cell.sErrorCounter);
    return 	cell.eStatus;
}


//------------------------------------------------------------------------------------
///  @brief   GetWeight
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CLCModuleImpl
///
///  @param[in] pW , LCProperty*
///  @param[in] n , int32_t
///  @return LC_STATUS
///
//------------------------------------------------------------------------------------
inline base::LC_STATUS CLCModuleImpl::GetWeight(const int32_t index, base::LCProperty* pW) const
{
    assert(pW);
    assert(IsOpen());
    assert(IsInitialized(index));
#if 0
    if ( ! IsOpen())
    {
        base::utils::baseZeroMemory(pW, sizeof(*pW));
        return base::LC_STATUS::LC_STATUS_UNKNOWN;
    }
#endif
    //std::lock_guard<std::mutex> rLock(m_Container);
    const auto& cell = m_Container.get(index);
    *pW = cell;
    return 	cell.eStatus;
}


#ifdef __SIM_WEIGHT_GUI
inline void  CLCModuleImpl::SetWeight(const int32_t index, const float32_t value)
{
    assert(index >= 0 && index < _S32(m_Container.size()));
    std::lock_guard<std::mutex> rLock(m_Container);
    auto& cell = m_Container.get(index);
    cell.fWeight = value;
    cell.eStatus = base::LC_STATUS::LC_STATUS_OK;
}
#endif


};



