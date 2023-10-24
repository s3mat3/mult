/**
 * @file logger.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Logger for internal use
 *
 * @author s3mat3
 */

#pragma once

#ifndef INTERNAL_LOGGER_Hpp
# define  INTERNAL_LOGGER_Hpp

# include "internal/logger_internal_impl.hpp"
# include "util.hpp"

namespace Mult {
    namespace Internal {
        extern Logger MultGlobalLoggerInternalInstance;
    }
} // namespace Mult

# define MULTLoggerInternal  Mult::Internal::MultGlobalLoggerInternalInstance //!< logger instance
static inline void MULTLoggerInternalLevel(Mult::LogLevel l) {Mult::Internal::MultGlobalLoggerInternalInstance.level(l);}

//#define NO_LINENUM
# if !defined(MULT_NO_LOGGING) && !defined(NO_LINENUM)
#  define MULTFatal(msg)                                                 \
    MULTLoggerInternal.logFatal(msg, "MULT", Mult::removePath(__FILE__), __LINE__)
#  define MULTError(msg)                                                 \
    MULTLoggerInternal.logError(msg, "MULT", Mult::removePath(__FILE__), __LINE__)
#  define MULTWarn(msg)                                                  \
    MULTLoggerInternal.logWarn(msg, "MULT", Mult::removePath(__FILE__), __LINE__)
#  define MULTNotice(msg)                                                \
    MULTLoggerInternal.logNotice(msg, "MULT", Mult::removePath(__FILE__), __LINE__)
#  define MULTInfo(msg)                                                  \
    MULTLoggerInternal.logInfo(msg, "MULT", Mult::removePath(__FILE__), __LINE__)
#  define MULTDebug(msg)                                                 \
    MULTLoggerInternal.logDebug(msg, "MULT", Mult::removePath(__FILE__), __LINE__)
# elif !defined(MULT_NO_LOGGING) && defined(NO_LINENUM)
#  define MULTFatal(msg)                                                 \
    MULTLoggerInternal.logFatal(msg, "MULT")
#  define MULTError(msg)                                                 \
    MULTLoggerInternal.logError(msg, "MULT")
#  define MULTWarn(msg)                                                  \
    MULTLoggerInternal.logWarn(msg, "MULT")
#  define MULTNotice(msg)                                                \
    MULTLoggerInternal.logNotice(msg, "MULT")
#  define MULTInfo(msg)                                                  \
    MULTLoggerInternal.logInfo(msg, "MULT")
#  define MULTDebug(msg)                                                 \
    MULTLoggerInternal.logDebug(msg, "MULT")
# else
#  define MULTFatal(msg)
#  define MULTError(msg)
#  define MULTWarn(msg)
#  define MULTNotice(msg)
#  define MULTInfo(msg)
#  define MULTDebug(msg)
# endif



#endif //<-- macro  INTERNAL_LOGGER_Hpp ends here.

