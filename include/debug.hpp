/**
 * @file debug.hpp
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

#ifndef MULT_DEBUG_Hpp
# define  MULT_DEBUG_Hpp

# include <cxxabi.h>
# include <array>
# include <iostream>
# include <string>
# include <stdexcept>
# include "mult.hpp"

namespace Mult {
    namespace Debug {
        /** Type name demangling .
         *
         * Usage
         * \code
         * std::cout << "Type name is " << Mult::Debug::demangle(typeid(size_t).name()) << std::endl;
         * \endcode
         */
        inline std::string demangle(const char* name)
        {
            int status = 0;
            auto x = abi::__cxa_demangle(name, 0, 0, &status); // x is malloced in __cxa_demangle function copy aferter free
            std::string y(x);
            //////////////////////////////////////////////////
            free(x); // free x  *** This line don't remove ***
            //////////////////////////////////////////////////
            return y;
        }

        static constexpr std::array<char[6], 8 * 20> ctrlCharTbl = {
            "[NUL]","[SOH]","[STX]","[ETX]","[EOT]","[ENQ]","[ACK]","[BEL]",
            // "[NUL]","[01H]","[STX]","[ETX]","[04H]","[05H]","[ACK]","[07H]", //  1
            "[ BS]","[ HT]","[ LF]","[ VT]","[ FF]","[ CR]","[ SO]","[ SI]",
            // "[08H]","[09H]","[0aH]","[0bH]","[0cH]","[0dH]","[0eH]","[0fH]", //  2
            "[DEL]","[DC1]","[DC2]","[DC3]","[DC4]","[NAK]","[SYN]","[ETB]",
            // "[10H]","[11H]","[12H]","[13H]","[14H]","[NAK]","[16H]","[17H]", //  3
            "[CAN]","[ EM]","[SUB]","[ESC]","[ FS]","[ GS]","[ RS]","[ US]",
            // "[18H]","[19H]","[1aH]","[1bH]","[1cH]","[1dH]","[1eH]","[1fH]", //  4

            "[80H]","[81H]","[82H]","[83H]","[84H]","[85H]","[86H]","[87H]", //  5
            "[88H]","[89H]","[8aH]","[8bH]","[8cH]","[8dH]","[8eH]","[8fH]", //  6
            "[90H]","[91H]","[92H]","[93H]","[94H]","[95H]","[96H]","[97H]", //  7
            "[98H]","[99H]","[9aH]","[9bH]","[9cH]","[9dH]","[9eH]","[9fH]", //  8
            "[a0H]","[a1H]","[a2H]","[a3H]","[a4H]","[a5H]","[a6H]","[a7H]", //  9
            "[a8H]","[a9H]","[aaH]","[abH]","[acH]","[adH]","[aeH]","[afH]", // 10
            "[b0H]","[b1H]","[b2H]","[b3H]","[b4H]","[b5H]","[b6H]","[b7H]", // 11
            "[b8H]","[b9H]","[baH]","[bbH]","[bcH]","[bdH]","[beH]","[bfH]", // 12
            "[c0H]","[c1H]","[c2H]","[c3H]","[c4H]","[c5H]","[c6H]","[c7H]", // 13
            "[c8H]","[c9H]","[caH]","[cbH]","[ccH]","[cdH]","[ceH]","[cfH]", // 14
            "[d0H]","[d1H]","[d2H]","[d3H]","[d4H]","[d5H]","[d6H]","[d7H]", // 15
            "[d8H]","[d9H]","[daH]","[dbH]","[dcH]","[ddH]","[deH]","[dfH]", // 16
            "[e0H]","[e1H]","[e2H]","[e3H]","[e4H]","[e5H]","[e6H]","[e7H]", // 17
            "[e8H]","[e9H]","[eaH]","[ebH]","[ecH]","[edH]","[eeH]","[efH]", // 18
            "[f0H]","[f1H]","[f2H]","[f3H]","[f4H]","[f5H]","[f6H]","[f7H]", // 19
            "[f8H]","[f9H]","[faH]","[fbH]","[fcH]","[fdH]","[feH]","[EOF]", // 20
        }; //!<  Control character code table (It is necessary to map the outside of the readable range of the character table to 0x20 after code 0x80)
        /*! Convert control characters to readable strings
         *
         *  \param[in] s Source string of convert
         *  \retval converted characters(string)
         */
        inline std::string toReadableCtrlCode(const std::string& s)
        {
            std::string d{};
            std::string t;
            for (size_t i = 0; i < s.size(); ++i) {
                auto x = (static_cast<size_t>(s[i])) & 0xff;
                if (x < 0x20) {
                    d += ctrlCharTbl[x];
                } else if (x == 0x20) {
                    d.append("[SPC]");
                } else if (x == 0x7f) {
                    d.append("[DEL]");
                } else if (x > 0x7f) {
                    if (x == 0xff) {
                        d.append("[EOF]");
                    } else {
                        d.append(ctrlCharTbl[static_cast<std::uint8_t>(x - 0x60)]);
                    }
                } else {
                    d.push_back(x);
                }
            }
            return d;
        } //<-- function toReadableCtrlCode ends here.

        static constexpr std::array<char[3], 16*16> hexChar256Tbl = {
            "00","01","02","03","04","05","06","07","08","09","0a","0b","0c","0d","0e","0f",
            "10","11","12","13","14","15","16","17","18","19","1a","1b","1c","1d","1e","1f",
            "20","21","22","23","24","25","26","27","28","29","2a","2b","2c","2d","2e","2f",
            "30","31","32","33","34","35","36","37","38","39","3a","3b","3c","3d","3e","3f",
            "40","41","42","43","44","45","46","47","48","49","4a","4b","4c","4d","4e","4f",
            "50","51","52","53","54","55","56","57","58","59","5a","5b","5c","5d","5e","5f",
            "60","61","62","63","64","65","66","67","68","69","6a","6b","6c","6d","6e","6f",
            "70","71","72","73","74","75","76","77","78","79","7a","7b","7c","7d","7e","7f",
            "80","81","82","83","84","85","86","87","88","89","8a","8b","8c","8d","8e","8f",
            "90","91","92","93","94","95","96","97","98","99","9a","9b","9c","9d","9e","9f",
            "a0","a1","a2","a3","a4","a5","a6","a7","a8","a9","aa","ab","ac","ad","ae","af",
            "b0","b1","b2","b3","b4","b5","b6","b7","b8","b9","ba","bb","bc","bd","be","bf",
            "c0","c1","c2","c3","c4","c5","c6","c7","c8","c9","ca","cb","cc","cd","ce","cf",
            "d0","d1","d2","d3","d4","d5","d6","d7","d8","d9","da","db","dc","dd","de","df",
            "e0","e1","e2","e3","e4","e5","e6","e7","e8","e9","ea","eb","ec","ed","ee","ef",
            "f0","f1","f2","f3","f4","f5","f6","f7","f8","f9","fa","fb","fc","fd","fe","ff",
        };
        /*! Convert a Byte sequence to a hexadecimal representation string
         *
         *  \param[in] s Source string of convert
         *  \retval converted characters(string)
         */
        inline std::string hexDump(const std::string& s)
        {
            std::string t{};
            for (auto d : s) {
                t.append(hexChar256Tbl[(static_cast<std::uint8_t>(d)) & 0xff]);
            }
            return t;
        } //<--  function hexDump ends here.
        /** Helper .
         *
         *
         */
        static void assertion(const char* cond, const char* extend, bool noExcept, const char* file, const char* function, int line)
        {
            std::string message{MULT_SGR_BOLD};
            message += MULT_SGR_RED;
            message += "Mult assertion occurred: ";
            message += cond;
            message += "  [[";
            message += extend;
            message += "]]\n\t";
            message += MULT_SGR_BLUE;
            message += "function=> ";
            message += function;
            message += "\n\tin ";
            message += file;
            message += " at ";
            message += line;
            message += MULT_SGR_RESET;
            if (noExcept) {
                std::cout << MULT_SGR_RED
                          << "%%%%%%%%%%%%%%%%%%%%\n\n"
                          << MULT_SGR_RESET
                          << message
                          << "\n"
                          << MULT_SGR_RESET
                          << MULT_SGR_RED
                          << "%%%%%%%%%%%%%%%%%%%%"
                          << MULT_SGR_RESET
                          << std::endl;
                abort();
            } else {
                throw std::logic_error(message.c_str());
            }
        }
    } //<-- namespace Debug ends here.
} //<-- namespace Mult ends here.

# if defined (MULT_CHECK)
#  define MultASSERT(cond, msg, flg)                  \
    if (!(cond)) Mult::Debug::assertion(#cond, msg, flg, __FILE__, __PRETTY_FUNCTION__, __LINE__) else (void)0
# else
#  define MultASSERT(cond, msg, flg) 
# endif

# if defined (MULT_TRACE_FUNCTION)
#  include <iostream>
#  define TRACE(msg)           \
    std::cout << MULT_SGR_BOLD \
    << MULT_SGR_GREEN          \
    << "====> "                \
    << msg                     \
    << "\n\t\tfunction is -> " \
    << MULT_SGR_CYAN           \
    <<__PRETTY_FUNCTION__      \
    << MULT_SGR_RESET          \
    << " "                     \
    << __LINE__                \
    << std::endl
#  define MARK() std::cout << "***** mark ====> " << __LINE__ << " <==== marK *****" << std::endl
# else
#  define TRACE(msg)
#  define MARK()
# endif

#endif //<-- macro  MULT_DEBUG_Hpp ends here.
