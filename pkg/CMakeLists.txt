include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Files related to the find-able CMake package get installed here.
set(CMAKE_INSTALL_PACKAGEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# Generate *-config.cmake and *-config-version.cmake files. Install them.
configure_package_config_file(
  cmake_package_config.cmake.in ${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PACKAGEDIR}
  PATH_VARS CMAKE_INSTALL_PACKAGEDIR)
write_basic_package_version_file(${PROJECT_NAME}-config-version.cmake
                                 COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_PACKAGEDIR})

# Install targets and header files to their default installation destinations.
# Also install a *-targets.cmake file next to the *-config.cmake file.
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ TYPE INCLUDE)
install(TARGETS mu EXPORT ${PROJECT_NAME}-targets)
install(
  EXPORT ${PROJECT_NAME}-targets
  DESTINATION ${CMAKE_INSTALL_PACKAGEDIR}
  NAMESPACE ${PROJECT_NAME}::)
