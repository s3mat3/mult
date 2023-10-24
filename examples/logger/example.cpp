/**
 * @file example.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Example for logger
 *
 * @author s3mat3
 */

#include "logger.hpp"

int main()
{
    MULTLoggerLevel(Mult::LogLevel::debug);
    Marker("debug");
    DebugLog("Debug");
    InfoLog("Info");
    NoticeLog("Notice");
    WarnLog("Warn");
    ErrorLog("Error");
    FatalLog("Fatal");

    MULTLoggerLevel(Mult::LogLevel::info);
    Marker("info");
    DebugLog("Debug");
    InfoLog("Info");
    NoticeLog("Notice");
    WarnLog("Warn");
    ErrorLog("Error");
    FatalLog("Fatal");

    MULTLoggerLevel(Mult::LogLevel::notice);
    Marker("notice");
    DebugLog("Debug");
    InfoLog("Info");
    NoticeLog("Notice");
    WarnLog("Warn");
    ErrorLog("Error");
    FatalLog("Fatal");

    MULTLoggerLevel(Mult::LogLevel::warn);
    Marker("warn");
    DebugLog("Debug");
    InfoLog("Info");
    NoticeLog("Notice");
    WarnLog("Warn");
    ErrorLog("Error");
    FatalLog("Fatal");

    MULTLoggerLevel(Mult::LogLevel::error);
    Marker("error");
    DebugLog("Debug");
    InfoLog("Info");
    NoticeLog("Notice");
    WarnLog("Warn");
    ErrorLog("Error");
    FatalLog("Fatal");

    MULTLoggerLevel(Mult::LogLevel::fatal);
    Marker("fatal");
    DebugLog("Debug");
    InfoLog("Info");
    NoticeLog("Notice");
    WarnLog("Warn");
    ErrorLog("Error");
    FatalLog("Fatal");
    return 0;
}
