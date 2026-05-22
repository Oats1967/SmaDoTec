//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsPesaCellSim
///
/// @file   AdsPesaCellSim.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#ifdef __SIM_PESA

#include "BASE/Base.def"
#include "DoseDataLib/include/DoseData.h"
#include "AdsClient/src/private/AdsPesaCellSim.h"
#include "AdsClient/include/AdsTypen.h"


#define STEPWEIGHT(_a, _c) _F32( ((_a) *10.0F / _c ))



static IAdsLoadCellInterface* g_pAdsPesaLoadcellSim = nullptr;


//*****************************************************************************************************************
//*****************************************************************************************************************
CAdsPesaLoadcellSim::CAdsPesaLoadcellSim() : m_ix{ 0 }
{
	m_retval[0] = ADS_MPM_OK;
	m_retval[1] = ADS_MPM_TIMEOUT;
};
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::Init(const int32_t)
{
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::Init()
{
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::Close(const int32_t)
{
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::Close()
{
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::Reset(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::Reset()
{
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::GetSensorActive(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::SetPPM(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::GetFullLoad(const int32_t index, float32_t* fFullLoad)
{
	*fFullLoad = c_Fullload;
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::SetNoiseControlWeight(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::SetAddress(const int32_t index, int32_t iNew)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iNew);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::GetVersion(int32_t* sSW)
{
	*sSW = 1;
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::SetNoiseControl(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::SetWindowTime(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::SetWindowRes(const int32_t index, int32_t iRes)
{
	BASE_UNUSED_VARIABLE(index);
	BASE_UNUSED_VARIABLE(iRes);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::DeleteReference(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::SetReference(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::SetTara(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::ClrTara(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::DeleteZero(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::SetZero(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::EEpromVerify(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::EEpromReadCode(const int32_t index, WORD* w)
{
	BASE_UNUSED_VARIABLE(index);
	*w = WORD(1);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::RegisterLC(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}

//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::EnableEEpromWriteProtection(const int32_t index, BOOL bEnable)
{
	BASE_UNUSED_VARIABLE(bEnable);
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::PrepareWriteEEprom(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::IsPresent(const int32_t index)
{
	BASE_UNUSED_VARIABLE(index);
	return m_retval[m_ix];
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::GetSubType(const int32_t index, base::eLcSubType* type)
{
	BASE_UNUSED_VARIABLE(index);
	*type = base::eLcSubType::ePESA;
	return m_retval[m_ix];
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsPesaLoadcellSim::GetZeroIndex(const int32_t index, int32_t* value)
{
	assert(value);
	BASE_UNUSED_VARIABLE(index);
	*value = 0;
	return ADS_MPM_OK;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
int32_t CAdsPesaLoadcellSim::GetWeight(const int32_t index, float32_t* f)
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
		ref.weight = STEPWEIGHT(c_InitWeight, c_Fullload);;
	}
#if 0
	static BOOL bRefillWorks = TRUE;
	BOOL state = FALSE;
	if (bRefillWorks)
	{
		Dose_DSVGetRefillState(index, &state);
	}
	if (state)
	{
		ref.weight += STEPWEIGHT(0.5F, c_Fullload);
		ref.m_aTimer.Stop();
	}
	else
	{
		static BOOL bJumpNoise = FALSE;
		if (bJumpNoise)
		{
			ref.weight += STEPWEIGHT(10.0F, c_Fullload);
			bJumpNoise = FALSE;
		}
		Dose_DSVGetRelease(index, &state);
		if (state)
		{
			if ( !ref.m_aTimer.IsStarted() )
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
					uint32_t dt = t - ref.lasttime;
					float32_t setpoint;
					base::eDoseType lDoseType = base::eDoseType::DOSETYPE_NONE;
					Dose_DSVGetDoseType(index, &lDoseType);
					if (base::__ISBATCHERTYPE(lDoseType))
					{
						float32_t driveCommand = 0.0F;
						Dose_DSVGetActualDriveCommand(index, &driveCommand);
						const float32_t cMaxMassflow = 50.0F;
						const float32_t fMassflow = cMaxMassflow / 3600.0F * driveCommand;
						float32_t loss = fMassflow * dt;
						ref.weight -= STEPWEIGHT(loss, c_Fullload);;
						ref.weight = __max(ref.weight, 0.0F);
					}
					else
					{
						Dose_DSVGetActualSetpoint(index, &setpoint);

						setpoint /= 3600.0F;
						float32_t loss = setpoint * dt;
						ref.weight -= STEPWEIGHT(loss, c_Fullload);;
						ref.weight = __max(ref.weight, 0.0F);
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
int32_t CAdsPesaLoadcellSim::RegisterBusInterface(IAdsBusInterface* pBusInterface)
{
	return ADS_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsPesaLoadcellSim::AdsGetInterface(IAdsLoadCellInterface** pInterface)
{
	assert(pInterface);
	if (!g_pAdsPesaLoadcellSim)
	{
		g_pAdsPesaLoadcellSim = new CAdsPesaLoadcellSim;
	}
	*pInterface = g_pAdsPesaLoadcellSim;
	return ADS_MPM_OK;
}
//************************************************************************************************
//************************************************************************************************
int32_t CAdsPesaLoadcellSim::AdsRemoveInterface()
{
	if (g_pAdsPesaLoadcellSim)
	{
		delete g_pAdsPesaLoadcellSim;
		g_pAdsPesaLoadcellSim = nullptr;
	}
	return ADS_MPM_OK;
}



#endif
