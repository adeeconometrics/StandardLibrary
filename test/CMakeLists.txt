enable_testing()

add_executable(array_test array_test.cpp)
target_link_libraries(array_test
    PRIVATE
    array_lib
    gtest
    gtest_main
)

add_test(NAME array_test COMMAND array_test)
