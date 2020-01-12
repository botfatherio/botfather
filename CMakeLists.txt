cmake_minimum_required(VERSION 2.8.12.1)
project(botfather)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

# Add local cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Turn off attribute warnings
add_definitions(-Wno-attributes)

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

# Include application targets.
add_subdirectory(botfather)
add_subdirectory(webhelper)

# Display CEF configuration settings.
PRINT_CEF_CONFIG()
