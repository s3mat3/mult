#
# usage cmake -D CMAKE_BUILD_TYPE=(Debug | Release | '') -DCMAKE_EXPORT_COMPILE_COMMANDS=on
#
cmake_minimum_required (VERSION 3.24)
project(storage-test-build)
set(TARGET_BASE "storage")
set(TARGET "${TARGET_BASE}-test")
set(RESULT_UNIT_TEST "${TARGET}-unit-test")
set(TARGET_BENCHMARK "${TARGET_BASE}-benchmark")

set(TEST_TARGET_SOURCES_BASE ${MULT_TEST_BASE}/${TARGET_BASE})

set(TEST_TARGET_SOURCES
  ${TEST_TARGET_SOURCES_BASE}/unit_test.cpp
  )
set(BENCHMARK_TARGET_SOURCES
  ${TEST_TARGET_SOURCES_BASE}/bench.cpp
  )

set(EXECUTABLE_OUTPUT_PATH ${MULT_TEST_OUT_DIR}/${TARGET_BASE})
#
# final executable target
add_executable(${TARGET}  ${TEST_TARGET_SOURCES})
#
target_link_directories(${TARGET}
  PUBLIC ${MULT_LIB_OUT_DIR}
  )
#
# link libraries このセクションは必ずadd_executableマクロの後ろに記述する必要有り
target_link_libraries(${TARGET}
  PUBLIC ${MULT_BASE_LIB}
  )
#
# include files
target_include_directories(${TARGET}
  PRIVATE ${TEST_SOURCES_BASE}
  PRIVATE ${TOOLS_TESTER_BASE}
  PRIVATE  ${MULT_INCLUDE_BASE}
  )
target_compile_options(${TARGET}
  PRIVATE -O2 -g3 -finline-functions -std=c++20
  PRIVATE -Wall -Wextra -W -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wreorder
  )
#
# test define
add_test(
  NAME ${TARGET} # テスト名
  COMMAND ${TARGET} # mylib::hoge というテストのみ実行する
 # CONFIGURATIONS Release # テスト構成がReleaseのときのみ実行
  WORKING_DIRECTORY ${MULT_TEST_OUT_DIR} # 実行ディレクトリは、ビルドディレクトリ直下のtmpディレクトリ
  )
##
# benchmark
#
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
  PRIVATE -O2 -mtune=native -march=native -finline-functions -flto -std=c++20
  PRIVATE -Wall -Wextra -W -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wreorder
  )
#
# test define
add_test(
  NAME ${TARGET_BENCHMARK} # テスト名
  COMMAND ${TARGET_BENCHMARK} # mylib::hoge というテストのみ実行する
 # CONFIGURATIONS Release # テスト構成がReleaseのときのみ実行
  WORKING_DIRECTORY ${MULT_TEST_OUT_DIR} # 実行ディレクトリは、ビルドディレクトリ直下のtmpディレクトリ
  )
