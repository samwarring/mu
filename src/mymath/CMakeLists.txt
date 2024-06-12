add_library(mymath mymath.cpp)
target_include_directories(
  mymath
  PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/public>"
         "$<INSTALL_INTERFACE:include>"
  PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/private>")
target_compile_features(mymath PUBLIC cxx_std_17)
