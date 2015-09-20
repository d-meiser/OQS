set(EXAMPLES
    RabiOscillations
   )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  ${PROJECT_BINARY_DIR}
  )
if(OQS_WITH_MBO)
  include_directories(
      ${PROJECT_SOURCE_DIR}/mbo/include
      ${PROJECT_BINARY_DIR}/mbo
      )
endif()

foreach (e ${EXAMPLES})
  add_executable(${e} ${e}.c)
  target_link_libraries(${e} OQS)
  if(OQS_RUN_EXAMPLES_AS_TESTS)
    add_test(${e} ${e})
  endif()
endforeach()

