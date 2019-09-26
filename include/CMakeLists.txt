set(HEADER_FILES
        kitsune/ioc/injector
        kitsune/ioc/service)

add_library(ioc INTERFACE)
add_library(kitsune::ioc ALIAS ioc)

target_include_directories(ioc INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(
    DIRECTORY kitsune/ioc
    DESTINATION include/kitsune
)
