//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsLaumasCellSim
///
/// @file   AdsLaumasCellSim.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#ifdef __SIM_LAUMAS

#include "EasyControlVer.h"
#include "AdsClient/src/private/AdsLaumasCellSim.h"
#include "Logger/include/Logger.h"

#define C_CR		_C(0x0D)


#define STEPWEIGHT(_a) _F32( ((_a) *10.0F / c_Fullload ))


extern int32_t Dose_DSVGetRefillState(const int32_t index, BOOL* state);
extern int32_t Dose_DSVGetRelease(const int32_t index, BOOL* state);
extern int32_t Dose_DSVGetActualSetpoint(const int32_t index, float32_t* setpoint);


static IAdsLoadCellInterface* g_pAdsLaumasLoadcellSim = nullptr;


//*****************************************************************************************************************
//*****************************************************************************************************************
CAdsLaumasLoadcellSim::CAdsLaumasLoadcellSim() : m_ix{ 0 }
, m_ScrewBroken{ FALSE }
, m_Jump{ FALSE }
{
	m_retval[0] = ADS_MPM_OK;
	m_retval[1] = ADS_MPM_TIMEOUT;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellSim::Init(const int32_t )
{
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellSim::Init()
{
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::Close(const int32_t)
{
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::Close()
{
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::Reset(const int32_t)
{
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::Reset()
{
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::GetSensorActive(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::SetPPM(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::GetFullLoad(const int32_t index, float32_t* fFullLoad)
{
	*fFullLoad = c_Fullload;
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellSim::SetNoiseControlWeight(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::SetAddress(const int32_t index, int32_t iNew)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iNew);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::GetVersion(int32_t* sSW)
{
	*sSW = 1;
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::SetNoiseControl(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::SetWindowTime(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellSim::SetWindowRes(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::DeleteReference(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::SetReference(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::SetTara(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::ClrTara(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::DeleteZero(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::SetZero(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::EEpromVerify(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::EEpromReadCode(const int32_t index, WORD* w)
{
	BASE_UNUSED_VARIABLE(index);
	*w = WORD(1);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::RegisterLC(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::EnableEEpromWriteProtection(const int32_t index, BOOL bEnable)
{
	BASE_UNUSED_VARIABLE(bEnable);
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::PrepareWriteEEprom(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellSim::GetSubType(const int32_t index, base::eLcSubType* pType)
{
	assert(pType);
	BASE_UNUSED_VARIABLE(index);
	*pType = base::eLcSubType::eLDU68P1;
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellSim::GetZeroIndex(const int32_t index, int32_t* value)
{
	assert(value);
	BASE_UNUSED_VARIABLE(index);
	*value = 0;
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::IsPresent(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::GetWeight(const int32_t index, float32_t* f)
{
	if (index >= GLOBALDOSEMAXCOUNT)
	{
		*f = 0;
		return 0;
	}
	auto& ref = m_SimData[index];
	if (!ref.init)
	{
		ref.init = true;
		ref.weight = STEPWEIGHT(c_MaxWeight);;
	}
#if 1
	BOOL state = FALSE;
	Dose_DSVGetRefillState(index, &state);

	if (state)
	{
		ref.weight += STEPWEIGHT(0.5F);
		ref.m_aTimer.Stop();
	}
	else
	{
		Dose_DSVGetRelease(index, &state);
		if (state)
		{
			if (!ref.m_aTimer.IsStarted())
			{
				ref.m_aTimer.Start();
				ref.m_aTimer.Reads(ref.lasttime);
			}
			else
			{
				uint32_t t = 0;
				ref.m_aTimer.Reads(t);
				if (t > ref.lasttime)
				{
					if (m_Jump)
					{
						m_Jump = 0;
						ref.weight = STEPWEIGHT(20);
					}
					else if ( ! m_ScrewBroken)
					{
						uint32_t dt = t - ref.lasttime;
						float32_t setpoint;
						Dose_DSVGetActualSetpoint(index, &setpoint);
						setpoint /= 3600.0F;
						float32_t loss = setpoint * dt;
						ref.weight -= STEPWEIGHT(loss);
					}
					ref.lasttime = t;
				}
			}
		}
	}

#endif
	*f = _F32(ref.weight * 1e05);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsLaumasLoadcellSim::RegisterBusInterface(IAdsBusInterface* pBusInterface)
{
	return ADS_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellSim::AdsGetInterface(IAdsLoadCellInterface** pInterface)
{
	assert(pInterface);
	if ( !g_pAdsLaumasLoadcellSim)
	{
		g_pAdsLaumasLoadcellSim = new CAdsLaumasLoadcellSim;
	}
	*pInterface = g_pAdsLaumasLoadcellSim;
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsLaumasLoadcellSim::AdsRemoveInterface()
{
	if (g_pAdsLaumasLoadcellSim)
	{
		delete g_pAdsLaumasLoadcellSim;
		g_pAdsLaumasLoadcellSim = nullptr;
	}
	return ADS_MPM_OK;
}


#endif
