/**
 * @file logger.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief
 *
 * @author s3mat3
 */

#pragma once

#ifndef MULT_LOGGER_Hpp
# define  MULT_LOGGER_Hpp

# include "internal/logger_internal_impl.hpp"
# include "util.hpp"

namespace Mult {
    namespace Internal {
        extern Logger MultGlobalLoggerInstance;
    }
} // namespace Mult


# define MULTLogger  Mult::Internal::MultGlobalLoggerInstance //!< logger instance
static inline void MULTLoggerLevel(Mult::LogLevel l) {Mult::Internal::MultGlobalLoggerInstance.level(l);}

// # define MULT_APP_NO_LOGGING 1
// # define NO_LINENUM 1
# if !defined(MULT_APP_NO_LOGGING) && !defined(NO_LINENUM)
#  define FatalLog(msg)                                                \
    MULTLogger.logFatal(msg, "APP ", Mult::removePath(__FILE__), __LINE__)
#  define ErrorLog(msg)                                                \
    MULTLogger.logError(msg, "APP ", Mult::removePath(__FILE__), __LINE__)
#  define WarnLog(msg)                                                 \
    MULTLogger.logWarn(msg, "APP ", Mult::removePath(__FILE__), __LINE__)
#  define NoticeLog(msg)                                               \
    MULTLogger.logNotice(msg, "APP ", Mult::removePath(__FILE__), __LINE__)
#  define InfoLog(msg)                                                 \
    MULTLogger.logInfo(msg, "APP ", Mult::removePath(__FILE__), __LINE__)
#  define DebugLog(msg)                                                \
    MULTLogger.logDebug(msg, "APP ", Mult::removePath(__FILE__), __LINE__)
#  define Marker(msg)                                                \
    MULTLogger.mark(msg,  Mult::removePath(__FILE__), __LINE__)
# elif !defined(MULT_APP_NO_LOGGING) && defined(NO_LINENUM)
#  define FatalLog(msg)                                                \
    MULTLogger.logFatal(msg, "APP ")
#  define ErrorLog(msg)                                                \
    MULTLogger.logErrror(msg, "APP ")
#  define WarnLog(msg)                                                 \
    MULTLogger.logWarn(msg, "APP ")
#  define NoticeLog(msg)                                               \
    MULTLogger.logNotice(msg, "APP ")
#  define InfoLog(msg)                                                 \
    MULTLogger.logInfo(msg, "APP ")
#  define DebugLog(msg)                                                \
    MULTLogger.logDebug(msg, "APP ")
#  define Marker(msg)                                                \
    MULTLogger.mark(msg)
# else
#  define FatalLog(msg)
#  define ErrorLog(msg)
#  define WarnLog(msg)
#  define NoticeLog(msg)
#  define InfoLog(msg)
#  define DebugLog(msg)
# endif
#endif //<-- macro  MULT_LOGGER_Hpp ends here.
