cmake_minimum_required(VERSION 3.22)
project(mu VERSION 0.1.0)
option(mu_ENABLE_TEST "Enable unit-tests" OFF)

add_library(mu INTERFACE)
add_library(mu::mu ALIAS mu)
target_include_directories(
  mu INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
               "$<INSTALL_INTERFACE:include>")
target_compile_features(mu INTERFACE cxx_std_20)

if(mu_ENABLE_TEST)
  enable_testing()
  add_subdirectory(test)
endif()

add_subdirectory(pkg)
