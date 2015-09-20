include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  ${CMAKE_CURRENT_SOURCE_DIR}/../src
  ${PROJECT_SOURCE_DIR}/mbo/include
  ${PROJECT_BINARY_DIR}/mbo
  ${PROJECT_BINARY_DIR}
  ${GTEST_INCLUDE_DIRS}
  )

set(TESTS
  test_Integrator
  test_OqsJumpTrajectory
  )
if(OQS_WITH_MBO)
  list(APPEND TESTS test_OqsMbo)
endif()

foreach(t ${TESTS})
  add_executable(${t} ${t}.cpp)
  target_link_libraries(${t} OQS gtest_main gtest)
  if(OQS_WITH_MBO)
    target_link_libraries(${t} MBO)
  endif()
  if(OQS_ENABLE_COVERAGE)
    target_link_libraries(${t} "${COV_LIBRARIES}")
  endif()
  add_test(${t} ${t})
endforeach()


