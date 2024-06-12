add_executable(mymath_test mymath_test.cpp)
target_link_libraries(mymath_test PRIVATE GTest::gtest_main mymath)
gtest_discover_tests(mymath_test)
