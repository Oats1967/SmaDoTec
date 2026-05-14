//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MutexContainer
///
/// @file   MutexContainer.h
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

#include <mutex>
#include <vector>
#include "BASE/types.h"

namespace base
{
    template<class T>
    class CMutexContainer : public std::mutex
    {
        std::vector< T > m_MutexItems;

    public:
        CMutexContainer() : m_MutexItems{}
        {}

        virtual ~CMutexContainer() = default;

        T& get(const int32_t index)
        {
            assert(index >= 0 && index < _S32(m_MutexItems.size()));
            return m_MutexItems[index];
        }
        const T& get(const int32_t index) const
        {
            assert(index >= 0 && index < _S32(m_MutexItems.size()));
            return m_MutexItems[index];
        }
        void resize(int32_t size)
        {
            m_MutexItems.resize(size);
        }
        size_t size() const
        {
            return m_MutexItems.size();
        }
    };
};



