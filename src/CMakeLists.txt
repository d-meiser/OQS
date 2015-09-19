include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )

set(OQS_SRCS
    Integrator.c
    OqsJumpTrajectory.c
   )

add_library(OQS ${OQS_SRCS} ${OQS_HEADERS})

if(UNIX)
  target_link_libraries(OQS m)
  if(MBO_ENABLE_COVERAGE)
    target_link_libraries(OQS gcov)
  endif()
endif()

install(TARGETS OQS DESTINATION lib)


