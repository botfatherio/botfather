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

# Include and config libgit2
find_package(libgit2 REQUIRED)
include_directories(${LIBGIT2_INCLUDE_DIR})

# Include and config OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Include and config CEF
set(CEF_RUNTIME_LIBRARY_FLAG "/MD" CACHE STRING "")
option(USE_ATL OFF)
option(USE_SANDBOX OFF)
find_package(CEF REQUIRED)

# Include the libcef_dll_wrapper target.
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

# Set build output directories
if(GEN_NINJA OR GEN_MAKEFILES)
	set(BOTFATHER_BIN_OUT_DIR ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
else()
	set(BOTFATHER_BIN_OUT_DIR "${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BOTFATHER_BIN_OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BOTFATHER_BIN_OUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BOTFATHER_BIN_OUT_DIR})

# Include application targets.
add_subdirectory(botfather)
add_subdirectory(webhelper)

# Display CEF configuration settings.
PRINT_CEF_CONFIG()
