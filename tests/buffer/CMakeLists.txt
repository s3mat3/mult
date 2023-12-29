#
# usage cmake -D CMAKE_BUILD_TYPE=(Debug | Release | '') -DCMAKE_EXPORT_COMPILE_COMMANDS=on
#
cmake_minimum_required (VERSION 3.24)
project(buffer-test-build)
set(TARGET_BASE "buffer")

set(TARGET "${TARGET_BASE}")
set(TARGET_MISC_TEST "${TARGET}-misc")
set(TARGET_UNIT_TEST "${TARGET}-unit")
set(TARGET_BENCHMARK "${TARGET_BASE}-benchmark")

set(TEST_TARGET_SOURCES_BASE ${MULT_TEST_BASE}/${TARGET_BASE})

set(UNIT_TEST_TARGET_SOURCES
  ${TEST_TARGET_SOURCES_BASE}/unit_test.cpp
  )
set(BENCHMARK_TARGET_SOURCES
  ${TEST_TARGET_SOURCES_BASE}/bench.cpp
  )

set(EXECUTABLE_OUTPUT_PATH ${MULT_TEST_OUT_DIR}/${TARGET_BASE})
#############
# MISC_TEST #
#############
set(MISC_TEST_TARGET_SOURCES
  ${TEST_TARGET_SOURCES_BASE}/misc_test.cpp
  )
add_executable(${TARGET_MISC_TEST}  ${MISC_TEST_TARGET_SOURCES})
#
target_link_directories(${TARGET_MISC_TEST}
  PUBLIC ${MULT_LIB_OUT_DIR}
  )
#
# link libraries
target_link_libraries(${TARGET_MISC_TEST}
  PUBLIC ${MULT_BASE_LIB}
  )
#
# include files
target_include_directories(${TARGET_MISC_TEST}
  PRIVATE ${TEST_SOURCES_BASE}
  PRIVATE ${TOOLS_TESTER_BASE}
  PUBLIC  ${MULT_INCLUDE_BASE}
  )
target_compile_options(${TARGET_MISC_TEST}
  PRIVATE -O2 -g3 -finline-functions
  PRIVATE -Wall -Wextra -W -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wreorder
  PRIVATE -DMULT_DEBUG -DMULT_ASSERT_CHECK -DMULT_ASSERT_OK -DMULT_TRACE
  )
target_compile_features(${TARGET_MISC_TEST} PRIVATE cxx_std_20)
#############
# UNIT_TEST #
#############
add_executable(${TARGET_UNIT_TEST}  ${UNIT_TEST_TARGET_SOURCES})
#
target_link_directories(${TARGET_UNIT_TEST}
  PUBLIC ${MULT_LIB_OUT_DIR}
  )
#
# link libraries
target_link_libraries(${TARGET_UNIT_TEST}
  PUBLIC ${MULT_BASE_LIB}
  )
#
# include files
target_include_directories(${TARGET_UNIT_TEST}
  PRIVATE ${TEST_SOURCES_BASE}
  PRIVATE ${TOOLS_TESTER_BASE}
  PUBLIC  ${MULT_INCLUDE_BASE}
  )
target_compile_options(${TARGET_UNIT_TEST}
  PRIVATE -O2 -g3 -finline-functions
  PRIVATE -Wall -Wextra -W -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wreorder
  )
target_compile_features(${TARGET_MISC_TEST} PRIVATE cxx_std_20)
#
# test define
add_test(
  NAME ${TARGET_UNIT_TEST}
  COMMAND ${TARGET_UNIT_TEST}
 # CONFIGURATIONS Release
  WORKING_DIRECTORY ${MULT_TEST_OUT_DIR}
  )
#############
# benchmark #
#############
add_executable(${TARGET_BENCHMARK}  ${BENCHMARK_TARGET_SOURCES})
target_link_directories(${TARGET_BENCHMARK}
  PRIVATE ${MULT_LIB_OUT_DIR}
  PRIVATE ${benchmark_SOURCE_DIR}
  )
#
# link libraries このセクションは必ずadd_executableマクロの後ろに記述する必要有り
target_link_libraries(${TARGET_BENCHMARK}
  PRIVATE ${MULT_BASE_LIB}
  PRIVATE "benchmark"
  )
#
# include files
target_include_directories(${TARGET_BENCHMARK}
  PRIVATE ${TEST_SOURCES_BASE}
  PRIVATE ${TOOLS_TESTER_BASE}
  PRIVATE ${MULT_INCLUDE_BASE}
  PRIVATE ${MULT_INTERNAL}
  PRIVATE ${benchmark_SOURCE_DIR}/include/benchmark
  )
target_compile_options(${TARGET_BENCHMARK}
  PRIVATE -O3 -mtune=native -march=native -finline-functions -flto
  PRIVATE -Wall -Wextra -W -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wreorder
  PRIVATE -DMULT_DEBUG_DISABLE -DNDEBUG
  )
target_compile_features(${TARGET_MISC_TEST} PRIVATE cxx_std_20)
