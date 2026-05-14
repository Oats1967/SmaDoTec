//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsDriveControl
///
/// @file   AdsDriveControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "AdsDriveControl.h"
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/include/AdsClient.h"
#include "BASE/Base.def"

//*********************************************************************************************************************
//*********************************************************************************************************************
static int32_t Dose_DSVSetIODriveErrorInputInv(int32_t id, BOOL bValue)
{
    return Dose_DSVSetIODriveErrorInput(id, !bValue);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
static int32_t Dose_DSVSetIODriveErrorPaddleInputInv(int32_t id, BOOL bValue)
{
    return Dose_DSVSetIODriveErrorPaddleInput(id, !bValue);
}

//*********************************************************************************************************************
//*********************************************************************************************************************
CAdsDriveControl::CAdsDriveControl(const int32_t index) : CBaseClass(index)
, m_DriveError      { index,  AdsClient_DoseGetDriveError,   Dose_DSVSetIODriveErrorInputInv }
, m_MotorLoad       { index,   AdsClient_DoseGetMotorLoad,   Dose_DSVSetIOMotorLoadInput }
, m_DriveErrorPaddle{ index,  AdsClient_DoseGetDriveErrorPaddle,   Dose_DSVSetIODriveErrorPaddleInputInv }
, m_DriveRelease    { index,  AdsClient_DoseSetDriveRelease,  Dose_DSVSetIODriveReleaseOutput }
, m_Start           { index,  AdsClient_DoseSetStart, Dose_DSVSetIOStartOutput }
, m_Operate         { index,  AdsClient_DoseSetOperate,   Dose_DSVSetIOOperateOutput }
, m_DriveCommand    { index,  AdsClient_DoseSetDriveCommand,   Dose_DSVSetActualDriveCommand }
, m_DecoderInput    { index, AdsClient_DoseGetEncoder,  Dose_DSVSetIOEncoderSpeedInput }
, m_PulseCounter    { index, AdsClient_DoseGetPulsCounter,  Dose_DSVSetIOPulseCounterInput }
, m_Container      { { &m_DriveError, &m_MotorLoad, &m_DriveErrorPaddle, &m_DriveRelease, &m_Start, &m_Operate, &m_DriveCommand, &m_DecoderInput, &m_PulseCounter} }
{
#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_DSVGetIODriveReleaseOutput(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIOOperateOutput(m_id, &bTemp);
    assert(bTemp == FALSE);
    float32_t fTemp = FALSE;
    Dose_DSVGetActualDriveCommand(m_id, &fTemp);
    assert(fTemp == 0.0F);
    Dose_DSVGetIODriveErrorInput(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIODriveErrorPaddleInput(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIOMotorLoadInput(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIOEncoderSpeedInput(m_id, &fTemp);
    assert(fTemp == 0.0F);
    uint32_t uTemp = 0;
    Dose_DSVGetIOPulseCounterInput(m_id, &uTemp);
    assert(uTemp == 0);

#endif
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CAdsDriveControl::Init()
{
    assert(! CBaseClass::IsInit());

    CBaseClass::Init();

    m_Container.Init();

    // Wegen inversen Funktion
    if ( ! m_DriveError.GetEnable())
    {
        Dose_DSVInitIODriveErrorInput(m_id, FALSE);
    }
    if ( ! m_DriveErrorPaddle.GetEnable())
    {
        Dose_DSVInitIODriveErrorPaddleInput(m_id, FALSE);
    }
    assert(m_DriveRelease.GetEnable());
    assert(m_DriveCommand.GetEnable());
    assert(m_Operate.GetEnable());


#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_DSVGetIODriveReleaseOutput(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIOOperateOutput(m_id, &bTemp);
    assert(bTemp == FALSE);
    float32_t fTemp = FALSE;
    Dose_DSVGetActualDriveCommand(m_id, &fTemp);
    assert(fTemp == 0.0F);
    Dose_DSVGetIOMotorLoadInput(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIOEncoderSpeedInput(m_id, &fTemp);
    assert(fTemp == 0.0F);
    uint32_t uTemp = 0;
    Dose_DSVGetIOPulseCounterInput(m_id, &uTemp);
    assert(uTemp == 0);
#endif
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CAdsDriveControl::Exit()
{
    assert(CBaseClass::IsInit());

    m_Container.Exit();

    // Wegen inversen Funktion
    Dose_DSVInitIODriveErrorInput(m_id, FALSE);
    Dose_DSVInitIODriveErrorPaddleInput(m_id, FALSE);

#if _DEBUG // Wenn assert -> ResetDoseItem vergessen
    BOOL bTemp = FALSE;
    Dose_DSVGetIODriveReleaseOutput(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIOOperateOutput(m_id, &bTemp);
    assert(bTemp == FALSE);
    float32_t fTemp = FALSE;
    Dose_DSVGetActualDriveCommand(m_id, &fTemp);
    assert(fTemp == 0.0F);
    Dose_DSVGetIOMotorLoadInput(m_id, &bTemp);
    assert(bTemp == FALSE);
    Dose_DSVGetIOEncoderSpeedInput(m_id, &fTemp);
    assert(fTemp == 0.0F);
    uint32_t uTemp = 0;
    Dose_DSVGetIOPulseCounterInput(m_id, &uTemp);
    assert(uTemp == 0);
#endif
    CBaseClass::Exit();
}
