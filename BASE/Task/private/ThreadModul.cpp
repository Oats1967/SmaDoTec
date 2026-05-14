//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ThreadModul.h
///
/// @file   ThreadModul.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include <windows.h>
#include "BASE/Task/public/ThreadModul.h"

namespace base
{
namespace task
{

void CThreadModul::_execute()
{
	while (m_Running)
	{
		execute();
		if (m_suspended)
		{
			std::unique_lock<std::mutex> ulock(rlock);
			sa.notify_all();
			cv.wait(ulock);
			m_suspended = FALSE;
		}
	}
}



int32_t CThreadModul::open(ThreadPriority iPriority)
{
	assert(!m_Running);
	if (m_Running)
	{
		return 0;
	}
	m_Running = TRUE;
	m_Thread = std::thread(&CThreadModul::_execute, this);
	auto wrapper = THREAD_PRIORITY_NORMAL;
	switch (iPriority)
	{
	case ThreadPriority::PRIORITY_IDLE: wrapper = THREAD_PRIORITY_IDLE;
		break;
	case ThreadPriority::PRIORITY_LOWEST: wrapper = THREAD_PRIORITY_LOWEST;
		break;
	case ThreadPriority::PRIORITY_BELOW_NORMAL: wrapper = THREAD_PRIORITY_BELOW_NORMAL;
		break;
	case ThreadPriority::PRIORITY_NORMAL: wrapper = THREAD_PRIORITY_NORMAL;
		break;
	case ThreadPriority::PRIORITY_ABOVE_NORMAL: wrapper = THREAD_PRIORITY_ABOVE_NORMAL;
		break;
	case ThreadPriority::PRIORITY_HIGHEST: wrapper = THREAD_PRIORITY_HIGHEST;
		break;
	default:
		wrapper = THREAD_PRIORITY_NORMAL;
		break;
	}
	::SetThreadPriority(m_Thread.native_handle(), wrapper);
	return 0;
}


int32_t CThreadModul::close(void)
{
	if (m_Running)
	{
		m_Running = FALSE;
		resume();
		m_Thread.join();
	}
	return 0;
}

CThreadModul::CThreadModul(void) : m_Running(FALSE), m_suspended(FALSE),/*m_Terminated(FALSE),*/  m_Thread()
{}


CThreadModul::~CThreadModul(void)
{
	close();
}

void CThreadModul::	suspend()
{
	m_suspended = (m_Running) ? TRUE : FALSE;
	if (m_suspended)
	{
		std::unique_lock<std::mutex> ulock(slock);
		sa.wait(ulock);
	}
}

void CThreadModul::resume()
{
	if (m_suspended)
	{
		cv.notify_all();
	}
}


};
};






