#
# usage cmake -D CMAKE_BUILD_TYPE=(Debug | Release | '') -DCMAKE_EXPORT_COMPILE_COMMANDS=on
#
cmake_minimum_required (VERSION 3.14)
project(notification-example-build)
set(TARGET_BASE "notification")
set(TARGET "${TARGET_BASE}-example")
set(NOTIFICATION_EXAMPLE ${TARGET})

set(EXAMPLE_TARGET_SOURCES_BASE ${MULT_EXAMPLE_BASE}/${TARGET_BASE})

set(EXAMPLE_TARGET_SOURCES
  ${EXAMPLE_TARGET_SOURCES_BASE}/example.cpp
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
  PRIVATE "-pthread"
  )
#
# include files
target_include_directories(${TARGET}
  PRIVATE ${EXAMPLE_SOURCES_BASE}
  PRIVATE  ${MULT_INCLUDE_BASE}
  )
target_compile_options(${TARGET}
  PRIVATE -O2 -g3 -finline-functions -pthread
  PRIVATE -Wall -Wextra -W -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wreorder
  PRIVATE -DMULT_DEBUG -DMULT_ASSERT_CHECK -DMULT_ASSERT_OK -DMULT_TRACE
  )
target_compile_features(${TARGET} PRIVATE cxx_std_20)
