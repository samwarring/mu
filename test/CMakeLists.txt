find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

add_executable(mu_test mu_test.cpp)
target_link_libraries(mu_test PRIVATE mu::mu GTest::gtest_main)
gtest_discover_tests(mu_test)
