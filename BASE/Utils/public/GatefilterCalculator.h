//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Plausibel.h
///
/// @file   Plausibel.h
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

#include "BASE/Base.h"
#include "BASE/Utils/public/FloatRingBuffer.h"
#include "BASE/include/Gatefilter.h"

namespace base
{
namespace utils
{
constexpr uint32_t GATEFILTER_RINGUFFERSIZE = 5U;

typedef struct __NOISEINDEX
{
	float32_t   fNoiseIndex;
	eGatefilter  sGatefilter;
	int32_t     lPlausibelFailed;
	float32_t   fMassflowGleitend;
	float32_t   fMassflowGewichtet;
	float32_t   fMassflow;
} NOISEINDEX;


class CGatefilterCalculator : public base::utils::CFloatRingBuffer<GATEFILTER_RINGUFFERSIZE>
{
public:
	static constexpr int32_t GATEFILTER_NMAX = 10;

public:
	float32_t   m_fNoiseIndex;
	float32_t   m_fMassflowGewichtet;
	float32_t   m_fMassflowGleitend;
	float32_t   m_fMassflowFilter;
	float32_t   m_fPidGatefilter;
	float32_t   m_fSetpoint;
	eGatefilter m_sGatefilterStatus;
	int32_t     m_lPlausibelFailed;
	int32_t     m_MaxPlausibelCounter;

private:
	BOOL         Calc( const float32_t fMassflow, NOISEINDEX& rRauschenIndex);
	eGatefilter  GetGatefilterStatus(const float32_t& fNoiseIndex) const;
	float32_t	 GetMeanShort() const;

public:
	CGatefilterCalculator(void);
	CGatefilterCalculator(const CGatefilterCalculator& rGate);
	CGatefilterCalculator& operator = (const CGatefilterCalculator& rGate);

	~CGatefilterCalculator(void);
	void	Open(void);
	void	Close(void);
	void    Init(const float32_t fPidGatefilter, const float32_t fMassflowFilter, const float32_t setpoint, const float32_t massflow);
	BOOL	Push(const float32_t fMassflow, NOISEINDEX& rRauschenIndex);

	float32_t	GetNoiseIndex(void) const
	{	return m_fNoiseIndex;	}

	float32_t	GetPidGatefilter(void) const
	{	return m_fPidGatefilter; 	}

	void	SetPidGatefilter(const float32_t fPidGatefilter)
	{m_fPidGatefilter = fPidGatefilter;	}

	float32_t	GetMassflowFilter(void) const
	{ return m_fMassflowFilter;}

	void	SetMassflowFilter(const float32_t fMassflowfilter)
	{ m_fMassflowFilter = RANGE(fMassflowfilter, 0.0f, 1.0f);	}

	void	SetMassflow(const float32_t);

	void	SetSetpoint(const float32_t);

	void SetMaxPlausibelCounter(const int32_t iMaxPlausibelCounter)
	{
		m_MaxPlausibelCounter = iMaxPlausibelCounter;
	}
	BOOL isNoError(void) const
	{
		return m_lPlausibelFailed == 0;
	}
	BOOL isErrorLimitExceeded(void) const
	{
		return m_lPlausibelFailed >= m_MaxPlausibelCounter;
	}
	int32_t GetPlausibelFailedCounter() const
	{
		return m_lPlausibelFailed;
	}
};

}; // namespace utils
}; // namespace base


