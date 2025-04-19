enable_testing()

add_executable(test_array test_array.cpp)
target_link_libraries(test_array
    PRIVATE
    standard_lib
    gtest
    gtest_main
)

add_test(NAME test_array COMMAND test_array)
