//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module TotalizerPulseThread
///
/// @file   TotalizerPulseThread.h
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

#include "BASE/Task/public/ThreadModul.h"
#include "BASE/Task/public/Task.h"
#include "AdsTotalizerPulseControl.h"


class CTotalizerPulseThread : public base::task::CThreadModul
{
	using CBaseClass = base::task::CThreadModul;
	CAdsTotalizerPulseControl m_AdsClient;
	std::condition_variable cv;
	std::mutex				mtx;
	uint32_t				m_PulseTime;
	std::atomic_bool		m_Triggered;
	std::atomic_bool		m_Terminated;

private:
	int32_t execute(void) override;

public:
	CTotalizerPulseThread() : CBaseClass()
		, m_PulseTime{ 0L }
		, m_Triggered{ false }
		, m_Terminated{ false }
	{}

	~CTotalizerPulseThread(void) override = default;
	BOOL Open();
	BOOL Close(void);
	void TriggerPulse(uint32_t t);
};

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
inline BOOL CTotalizerPulseThread ::Open()
{
	assert(!IsOpen());
	m_AdsClient.Init();
	auto result = CBaseClass::open();
	return BOOL(result == 0);
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
inline BOOL CTotalizerPulseThread::Close()
{
	assert(IsOpen());
	while (m_Triggered);
	m_Terminated = true;
	cv.notify_all();
	m_AdsClient.Exit();
	auto result = CBaseClass::close();
	return BOOL(result == 0);
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
inline void CTotalizerPulseThread::TriggerPulse(uint32_t t)
{
	assert(!m_Terminated);
	m_PulseTime = t;
	if ( (! m_Triggered) &&  (m_PulseTime > 0))
	{
		cv.notify_all();
	}
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
inline int32_t CTotalizerPulseThread::execute()
{
	if ( ! m_Terminated)
	{
		std::unique_lock<std::mutex> lck(mtx);
		cv.wait(lck);
		if ( ! m_Terminated)
		{
			m_Triggered = true;
			m_AdsClient.SetState(TRUE);
			base::task::Sleep(m_PulseTime);
			m_AdsClient.SetState(FALSE);
			m_Triggered = false;
		}
		else
		{
			//int k = 0;
		}
	}
	else
	{
		//int k = 0;
	}
	return 0;
}
