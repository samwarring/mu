# cmake-format: off
# Usage: scrape_code_blocks(INPUT_FILE <input file>
#                           OUTPUT_FILE_PREFIX <prefix>
#                           OUTPUT_FILE_LIST <result var>)
# cmake-format: on
#
# Scrapes code blocks from markdown file specified in <input file>. Code blocks
# begin with "```cpp" on its own line, and end with "```" on its own line. Each
# code block is dumped to a file named <prefix>_N.cpp, where N is a number. The
# list of generated cpp files is stored in <result var>.
#
function(scrape_code_blocks)
  # Parse args
  set(OPTIONS)
  set(ONE_VALUE_ARGS INPUT_FILE OUTPUT_FILE_PREFIX OUTPUT_FILE_LIST)
  set(MULTI_VALUE_ARGS)
  cmake_parse_arguments(ARGS "${OPTIONS}" "${ONE_VALUE_ARGS}"
                        "${MULTI_VALUE_ARGS}" ${ARGN})

  # Read file as list of strings. Iterate over the strings.
  file(STRINGS "${ARGS_INPUT_FILE}" INPUT_FILE_CONTENTS ENCODING UTF-8)
  set(IN_CODE_BLOCK OFF)
  set(CODE_BLOCK_NUM 0)
  set(CODE_BLOCK_FILE "")
  set(CODE_BLOCK_FILE_LIST)
  foreach(INPUT_FILE_LINE IN LISTS INPUT_FILE_CONTENTS)

    # Lines containing ```cpp begin a code block
    string(FIND "${INPUT_FILE_LINE}" "```cpp" FOUND_START)
    if(NOT ${FOUND_START} EQUAL -1)
      set(IN_CODE_BLOCK ON)
      set(CODE_BLOCK_FILE "${ARGS_OUTPUT_FILE_PREFIX}_${CODE_BLOCK_NUM}.cpp")
      list(APPEND CODE_BLOCK_FILE_LIST "${CODE_BLOCK_FILE}")
      file(WRITE "${CODE_BLOCK_FILE}" )
      continue()
    endif()

    # Lines containing ``` end a code block
    string(FIND "${INPUT_FILE_LINE}" "```" FOUND_END)
    if(NOT ${FOUND_END} EQUAL -1)
      set(IN_CODE_BLOCK OFF)
      math(EXPR CODE_BLOCK_NUM "${CODE_BLOCK_NUM} + 1")
      continue()
    endif()

    # If inside a code block, line to the current output file.
    if(IN_CODE_BLOCK)
      file(APPEND "${CODE_BLOCK_FILE}" "${INPUT_FILE_LINE}\n")
    endif()
  endforeach()

  # Store list of generated files in the output argument
  set(${ARGS_OUTPUT_FILE_LIST}
      "${CODE_BLOCK_FILE_LIST}"
      PARENT_SCOPE)

endfunction()

# Scrape examples from the readme.
configure_file("${PROJECT_SOURCE_DIR}/README.md" README.md)
scrape_code_blocks(
  INPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/README.md"
  OUTPUT_FILE_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/readme_example"
  OUTPUT_FILE_LIST README_EXAMPLES)

# Build each example from the readme as its own executable.
foreach(README_EXAMPLE IN LISTS README_EXAMPLES)
  cmake_path(GET README_EXAMPLE STEM EXAMPLE_NAME)
  add_executable(${EXAMPLE_NAME} "${README_EXAMPLE}")
  target_link_libraries(${EXAMPLE_NAME} PRIVATE mu::mu)
  add_test(NAME ${EXAMPLE_NAME} COMMAND ${EXAMPLE_NAME})
endforeach()
