//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Logger
///
/// @file   Logger.h
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


#include <iostream>
#include <sstream>      // std::stringbuf
#include <streambuf>
#include "BASE/types.h"
#include "BASE/Base.def"
#include "Logger/include/LoggerImpl.h"

#define LOGLEVEL_INFO    0
#define LOGLEVEL_DEBUG   1
#define LOGLEVEL_WARNING 2
#define LOGLEVEL_ERROR   3
#define LOGLEVEL_NONE    4


#include LOGLEVELFILE

#if LOGLEVEL  == LOGLEVEL_INFO
#pragma message("LogLevel = INFO")
#elif LOGLEVEL  == LOGLEVEL_DEBUG
#pragma message("LogLevel = DEBUG")
#elif LOGLEVEL  == LOGLEVEL_WARNING
#pragma message("LogLevel = WARNING")
#elif LOGLEVEL  == LOGLEVEL_ERROR
#pragma message("LogLevel = ERROR")
#endif

#if LOGLEVEL  < LOGLEVEL_NONE

namespace base
{
namespace logger
{
struct None { };

template<typename List>
struct LogData {
    List list;
};

class dbgview_buffer : public std::stringbuf
{
public:
    ~dbgview_buffer()
    {
        sync(); // can be avoided
    }

    int sync()
    {
        auto p = logger_getDebugOutput();
        if (!p)
        {
            std::cout << str();
        }
        else
        {
            p(str().c_str());
        }
        str("");
        return 0;
    }
};


class debug_stream : public std::ostringstream
{
public:
    template<typename T>
    friend debug_stream& operator<<(debug_stream& os, T&& s);
};

template<typename T>
debug_stream& operator<<(debug_stream& os, T&& s)
 {
    (std::ostringstream&)os << s;
    auto p = logger_getDebugOutput();
    if (!os.str().empty())
    {
        if (!p)
        {
            std::cout << os.str();
        }
        else
        {
            p(os.str().c_str());
        }
        os.str("");
    }
    return os;
}


template<typename List>
void Log(const char_t* type, const char_t* file, LogData<List>&& data)
{
    dbgview_buffer buf;
    debug_stream os;
    
    os << type << " " << file << ": ";
    output(os, std::move(data.list));
    os << '\n';
}


template<typename Begin, typename Value>
constexpr LogData<std::pair<Begin&&, Value&&>> operator<<(LogData<Begin>&& begin,
    Value&& value) noexcept
{
    return { { std::forward<Begin>(begin.list), std::forward<Value>(value) } };
}

template<typename Begin, size_t n>
constexpr LogData<std::pair<Begin&&, const char*>> operator<<(LogData<Begin>&& begin,
    const char(&value)[n]) noexcept
{
    return { { std::forward<Begin>(begin.list), value } };
}

typedef std::ostream& (*PfnManipulator)(std::ostream&);

template<typename Begin>
constexpr LogData<std::pair<Begin&&, PfnManipulator>> operator<<(LogData<Begin>&& begin,
    PfnManipulator value) noexcept
{
    return { { std::forward<Begin>(begin.list), value } };
}

template <typename Begin, typename Last>
void output(std::ostream& os, std::pair<Begin, Last>&& data)
{
    output(os, std::move(data.first));
    (debug_stream& )os << data.second;
}

inline void output(std::ostream& os, None)
{
    BASE_UNUSED_VARIABLE(os);
}

};
};
#endif


#define LOG(type, msg) (base::logger::Log(type, __FILE__, base::logger::LogData<base::logger::None>() << msg))

#if LOGLEVEL <= LOGLEVEL_INFO
#define LOGINFO(msg) LOG("INFO !", __func__ << " (" << __LINE__<< ") " << msg);
#else
#define LOGINFO(...) do; while (0)
#endif

#if LOGLEVEL <= LOGLEVEL_DEBUG
#define LOGDEBUG(msg) LOG("DEBUG !",  __func__ << " (" << __LINE__<< ") " << msg);
#else
#define LOGDEBUG(...) do; while (0)
#endif


#if LOGLEVEL <= LOGLEVEL_WARNING
#define LOGWARN(msg) LOG("WARNING !", __func__ << " (" << __LINE__<< ") " << msg);
#else
#define LOGWARN(...) do; while (0)
#endif

#if LOGLEVEL <= LOGLEVEL_ERROR
#define LOGERROR(msg) LOG("ERROR !", __func__ << " (" << __LINE__<< ") " << msg);
#else
#define LOGERROR(...) do; while (0)
#endif


