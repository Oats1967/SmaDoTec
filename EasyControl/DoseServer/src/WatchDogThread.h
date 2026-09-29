//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WatchDogThread
///
/// @file   WatchDogThread.h
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
#include "AdsWatchDogControl.h"


class CWatchDogThread : public base::task::CThreadModul
{
	using CBaseClass = base::task::CThreadModul;
	CAdsWatchDogControl m_AdsClient;
	std::condition_variable cv;
	std::mutex				mtx;
	std::atomic_bool		m_Triggered;
	std::atomic_bool		m_Terminated;
	BOOL					m_bOpen;
	const uint32_t c_TriggerPulse = 1000u;

private:
	int32_t execute(void) override;

public:
	CWatchDogThread() : CBaseClass()
		, m_Triggered{ false }
		, m_Terminated{ false }
		, m_bOpen{ FALSE }
	{}

	~CWatchDogThread(void) override = default;
	BOOL Open();
	BOOL Close(void);
	void TriggerWatchDog();
};

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
inline BOOL CWatchDogThread ::Open()
{
	m_Triggered = false;
	m_Terminated = false;

	assert( ! m_bOpen);
	if (! m_bOpen)
	{
		m_AdsClient.Init();
#ifndef _DEBUG
		if (m_AdsClient.IsEnabled())
#endif
		{
			(void)CBaseClass::open();
		}
		m_bOpen = TRUE;
	}
	return m_bOpen;
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
inline BOOL CWatchDogThread::Close()
{
	assert(m_bOpen);
	if (m_bOpen)
	{
		if (CBaseClass::IsOpen())
		{
			m_Terminated = true;
			while (m_Triggered);
			cv.notify_all();
			(void)CBaseClass::close();
		}
		m_AdsClient.Exit();
		m_bOpen = FALSE;
	}
	return m_bOpen;
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
inline void CWatchDogThread::TriggerWatchDog()
{
	if ( CBaseClass::IsOpen())
	{
		assert(!m_Terminated);
		if (!m_Triggered)
		{
			cv.notify_all();
		}
	}
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
inline int32_t CWatchDogThread::execute()
{
	if ( ! m_Terminated)
	{
		std::chrono::milliseconds ms{ c_TriggerPulse };
		std::unique_lock<std::mutex> lck(mtx);
		cv.wait_for(lck, ms);
		m_Triggered = true;
		BOOL bSignal = TRUE;
		for (uint32_t k = 0; k < 2u; k++)
		{
			if (m_Terminated )
			{ 
				m_AdsClient.SetState(FALSE);
				break;
			}
			m_AdsClient.SetState(bSignal);
			base::task::Sleep(c_TriggerPulse);
			bSignal = !bSignal;
		}
		m_Triggered = false;
	}
	else
	{
		//int k = 0;
	}
	return 0;
}
