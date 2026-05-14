//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module TypeBuffer
///
/// @file   TypeBuffer.h
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

#include <array>
#include "BASE/Base.h"
#include "BASE/Utils/public/Memory.h"


namespace base
{
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    template <class T, int32_t N>
    class CTypeBuffer : public std::array<T, N>
    {
    protected:
        uint32_t m_Filled;

        template<typename... ArgTypes>
        void  FillBuffer()
        { }

        template<typename... ArgTypes>
        void FillBuffer(T c, ArgTypes... args)
        {
            Push(c);
            FillBuffer(args...);
        }
        virtual void Clear()
        {   m_Filled = 0;      }

    public:
        GET(const uint32_t, Filled)

        void SetFilled(const uint32_t filled)
        {
            assert(filled <= N);
            m_Filled = filled;
        }

        operator const T* () const
        {   return std::array<T, N>::data();  }

        operator T* ()
        {   return std::array<T, N>::data();    }

        CTypeBuffer() : m_Filled{ 0 }
        {}
 
        void Push(const T c)
        {
            assert(m_Filled < N);
            std::array<T, N>::operator [] (m_Filled) = c;
            m_Filled++;
        }

    };

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    template <class T, int32_t N>
    class CNumericBuffer : public CTypeBuffer<T, N>
    {
    protected:
        void Clear() override
        {
            CTypeBuffer<T, N>::Clear();
            CTypeBuffer<T, N>::fill(0);
        }
    public:
        template<typename... ArgTypes>
        CNumericBuffer(T c, ArgTypes... args)
        {
            InitBuffer(c, args...);
        }

        template<typename... ArgTypes>
        void InitBuffer(T c, ArgTypes... args)
        {
            Clear();
            CTypeBuffer<T, N>::FillBuffer(c, args...);
        }

        CNumericBuffer()
        {   Clear();    }
    };

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    template <uint32_t N>
    class CCharBuffer : public CNumericBuffer<char_t, N>
    {
    public:
        template<typename... ArgTypes>
        CCharBuffer(char_t c, ArgTypes... args) : CNumericBuffer<char_t, N>(c, args...)
        {}

        CCharBuffer() = default;
    };

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    template <uint32_t N>
    class CWordBuffer : public CNumericBuffer<WORD, N>
    {
    public:
        template<typename... ArgTypes>
        CWordBuffer(WORD c, ArgTypes... args) : CNumericBuffer<WORD, N>(c, args...)
        {}

        CWordBuffer() = default;
    };

};
