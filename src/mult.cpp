/**
 * @file mult.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Core 
 *
 * @author s3mat3
 */


#include "mult.hpp"

#include "internal/logger_internal_impl.hpp"

MULT_NAMESPACE_BEGIN

namespace Internal {
    Logger MultGlobalLoggerInstance(LogLevel::debug);
    Logger MultGlobalLoggerInternalInstance(LogLevel::debug);
}

MULT_NAMESPACE_END
