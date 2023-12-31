/**
 * @file util.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Helper functions
 *
 * @author s3mat3
 */

#pragma once

#ifndef MULT_UTIL_Hpp
# define  MULT_UTIL_Hpp

# include <chrono>
# include <cstring> // for ::strerror_r
# include <iomanip>
# include <sys/time.h>

# include <string>

namespace Mult {
    /** Extruct file name from full path.
     *
     * constexpr version
     * nessesary target is fixed (like __FILE__ macro)
     */
    static inline constexpr std::string_view removePath(const char* target, char del='/')
    {
        std::string_view x = target;
        return x.substr(x.find_last_of(del) + 1);
    }
    /** Extruct file name from full path.
     *
     *  Normal function version
     */
    inline std::string removePath(const std::string& target, char del = '/')
    {
        auto pos = target.find_last_of(del);
        if (pos != std::string::npos) {
            return target.substr(pos + 1);
        }
        return target;
    }

    /** Get current time. 
     *
     * \note Only POSIX, precision microseconds
     */
    // [[deprecated("Please using another function!! This is only POSIX system")]]
    inline std::string getCurrentTime()
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

    /** Get elapsed time form startup .
     *
     * 起動開始から現在の経過時間をmicro秒で取得する
     *
     * debian/gccでの起動開始はOSの起動開始をepochとしている様だ、他の環境だともしかすると対象アプリの起動をepochとしているかも(大した問題ではない)
     */
    inline auto getElapsedTime() -> std::time_t
    {
        using namespace std::chrono;
        return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
    }

    /** std::chrono::time_t to string .
     *
     * 時間間隔を文字列に変換する。第二引数省略で16桁右詰め0パディングのフォーマット
     *
     *  \param[in] target 変換対象
     *  \param[in] col 桁数 デフォルトは16桁 
     *  \retval N桁右詰め0パディングの文字列
     */
    inline auto convertTime_tToStr(std::time_t target, size_t fill=16) -> std::string
    {
        std::stringstream time_stream;
        time_stream << std::setw(fill) << std::setfill('0') << target;
        return time_stream.str();
    }

    inline auto system_error(int err) -> std::string
    {
        std::string cause;
        cause.assign(::strerror_r(err, cause.data(), 128));
        return cause;
    }



} // namespace Mult

#endif //<-- macro  MULT_UTIL_Hpp ends here.
