cmake_minimum_required(VERSION 3.14)

if (NOT (DEFINED DOC_ROOT_NAME))
  set(DOC_ROOT_NAME "mult-doc")
endif()

project(${DOC_ROOT_NAME}_project
  LANGUAGES NONE)

## if compile to multi document 
set(PROJ_DOC_MULTI 1) 

## if use subdirectory under correct

if (NOT (DEFINED MULT_BASE_DIR))
  set(MULT_BASE_DIR     ${PROJECT_SOURCE_DIR})
  set(MULT_BUILD_DIR    ${PROJECT_BINARY_DIR})
  set(MULT_DOC_OUT_DIR  ${MULT_BUILD_DIR}/doc)
  set(MULT_3RD_BASE     ${MULT_BASE_DIR}/3rd)
  set(MULT_INCLUDE_BASE ${MULT_BASE_DIR}/include)
  set(MULT_INTERNAL     ${MULT_INCLUDE_BASE}/internal)
  set(MULT_DOC_ROOT     ${MULT_BASE_DIR}/doc-root)
endif()

if(NOT EXISTS ${MULT_DOC_OUT_DIR})
  file(MAKE_DIRECTORY ${MULT_DOC_OUT_DIR})
endif()

find_package(Doxygen)
if(DOXYGEN_FOUND)
        set(DOXYGEN_CONF_FILE "Doxyfile")
        set(DOXYGEN_CONF_PATH   ${MULT_DOC_ROOT}/${DOXYGEN_CONF_FILE})
        set(DOXYGEN_TARGET ${DOC_ROOT_NAME})
        CONFIGURE_FILE(${DOXYGEN_CONF_PATH}.in ${DOXYGEN_CONF_PATH})
        ADD_CUSTOM_TARGET(${DOXYGEN_TARGET}
                        ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONF_PATH}
			            WORKING_DIRECTORY ${MULT_DOC_OUT_DIR})
else(DOXYGEN_FOUND)
        message (WARNING "doxygen binary couldn't be found")
endif(DOXYGEN_FOUND)
