/**
 * @file logger_impl.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Logger for app
 *
 * @author s3mat3
 */

#include <iostream>
#include <thread>

#include "mult.hpp"
#include "util.hpp"
#include "internal/logger_internal_impl.hpp"

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
        const std::string_view& fname = "",
        int line=0)
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
            std::cout << " " << fname << " ";
        }
        if (line > 0) {
            std::cout << " in " << line;
        }
        std::cout << std::endl;
        return msg;
    }
}
MULT_NAMESPACE_END

