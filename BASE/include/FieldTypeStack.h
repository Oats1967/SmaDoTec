//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FieldTypeStack
///
/// @file   FieldTypeStack.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <cassert>
#include <type_traits>
#include "GenericRemoteConfig.h"
#include "Endian.h"


#if _MSC_VER
#pragma warning ( push ) 
#pragma warning ( disable : 4701 )
#pragma warning ( disable : 4789 )
#pragma warning ( disable : 4018 )
#endif

namespace base
{
    template <typename T>
    inline typename std::enable_if<std::is_enum<T>::value, T>::type Div(const T c_value, const float32_t c_scale, const BOOL c_isScale)
    {  return c_value;   }

    template <typename T>
    inline typename std::enable_if< ! std::is_enum<T>::value, T>::type Div(const T c_value, const float32_t c_scale, const BOOL c_isScale)
    {   return c_isScale ? static_cast<T>(c_value / c_scale) : c_value;   }


    template <typename T>
    inline typename std::enable_if< ! std::is_enum<T>::value, T>::type Mul(const T c_value, const float32_t c_scale, const BOOL c_isScale)
    {   return c_isScale ? static_cast<T>(c_value * c_scale) : c_value;    }

    template <typename T>
    inline typename std::enable_if<std::is_enum<T>::value, T>::type Mul(const T c_value, const float32_t c_scale, const BOOL c_isScale)
    {   return c_value;   }

    template <typename T1, typename T2>
    inline typename std::enable_if<std::is_enum<T2>::value, T1>::type MaxRange(T2& value)
    {   return static_cast<T1>(value);    }

    template <typename T1, typename T2>
    inline typename std::enable_if< (! std::is_enum<T2>::value) && (std::is_integral<T1>::value), T1>::type MaxRange(T2& value)
    {
        return (value > std::numeric_limits<T1>::max()) ? std::numeric_limits<T1>::max() :
               ((value < std::numeric_limits<T1>::min()) ? std::numeric_limits<T1>::min() : static_cast<T1>(value));
    }

    template <typename T1, typename T2>
    inline typename std::enable_if< (!std::is_enum<T2>::value) && (! std::is_integral<T1>::value), T1>::type MaxRange(T2& value)
    {  return (value > std::numeric_limits<T1>::max()) ? std::numeric_limits<T1>::max() : static_cast<T1>(value);   }

    class CFieldTypeStack
    {
        uint8_t* m_buffer;
        const RemoteFieldType c_typ;
        const BOOL            c_bigEndian;
        const float32_t       c_scale;
        const BOOL            c_bIsScale;
        const uint32_t        c_MaxLength;
        uint32_t              m_count;

    private:
        template <typename T1, typename T2>
        T2 Get();

        template <typename T1, typename T2>
        void Add(T2& value);

        uint8_t MakeBigEndian(uint8_t value)
        {  return value;   }

    public:
        explicit CFieldTypeStack(uint8_t* buffer, const uint32_t maxlength, RemoteFieldType typ, BOOL bigEndian, float32_t scale = 0.0F) :
            m_buffer{ buffer }
            , c_typ{ typ }
            , c_bigEndian{ bigEndian }
            , c_scale{ scale }
            , c_bIsScale{ BOOL(fabs(scale) > std::numeric_limits<float32_t>::epsilon()) }
            , c_MaxLength{ maxlength }
            , m_count{ 0 }
        {}

        CFieldTypeStack() = delete;

        template <typename T>
        T pop();

        template <typename T>
        void push(T& value);

    };

    
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    template <typename T1, typename T2>
    T2 CFieldTypeStack::Get()
    {
        T2 retval = static_cast<T2>(0);
        assert(m_count + sizeof(T1) <= c_MaxLength);
        if (m_count + sizeof(T1) <= c_MaxLength)
        {
            T1 value = CEndianBase<T1>(c_bigEndian).Get(*((T1*)&m_buffer[m_count]));
            value = Div(value, c_scale, c_bIsScale);
            retval = MaxRange<T2>(value);
            m_count += sizeof(T1);
        }
        return retval;
    }
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    template <typename T1, typename T2>
    void CFieldTypeStack::Add(T2& value)
    {
        assert(m_count + sizeof(T1) <= c_MaxLength);
        if (m_count + sizeof(T1) <= c_MaxLength)
        {
            T2 v2 = Mul(value, c_scale, c_bIsScale);
            *((T1*)&m_buffer[m_count]) = CEndianBase<T1>(c_bigEndian).Get(MaxRange<T1>(v2));
            m_count += sizeof(T1);
        }
    }
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    template <typename T>
    T CFieldTypeStack::pop()
    {
        T retval = 0;

        switch (c_typ)
        {
        case RemoteFieldType::UINT8:
            retval = Get<uint8_t, T>();
            break;

        case RemoteFieldType::UINT16:
            retval = Get<uint16_t, T>();
            break;

        case RemoteFieldType::UINT32:
            retval = Get<uint32_t, T>();
            break;

        case RemoteFieldType::UINT64:
            retval = Get<uint64_t, T>();
            break;

        case RemoteFieldType::INT8:
            retval = Get<int8_t, T>();
            break;

        case RemoteFieldType::INT16:
            retval = Get<int16_t, T>();
            break;

        case RemoteFieldType::INT32:
            retval = Get<int32_t, T>();
            break;

        case RemoteFieldType::INT64:
            retval = Get<int64_t, T>();
            break;

        case RemoteFieldType::FLOAT32:
            retval = Get<float32_t, T>();
            break;

        case RemoteFieldType::FLOAT64:
            retval = Get<float64_t, T>();
            break;

        default:
            assert(FALSE);
            break;
        }
        return retval;
    }
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------
    template <typename T>
    void CFieldTypeStack::push(T& value)
    {
        switch (c_typ)
        {
        case RemoteFieldType::UINT8:
            Add<uint8_t, T>(value);
            break;

        case RemoteFieldType::UINT16:
            Add<uint16_t, T>(value);
            break;

        case RemoteFieldType::UINT32:
            Add<uint32_t, T>(value);
            break;

        case RemoteFieldType::UINT64:
            Add<uint64_t, T>(value);
            break;

        case RemoteFieldType::INT8:
            Add<int8_t, T>(value);
            break;

        case RemoteFieldType::INT16:
            Add<int16_t, T>(value);
            break;

        case RemoteFieldType::INT32:
            Add<int32_t, T>(value);
            break;

        case RemoteFieldType::INT64:
            Add<int64_t, T>(value);
            break;

        case RemoteFieldType::FLOAT32:
            Add<float32_t, T>(value);
            break;

        case RemoteFieldType::FLOAT64:
            Add<float64_t, T>(value);
            break;

        default:
            assert(FALSE);
            break;
        }
    }
};

#if _MSC_VER
#pragma warning ( pop )
#endif

