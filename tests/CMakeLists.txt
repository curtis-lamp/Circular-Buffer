include(doctest)

find_package(doctest REQUIRED)

add_executable(test_circular_buffer test.cpp)
target_link_libraries(test_circular_buffer PRIVATE doctest::doctest circular_buffer)
doctest_discover_tests(test_circular_buffer)
