set(OQS_HEADERS
    Oqs.h
    OqsAmplitude.h
    OqsErrors.h
    )

install(FILES ${OQS_HEADERS} ${PROJECT_BINARY_DIR}/MboExport.h DESTINATION include)

