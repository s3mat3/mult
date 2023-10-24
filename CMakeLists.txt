###########################################################
# usage cmake
# -DCMAKE_BUILD_TYPE=(Debug | Release | Memcheck)
# -DCMAKE_EXPORT_COMPILE_COMMANDS=on
# -DCMAKE_VERBOSE_MAKEFILE:BOOL=TRUE
# -DMAKE_MULT_TEST=1 we will compile code
# -DMAKE_MULT_EXAMPLE=1 we will compile example code
# -DMAKE_MULT_DOC=1 we will generate source document
#############################################################
cmake_minimum_required (VERSION 3.14)
project("Mult" CXX)
set(TARGET_BASE "mult-base")
set(MULT_BASE_LIB ${TARGET_BASE})
if (NOT (DEFINED PROJ_VERSION))
  #
  # SET VERSION NUMBER
  #
  EXECUTE_PROCESS (COMMAND date +%F OUTPUT_VARIABLE CMAKE_BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
  EXECUTE_PROCESS (COMMAND git rev-parse --short HEAD OUTPUT_VARIABLE CMAKE_REVISION OUTPUT_STRIP_TRAILING_WHITESPACE)
  EXECUTE_PROCESS (COMMAND git rev-parse --abbrev-ref HEAD OUTPUT_VARIABLE CMAKE_DESTINATION OUTPUT_STRIP_TRAILING_WHITESPACE)
  EXECUTE_PROCESS (COMMAND cat ${PROJECT_SOURCE_DIR}/VERSION OUTPUT_VARIABLE CMAKE_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

  set (PROJ_BUILD_DATE ${CMAKE_BUILD_DATE})
  set (PROJ_VERSION "ver. ${CMAKE_VERSION}")
  set (PROJ_DOC_VERSION ${CMAKE_VERSION})
  set (PROJ_REVISION ${CMAKE_REVISION})
  #SET (PROJ_VERSION_REVISION "${PROJ_BUILD_DATE} ${CMAKE_DESTINATION} ${CMAKE_VERSION}-${PROJ_REVISION}")
  set (PROJ_VERSION_REVISION "${PROJ_BUILD_DATE} ${CMAKE_VERSION}-${PROJ_REVISION}")
endif()

if (NOT (DEFINED MULT_BASE_DIR))
  set(MULT_BASE_DIR     ${PROJECT_SOURCE_DIR})
  set(MULT_BUILD_DIR    ${PROJECT_BINARY_DIR})
  set(MULT_DOC_OUT_DIR  ${MULT_BUILD_DIR}/doc)
  set(MULT_3RD_BASE     ${MULT_BASE_DIR}/3rd)
  set(MULT_INCLUDE_BASE ${MULT_BASE_DIR}/include)
  set(MULT_SOURCE_BASE  ${MULT_BASE_DIR}/src)
  set(MULT_INTERNAL     ${MULT_INCLUDE_BASE}/internal)
  set(MULT_DOC_ROOT     ${MULT_BASE_DIR}/doc-root)
  set(TOOLS_TESTER_NAME "doctest")
  set(TOOLS_TESTER_BASE ${MULT_3RD_BASE}/doctest/${TOOLS_TESTER_NAME})
  set(TOOLS_BENCHMARK_NAME "benchmark")
  set(TOOLS_BENCHMARK_BASE "/opt/tools")
  ## compile options
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  ## list(APPEND CMAKE_CXX_FLAGS "-std=c++17 -pthread -lrt -fpic -pedantic -Wall -W -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual ${CMAKE_CXX_FLAGS}")
  # debug or release
  set(CMAKE_CXX_FLAGS_MEMCHECK "-O0 -g3 -DTRACE_FUNCTION")
  set(CMAKE_CXX_FLAGS_DEBUG    "-O2 -g3 -DTRACE_FUNCTION")
  set(CMAKE_CXX_FLAGS_RELEASE  "-DNDEBUG -O2")
else()
  set(MULT_DOC_OUT_DIR "${PROJECT_BINARY_DIR}/Mult/doc")
endif()

if(NOT (DEFINED MULT_LIB_OUT_DIR))
  set(MULT_LIB_OUT_DIR ${MULT_BUILD_DIR}/lib)
endif()

set(LIBRARY_OUTPUT_PATH ${MULT_LIB_OUT_DIR})

if (MAKE_MULT_TEST)
  enable_testing()
  if (NOT (DEFINED MULT_TEST_BASE))
    set(MULT_TEST_BASE ${MULT_BASE_DIR}/tests)
  endif()

  if(NOT (DEFINED MULT_TEST_OUT_DIR))
    set(MULT_TEST_OUT_DIR ${PROJECT_BINARY_DIR}/mult/tests)
  endif()

  # add_subdirectory(${MULT_TEST_BASE}/byte_array)
  add_subdirectory(${MULT_TEST_BASE}/buffer)
  add_subdirectory(${MULT_TEST_BASE}/measure_time)
endif()

if (MAKE_MULT_EXAMPLE)
  find_program(TOOL_MEMORY_CHECKER "valgrind")
  if (TOOL_MEMORY_CHECKER)
    set(TOOL_THREAD_CHECKER ${TOOL_MEMORY_CHECKER})
    set(TOOL_MEMORY_CHECK_OPTIONS
      --tool=memcheck
      --leak-check=full
      --show-reachable=yes
      --track-fds=yes
      )
    set(TOOL_THREAD_CHECK_OPTIONS
      --tool=helgrind
      )
    message("We have a memory checker called Valgrind")
  endif()
  # check and examples directories
  if(NOT (DEFINED MULT_EXAMPLE_BASE))
    set(MULT_EXAMPLE_BASE ${MULT_BASE_DIR}/examples)
  endif()
  # examples output directories
  if(NOT (DEFINED MULT_EXAMPLE_OUT_DIR))
    set(MULT_EXAMPLE_OUT_DIR ${PROJECT_BINARY_DIR}/mult/examples)
  endif()
  add_subdirectory(${MULT_EXAMPLE_BASE}/controller)
  add_subdirectory(${MULT_EXAMPLE_BASE}/logger)
  add_subdirectory(${MULT_EXAMPLE_BASE}/notification)
  add_subdirectory(${MULT_EXAMPLE_BASE}/thread)
endif()
##############
### documents
##############
if (MAKE_MULT_DOC)
  add_subdirectory(${MULT_DOC_ROOT})
endif()

set(MULT_BASE_LIB_SOURCES
  ${MULT_SOURCE_BASE}/util.cpp
  ${MULT_SOURCE_BASE}/internal/logger_internal_impl.cpp
  ${MULT_SOURCE_BASE}/mult.cpp
  )

add_library(${MULT_BASE_LIB}
  STATIC
  ${MULT_BASE_LIB_SOURCES}
  )

target_include_directories(${MULT_BASE_LIB}
  PUBLIC
  ${MULT_INCLUDE_BASE}
  )

target_link_libraries(${MULT_BASE_LIB}
  PUBLIC "-pthread"
  )
