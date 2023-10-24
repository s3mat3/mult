/**
 * @file platform.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Platform detecter
 *
 * @author s3mat3
 */

#pragma once

#ifndef MULT_PLATFORM_Hpp
# define  MULT_PLATFORM_Hpp

# ifdef _WIN32
#  define MULT_WINDOWS
# elif __posix
#  define MULT_POSIX
# elif __linux
#  define MULT_POSIX
# elif __unix
#  define MULT_POSIX
# else
#  define MULT_NOT_SUPORTED
# endif


#endif //<-- macro  MULT_PLATFORM_Hpp ends here.
