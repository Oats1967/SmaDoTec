//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module task.h
///
/// @file   task.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/types.h"
#include <thread>
#include <chrono>

namespace base
{
namespace task
{
    void	Sleep(uint32_t waittime)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(waittime));
    }
};
};



