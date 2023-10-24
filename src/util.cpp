/**
 * @file util.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Utility
 *
 * @author s3mat3
 */

#include <cstring> // for ::strerror_r

#include <chrono>
#include <iomanip>
#include <sys/time.h>

#include "byte_buffer.hpp"
//#include "mult.hpp"
#include "util.hpp"

MULT_NAMESPACE_BEGIN

std::string removePath(const std::string& target, char del)
{
    auto pos = target.find_last_of(del);
    if (pos != std::string::npos) {
        return target.substr(pos + 1);
    }
    return target;
}

std::string getCurrentTime()
{
    std::string buf;
    std::string timeStampFormat = "0000-00-00T00:00:00"; // iso8601 style
    struct timeval now;
    gettimeofday(&now, NULL);
    // std::string usec = std::to_string( now.tv_usec );
    std::stringstream usec;
    usec << std::setw(6) << std::setfill('0') << std::to_string(now.tv_usec);
    std::strftime(const_cast<char *>(timeStampFormat.c_str()),
                  timeStampFormat.size() + 1, "%FT%H:%M:%S",
                  std::localtime(&(now.tv_sec)));
    buf = timeStampFormat;
    buf += ".";
    buf += usec.str();
    return buf;
}

auto getElapsedTime() -> std::time_t
{
    using namespace std::chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

auto convertTime_tToStr(std::time_t target, size_t fill) -> std::string
{
    std::stringstream time_stream;
    time_stream << std::setw(fill) << std::setfill('0') << target;
    return time_stream.str();
}

auto system_error(errno_t e) -> std::string
{
    std::string cause;
    cause.assign(::strerror_r(e, cause.data(), 128));
    return cause;
}

MULT_NAMESPACE_END
