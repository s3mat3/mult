#
# usage cmake -D CMAKE_BUILD_TYPE=(Debug | Release | '') -DCMAKE_EXPORT_COMPILE_COMMANDS=on
#
cmake_minimum_required (VERSION 3.0)
project(buffer-test-build)
set(TARGET_BASE "buffer")
set(TARGET "${TARGET_BASE}-test")
set(TARGET_PERFORMANCE "${TARGET_BASE}-performance")
set(BUFFER_UNIT_TEST "${TARGET}-unit-test")

set(TEST_TARGET_SOURCES_BASE ${MULT_TEST_BASE}/${TARGET_BASE})

set(TEST_TARGET_SOURCES
  ${TEST_TARGET_SOURCES_BASE}/unit_test.cpp
  )

set(EXECUTABLE_OUTPUT_PATH ${MULT_TEST_OUT_DIR}/${TARGET_BASE})
#
# final executable target
add_executable(${TARGET}  ${TEST_TARGET_SOURCES})
#
# target compile options
# target_compile_options(${TARGET}
#   PUBLIC -Wall  -W -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual
#   )
#
# link libraries このセクションは必ずadd_executableマクロの後ろに記述する必要有り
#
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
  PUBLIC  ${MULT_INCLUDE_BASE}
  PUBLIC  ${MULT_INTERNAL}
  )
##
# measurement performance
#
add_executable(${TARGET_PERFORMANCE}  ${TEST_TARGET_SOURCES_BASE}/measure.cpp)
target_link_directories(${TARGET_PERFORMANCE}
  PUBLIC ${MULT_LIB_OUT_DIR}
  PUBLIC "/opt/tools/lib"
  )
#
# link libraries このセクションは必ずadd_executableマクロの後ろに記述する必要有り
target_link_libraries(${TARGET_PERFORMANCE}
  PRIVATE ${MULT_BASE_LIB}
  PRIVATE "benchmark"
  PRIVATE "pthread"
  )
#
# include files
target_include_directories(${TARGET_PERFORMANCE}
  PRIVATE ${TEST_SOURCES_BASE}
  PRIVATE ${TOOLS_TESTER_BASE}
  PUBLIC  ${MULT_INCLUDE_BASE}
  PUBLIC  ${MULT_INTERNAL}
  PUBLIC  "/opt/tools/include"
  )
target_compile_options(${TARGET_PERFORMANCE}
  PRIVATE -O2
  PRIVATE -Wall -Wextra -W -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wreorder
  )
#
# test define
add_test(
  NAME ${TARGET} # テスト名は hoge_test
  COMMAND ${TARGET} # mylib::hoge というテストのみ実行する
 # CONFIGURATIONS Release # テスト構成がReleaseのときのみ実行
  WORKING_DIRECTORY ${MULT_TEST_OUT_DIR} # 実行ディレクトリは、ビルドディレクトリ直下のtmpディレクトリ
  )
