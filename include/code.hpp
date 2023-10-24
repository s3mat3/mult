/**
 * @file code.hpp
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

#ifndef MULT_CODE_Hpp
# define  MULT_CODE_Hpp

namespace Mult {
    /** code class .
     *
     * The code generator
     */
    template <typename T> class code
    {
    public:
        const T m_value;
        constexpr code(T v) : m_value(v) {}
        constexpr operator T() const {return m_value;}
    };

} //<-- namespace Mult ends here.


#endif //<-- macro  MULT_CODE_Hpp ends here.
