set(HEADER_FILES 
        kitsune/ioc/Hash.h
        kitsune/ioc/Injector.h
        kitsune/ioc/Service.h
    )

add_library(ioc INTERFACE)

target_include_directories(ioc INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(
    DIRECTORY kitsune/ioc
    DESTINATION include/kitsune
    FILES_MATCHING PATTERN "*.h"
)