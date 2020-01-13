cmake_minimum_required(VERSION 2.8.12.1)

# Allow find_package to use <PackageName>_ROOT  variables
cmake_policy(SET CMP0074 NEW)

# Allow the project command to manage the VERSION variables
cmake_policy(SET CMP0048 NEW)

project(botfather VERSION 6.2.1)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

# Add local cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include QHotkey
option(QHOTKEY_EXAMPLES OFF)
add_subdirectory(3rdparty/QHotkey)
include_directories(3rdparty/QHotkey/QHotkey)

# Include system wide installed third party libraries
find_package(git2 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(CEF REQUIRED)

# Include the libcef_dll_wrapper target.
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

# Set build output dir for application targets.
set(BOTFATHER_BIN_OUT_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}Output)

# Include application targets.
add_subdirectory(botfather)
add_subdirectory(webhelper)

# Display CEF configuration settings.
PRINT_CEF_CONFIG()
