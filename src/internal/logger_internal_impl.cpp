/**
 * @file logger_internal_impl.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Logger for internal use
 *
 * @author s3mat3
 */


#include <iostream>
#include <thread>
#include "mult.hpp"

#include "internal/logger_internal_impl.hpp"
#include "util.hpp"
MULT_NAMESPACE_BEGIN


namespace Internal {
    static constexpr std::array<char[12],8> LogLevelNameList = {
        " [None  ]: ",
        " [Fatal ]: ",
        " [Error ]: ",
        " [Warn  ]: ",
        " [Notice]: ",
        " [Info  ]: ",
        " [Debug ]: ",
    };
    const std::string& writeCOUT(
        LogLevel l,
        const std::string& msg,
        const std::string& ext,
        const std::string_view& fname,
        int line)
    {
        std::string base = LogLevelNameList[LogLevelIndex(l)] + msg;
        std::cout << convertTime_tToStr(getElapsedTime())
                  <<" ("
                  << ext
                  << ") ["
                  << std::this_thread::get_id()
                  << "] "
                  << base;
        if (fname.size() > 0) {
            std::cout << " " << fname;
        }
        if (line > 0) {
            std::cout << " in " << line;
        }
        std::cout << std::endl;
        return msg;
    }

    Logger::Logger(LogLevel l)
        : m_level(l)
        , m_guard() {}

    Logger::Logger() : Logger(LogLevel::non) {}
    Logger::~Logger()
    {
        std::cout << "Called Internal::Logger::~Logger() ON Terminate"
                  << std::endl;
    }

    void Logger::level(LogLevel l) noexcept
    {
        m_level = l;
    }

    LogLevel Logger::level() const noexcept
    {
        return m_level;
    }


    using namespace Internal;

    const std::string& Logger::logDebug(const std::string& msg, const std::string& ext, const std::string_view& fname, int line) const noexcept
    {
        std::lock_guard<std::mutex> lock(m_guard);
        if (m_level >= LogLevel::debug) {
            return writeCOUT(LogLevel::debug, msg, ext, fname, line);
        }
        return ext;
    }
    const std::string& Logger::logInfo(const std::string& msg, const std::string& ext, const std::string_view& fname, int line) const noexcept
    {
        std::lock_guard<std::mutex> lock(m_guard);
        if (m_level >= LogLevel::info) {
            return writeCOUT(LogLevel::info, msg, ext, fname, line);
        }
        return ext;
    }
    const std::string& Logger::logNotice(const std::string& msg, const std::string& ext, const std::string_view& fname, int line) const noexcept
    {
        std::lock_guard<std::mutex> lock(m_guard);
        if (m_level >= LogLevel::notice) {
            return writeCOUT(LogLevel::notice, msg, ext, fname, line);
        }
        return ext;
    }
    const std::string& Logger::logWarn(const std::string& msg, const std::string& ext, const std::string_view& fname, int line) const noexcept
    {
        std::lock_guard<std::mutex> lock(m_guard);
        if (m_level >= LogLevel::warn) {
            return writeCOUT(LogLevel::warn, msg, ext, fname, line);
        }
        return ext;
    }
    const std::string& Logger::logError(const std::string& msg, const std::string& ext, const std::string_view& fname, int line) const noexcept
    {
        std::lock_guard<std::mutex> lock(m_guard);
        if (m_level >= LogLevel::error) {
            return writeCOUT(LogLevel::error, msg, ext, fname, line);
        }
        return ext;
    }
    const std::string& Logger::logFatal(const std::string& msg, const std::string& ext, const std::string_view& fname, int line) const noexcept
    {
        std::lock_guard<std::mutex> lock(m_guard);
        if (m_level >= LogLevel::fatal) {
            return writeCOUT(LogLevel::fatal, msg, ext, fname, line);
        }
        return ext;
    }
    const std::string& Logger::mark(const std::string& msg, const std::string_view& fname, int line) const noexcept
    {
        std::lock_guard<std::mutex> lock(m_guard);
        std::cout << "\e[01;31m"; /* red bold */
        std::cout << Mult::getCurrentTime();
        std::cout << " ===>>>>> MARK <<<<<=== ";
        if (msg.size() > 0) std::cout << msg;
        if (fname.size() > 0) std::cout << " " << fname;
        if (line > 0) std::cout << " in " << line;
        std::cout << "\e[0m"<< std::endl;
        return msg;
    }
} //<-- namespace Internal ends here.
MULT_NAMESPACE_END

