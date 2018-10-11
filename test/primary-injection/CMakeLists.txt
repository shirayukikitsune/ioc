add_executable(primary-injection-test test.cpp)

target_include_directories(primary-injection-test PUBLIC ..)
target_link_libraries(primary-injection-test PUBLIC ioc)

add_test(
    NAME
        primary-injection-test
    COMMAND
        primary-injection-test --reporter=crash
)
