/**
 * @file logger_internal_impl.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Logger internal
 *
 * @author s3mat3
 */

#pragma once

#ifndef INTERNAL_LOGGER_INTERNAL_IMPL_Hpp
# define  INTERNAL_LOGGER_INTERNAL_IMPL_Hpp

# include <mutex>
# include <string>

namespace Mult {
    enum class LogLevel : std::size_t {
        non = 0, //!< Mean logging non
        fatal,   //!< Mean logging only fatal
        error,   //!< Mean logging error fatal
        warn,    //!< Mean logging warning and error and fatal
        notice,  //!< Mean logging notice and warning ...
        info,    //!< Mean logging info and notice ...
        debug    //!< Mean logging all
    };
    constexpr std::size_t LogLevelIndex(LogLevel l) {return static_cast<std::size_t>(l);}

    namespace Internal {
        const std::string& writeCOUT(LogLevel l, const std::string& msg, const std::string& ext, const std::string_view& fname, int line);

        class Logger final
        {
        public:
            Logger(const Logger&) = delete;
            Logger& operator=(const Logger&) = delete;
            explicit Logger(LogLevel l);
            Logger();

            ~Logger();
            void level(LogLevel l) noexcept;
            LogLevel level() const noexcept;
            const std::string& logDebug( const std::string& msg, const std::string& ext, const std::string_view& fname="", int line=0) const noexcept;
            const std::string& logInfo(  const std::string& msg, const std::string& ext, const std::string_view& fname="", int line=0) const noexcept;
            const std::string& logNotice(const std::string& msg, const std::string& ext, const std::string_view& fname="", int line=0) const noexcept;
            const std::string& logWarn(  const std::string& msg, const std::string& ext, const std::string_view& fname="", int line=0) const noexcept;
            const std::string& logError( const std::string& msg, const std::string& ext, const std::string_view& fname="", int line=0) const noexcept;
            const std::string& logFatal( const std::string& msg, const std::string& ext, const std::string_view& fname="", int line=0) const noexcept;
            const std::string& mark(const std::string& msg="", const std::string_view& fname="", int line=0) const noexcept;
        private:
            LogLevel           m_level;
            mutable std::mutex m_guard;
        }; //<-- class Logger ends here.
    } //<-- namespace Internal ends here.
} //<-- namespace Mult ends here.

#endif //<-- macro  INTERNAL_LOGGER__INTERNAL_IMPL_Hpp ends here.
