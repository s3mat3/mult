#
# usage cmake -D CMAKE_BUILD_TYPE=(Debug | Release | '') -DCMAKE_EXPORT_COMPILE_COMMANDS=on
#
cmake_minimum_required (VERSION 3.14)
project(fsm-example-build)
set(TARGET_BASE "controller")
set(TARGET "${TARGET_BASE}-example")
set(FSM_EXAMPLE ${TARGET})

set(EXAMPLE_SOURCES_BASE ${MULT_EXAMPLE_BASE}/${TARGET_BASE})

set(EXAMPLE_TARGET_SOURCES
  ${EXAMPLE_SOURCES_BASE}/example.cpp
  ${EXAMPLE_SOURCES_BASE}/signal_controller.cpp
  ${EXAMPLE_SOURCES_BASE}/signal_machine.cpp
  ${EXAMPLE_SOURCES_BASE}/idle.cpp
  ${EXAMPLE_SOURCES_BASE}/red.cpp
  ${EXAMPLE_SOURCES_BASE}/yellow.cpp
  ${EXAMPLE_SOURCES_BASE}/green.cpp
  )

set(EXECUTABLE_OUTPUT_PATH ${MULT_EXAMPLE_OUT_DIR}/${TARGET_BASE})
link_directories(${MULT_LIB_OUT_DIR})
#
# final executable target
add_executable(${TARGET}  ${EXAMPLE_TARGET_SOURCES})
#
#
target_link_directories(${TARGET}
  PUBLIC ${MULT_LIB_OUT_DIR}
  )
#
# link libraries このセクションは必ずadd_executableマクロの後ろに記述する必要有り
target_link_libraries(${TARGET}
  PUBLIC ${MULT_BASE_LIB}
  PUBLIC pthread
  )
#
# include files
target_include_directories(${TARGET}
  PRIVATE ${EXAMPLE_SOURCES_BASE}
  PRIVATE ${TOOLS_TESTER_BASE}
  PUBLIC  ${MULT_INCLUDE_BASE}
  )
