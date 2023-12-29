/**
 * @file mult.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Micro Utility Library for Tiny develop
 *
 * @author s3mat3
 */

#pragma once

#ifndef MULT_Hpp
# define  MULT_Hpp

# include <cstdint>
# include <limits>
# include <string>
# include <type_traits>

# define MULT_SGR_RESET   "\e[0m"
# define MULT_SGR_BOLD    "\e[1m"
# define MULT_SGR_THIN    "\e[2m"
# define MULT_SGR_ITALIC  "\e[3m"
# define MULT_SGR_UNDER   "\e[4m"
# define MULT_SGR_BLACK   "\e[30m"
# define MULT_SGR_RED     "\e[31m"
# define MULT_SGR_GREEN   "\e[32m"
# define MULT_SGR_YELLOW  "\e[33m"
# define MULT_SGR_BLUE    "\e[34m"
# define MULT_SGR_MAGENTA "\e[35m"
# define MULT_SGR_CYAN    "\e[36m"
# define MULT_SGR_WHITE   "\e[37m"


# define MULT_UNUSED_ARG(x) static_cast<void>(x)


# define MULT_NAMESPACE_BEGIN namespace Mult {
# define MULT_NAMESPACE_END }

namespace Mult {
    using void_ptr          = void*;
    using errno_t           = int;
    using byte_type         = char;
    using return_code       = std::int64_t; //!< return code
    using logic_code        = std::int64_t;
    using count_type        = std::size_t;
    using index_type        = std::size_t;
    using size_type         = std::size_t;
    using ID_t              = std::size_t;
    using ByteArray_t       = std::string;
    using time_interval     = std::int32_t;
    using millisec_interval = time_interval; //!< ミリ秒 2^31=2147483648[ms]で約2147483[s]で約596時間で約24日が最大となる
    using seconds_interval  = time_interval; //!< 秒 2^31=2147483648[s]で約596523時間で約24855日が最大となる
    using name_type         = std::string;

    using namespace std::literals::string_literals;
    /*! maximum value of index_t */
    constexpr index_type max_index() {return std::numeric_limits<index_type>::max();}
    /*! minimum value of index_t */
    constexpr index_type min_index() {return std::numeric_limits<index_type>::min();}
    /*! maximum value of return_code */
    constexpr return_code max_return_code() {return std::numeric_limits<return_code>::max();}
    /*! minimum value of return_code */
    constexpr return_code min_return_code() {return std::numeric_limits<return_code>::min();}
    /**  return_code .
     *
     *
     */
    static constexpr return_code DEVICE_ERROR_BASE = -30000;
    static constexpr return_code IO_ERROR_BASE     = -20000;
    static constexpr return_code OVER_INDEX        = -10104;
    static constexpr return_code UNDER_INDEX       = -10103;
    static constexpr return_code OVER_FLOW         = -10102;
    static constexpr return_code UNDER_FLOW        = -10101;
    static constexpr return_code OUT_OF_RANGE      = -10100;
    static constexpr return_code FAIL_JOIN         = -10014;
    static constexpr return_code FAIL_LUNCH        = -10013;
    static constexpr return_code FAIL_CMD          = -10012;
    static constexpr return_code FAIL_ARGC         = -10011;
    static constexpr return_code FAIL_ARG          = -10010;
    static constexpr return_code NO_DATA           = -10002;
    static constexpr return_code NO_RESOURCE       = -10001;
    static constexpr return_code TIMEOUT           = -10000;
    static constexpr return_code FAILURE           = -1;
    static constexpr return_code OK                =  0;

    static constexpr bool enable  = true;
    static constexpr bool disable = false;
    /*! Logic code
     *
     *
     */
    enum class LogicCode : logic_code {
        UNK = -1, //!< unknown state
        OFF =  0, //!< off state
        ON  =  1, //!< on state
        HiZ =  2, //!< high impedance state
    };
    static inline constexpr logic_code convert(LogicCode c) {return static_cast<logic_code>(c);}
    static inline constexpr LogicCode OFF()     {return LogicCode::OFF;}
    static inline constexpr LogicCode nOFF()    {return LogicCode::ON;}
    static inline constexpr LogicCode ON()      {return LogicCode::ON;}
    static inline constexpr LogicCode nON()     {return LogicCode::OFF;}
    static inline constexpr LogicCode Unknown() {return LogicCode::UNK;}
    static inline constexpr LogicCode HiZ()     {return LogicCode::HiZ;}

    /** char type checker .
     *
     *
     */
    template<typename T>
    struct is_char {static constexpr bool value = false;};

    template<>
    struct is_char<char> {static constexpr bool value = true;};
    template<typename T>

    static constexpr bool is_char_v = is_char<T>::value;
    /** range checker .
     *
     *
     */
    template<size_t lower, size_t upper, size_t target>
    struct static_range_check {
        static constexpr bool value = (lower <= target && target < upper);
    };

} //<-- namespace Mult ends here.

#endif //<-- macro  MULT_Hpp ends here.
