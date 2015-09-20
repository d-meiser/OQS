set(OQS_HEADERS
    Oqs.h
    OqsAmplitude.h
    OqsErrors.h
    )
if(OQS_WITH_MBO)
  list(APPEND OQS_HEADERS OqsMbo.h)
endif()

install(FILES ${OQS_HEADERS} ${PROJECT_BINARY_DIR}/MboExport.h DESTINATION include)

