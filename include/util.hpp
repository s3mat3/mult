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
# include <string>

namespace Mult {
    /**  full pathよりファイル名のみ取出す.
     *
     * コンパイル時定数となるハズですが。。。念の為static_asertで確認してね
     * 主な用途はtarget入力に__FILE__が来た場合を想定している。実行時にfile名が決る場合下のオーバーロードを利用してね
     */
    static inline constexpr std::string_view removePath(const char* target, char del='/')
    {
        std::string_view x = target;
        return x.substr(x.find_last_of(del) + 1);
    }
    /**  full pathよりファイル名のみ取出す.
     *
     *  通常関数版
     */
    std::string removePath(const std::string& target, char del = '/');
    /** 現時刻をiso8601スタイルで取得する .
     *
     * 精度はmicorosecであるがposixのみの対応である
     */
    [[deprecated("Please using another function!! This is only POSIX system")]]
    std::string getCurrentTime();
    /** Get elapsed time form startup .
     *
     * 起動開始から現在の経過時間をmicro秒で取得する
     *
     * debian/gccでの起動開始はOSの起動開始をepochとしている様だ、他の環境だともしかすると対象アプリの起動をepochとしているかも(大した問題ではない)
     */
    auto getElapsedTime() -> std::time_t;
    /** std::chrono::time_t to string .
     *
     * 時間間隔を文字列に変換する。第二引数省略で16桁右詰め0パディングのフォーマット
     *
     *  \param[in] target 変換対象
     *  \param[in] col 桁数 デフォルトは16桁 
     *  \retval N桁右詰め0パディングの文字列
     */
    auto convertTime_tToStr(std::time_t target, size_t col=16) -> std::string;
    auto system_error(int err) -> std::string;
} // namespace Mult

#endif //<-- macro  MULT_UTIL_Hpp ends here.
