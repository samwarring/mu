find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

add_executable(
  mu_test
  factor_test.cpp
  units_test.cpp
  concrete_factor_test.cpp
  analysis_test.cpp
  primes_test.cpp
  compute_pow_test.cpp
  units_conversion_test.cpp
  rep_test.cpp)
target_link_libraries(mu_test PRIVATE mu::mu GTest::gtest_main)
gtest_discover_tests(mu_test)
