cmake_minimum_required(VERSION 2.6.0)
project(OQS)
enable_testing()
include(GenerateExportHeader)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
include(AddCCompilerFlagIfSupported)
include(AddCLinkerFlagIfSupported)

option(OQS_RUN_EXAMPLES_AS_TESTS 
    "Whether to run the examples as part of the test suite" OFF)
option(OQS_ENABLE_COVERAGE
    "Instrument tests for coverage" OFF)
option(OQS_BUILD_DOCS
    "Whether to build documentation" OFF)
option(OQS_BUILD_TESTS
    "Whether to build tests" OFF)
option(OQS_WITH_MBO
    "Whether to build with MBO support" OFF)

set(COV_LIBRARIES "")
if(CMAKE_COMPILER_IS_GNUCC)
  set(WARNING_FLAGS "-Wall")
  add_c_compiler_flag_if_supported("-Wpedantic")
  add_c_compiler_flag_if_supported("-fdiagnostics-color=auto")
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_c_compiler_flag_if_supported("-fsanitize=address")
  endif()
  if(OQS_ENABLE_COVERAGE)
    add_definitions("-ftest-coverage -fprofile-arcs")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ftest-coverage -fprofile-arcs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage -fprofile-arcs")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ftest-coverage -fprofile-arcs")
    set(COV_LIBRARIES gcov)
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long -Wno-variadic-macros")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 ${CC_FLAGS} ${WARNING_FLAGS}")
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  get_filename_component(CLANG_PATH ${CMAKE_C_COMPILER} PATH)
  if(NOT DEFINED CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER ${CLANG_PATH}/clang++)
  endif()
  set(WARNING_FLAGS "-Wall -Wpedantic")
  if(OQS_ENABLE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ftest-coverage -fprofile-arcs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage -fprofile-arcs")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ftest-coverage -fprofile-arcs")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -ftest-coverage -fprofile-arcs")
    set(COV_LIBRARIES gcov)
  endif()
  set(CMAKE_C_FLAGS 
      "${CMAKE_C_FLAGS} -std=c99 -fdiagnostics-color=auto ${WARNING_FLAGS}")
  set(CMAKE_C_FLAGS_DEBUG 
      "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address")
  set(CMAKE_CXX_FLAGS_DEBUG 
      "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel")
endif()

configure_file(OqsConfig.h.in OqsConfig.h)
install(FILES ${CMAKE_BINARY_DIR}/OqsConfig.h DESTINATION include)
include_directories(${PROJECT_BINARY_DIR})

if(OQS_WITH_MBO)
  add_subdirectory(mbo)
endif()
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(examples)
if(OQS_BUILD_TESTS)
  add_subdirectory(gtest-1.7.0)
  include_directories(${gtest_SOURCE_DIR}/include)
  add_subdirectory(tests)
endif()

generate_export_header(OQS EXPORT_FILE_NAME OqsExport.h)
install(FILES ${CMAKE_BINARY_DIR}/OqsExport.h DESTINATION include)

if(OQS_BUILD_DOCS)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doxyfile @ONLY)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doc")
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxyfile
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doc"
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
  else()
    message(STATUS "Doxygen not found -- not building docs")
  endif()
endif()

