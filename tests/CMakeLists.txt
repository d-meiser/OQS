include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  ${CMAKE_CURRENT_SOURCE_DIR}/../src
  ${PROJECT_BINARY_DIR}
  ${GTEST_INCLUDE_DIRS}
  )

set(TESTS
  test_Integrator
  test_OqsJumpTrajectory
  )

foreach(t ${TESTS})
  add_executable(${t} ${t}.cpp)
  target_link_libraries(${t} OQS gtest_main gtest)
  if(ENABLE_COVERAGE)
    target_link_libraries(${t} "${COV_LIBRARIES}")
  endif()
  add_test(${t} ${t})
endforeach()


