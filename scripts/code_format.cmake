# cmake-format: off
# Usage: cmake [-DCLANG_FORMAT=<path>] [-DCMAKE_FORMAT=<path>] -P code_format.cmake
# cmake-format: on

# Get path to project root
cmake_path(GET CMAKE_SCRIPT_MODE_FILE PARENT_PATH PROJECT_DIR)
cmake_path(GET PROJECT_DIR PARENT_PATH PROJECT_DIR)

# Determine which clang-format to use
if(NOT CLANG_FORMAT)
  set(CLANG_FORMAT "clang-format")
endif()
if(NOT CMAKE_FORMAT)
  set(CMAKE_FORMAT "cmake-format")
endif()

message("---------------------------------------------------------------------")
message("clang-format path: ${CLANG_FORMAT}")
execute_process(COMMAND "${CLANG_FORMAT}" --version COMMAND_ERROR_IS_FATAL ANY)
file(GLOB_RECURSE SRC_FILES "${PROJECT_DIR}/*.cpp" "${PROJECT_DIR}/*.h"
     "${PROJECT_DIR}/*.hpp")
list(FILTER SRC_FILES EXCLUDE REGEX vcpkg/|build/)
list(LENGTH SRC_FILES SRC_FILES_LENGTH)
message("Formatting ${SRC_FILES_LENGTH} files...")
message("---------------------------------------------------------------------")
foreach(SRC_FILE IN LISTS SRC_FILES)
  message("${SRC_FILE}")
  execute_process(COMMAND "${CLANG_FORMAT}" -i "${SRC_FILE}"
                          COMMAND_ERROR_IS_FATAL ANY)
endforeach()

message("---------------------------------------------------------------------")
message("cmake-format path: ${CMAKE_FORMAT}")
execute_process(COMMAND "${CMAKE_FORMAT}" --version COMMAND_ERROR_IS_FATAL ANY)
file(GLOB_RECURSE CMAKE_FILES "${PROJECT_DIR}/CMakeLists.txt"
     "${PROJECT_DIR}/**/CMakeLists.txt" "${PROJECT_DIR}/*.cmake")
list(FILTER CMAKE_FILES EXCLUDE REGEX vcpkg/|build/)
list(LENGTH CMAKE_FILES CMAKE_FILES_LENGTH)
message("Formatting ${CMAKE_FILES_LENGTH} files...")
message("---------------------------------------------------------------------")
foreach(CMAKE_FILE IN LISTS CMAKE_FILES)
  message("${CMAKE_FILE}")
  execute_process(COMMAND "${CMAKE_FORMAT}" -i "${CMAKE_FILE}"
                          COMMAND_ERROR_IS_FATAL ANY)
endforeach()

message("---------------------------------------------------------------------")
message("OK!")
