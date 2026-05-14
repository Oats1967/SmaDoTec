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
#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include "BASE/types.h"

namespace base
{
namespace task
{
enum class ThreadPriority : uint16_t
{
	PRIORITY_IDLE = 0,
	PRIORITY_LOWEST,
	PRIORITY_BELOW_NORMAL,
	PRIORITY_NORMAL,
	PRIORITY_ABOVE_NORMAL,
	PRIORITY_HIGHEST
};

class CThreadModul
{
private:
	std::atomic_bool m_Running;
	std::atomic_bool m_suspended;
	std::atomic_bool m_suspendActive;
	std::thread	m_Thread;
	std::mutex rlock;
	std::mutex slock;
	std::condition_variable cv;
	std::condition_variable sa; // suspend active

	void _execute();

protected:
	virtual int32_t execute(void) = 0;

public:
	CThreadModul(void);

	CThreadModul(const CThreadModul&) = delete;
	CThreadModul& operator=(const CThreadModul&) = delete;

	virtual ~CThreadModul(void);

	virtual int32_t open(ThreadPriority iPriority = ThreadPriority::PRIORITY_NORMAL);
	virtual int32_t close(void);

	virtual BOOL IsOpen(void) const
	{ return m_Running;	}

	BOOL IsRunning(void) const
	{	return m_Running;	}

	BOOL IsSuspended(void) const
	{	return m_suspended;	}

	void suspend();

	void resume();
};

};
};


