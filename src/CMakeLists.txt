include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${PROJECT_BINARY_DIR}
    )
if(OQS_WITH_MBO)
  include_directories(
      ${PROJECT_SOURCE_DIR}/mbo/include
      ${PROJECT_BINARY_DIR}/mbo
      )
endif()

set(OQS_SRCS
    Integrator.c
    OqsJumpTrajectory.c
   )
if(OQS_WITH_MBO)
  list(APPEND OQS_SRCS OqsMbo.c)
endif()

add_library(OQS ${OQS_SRCS} ${OQS_HEADERS})

if(UNIX)
  target_link_libraries(OQS m)
  if(MBO_ENABLE_COVERAGE)
    target_link_libraries(OQS gcov)
  endif()
endif()

install(TARGETS OQS DESTINATION lib)


