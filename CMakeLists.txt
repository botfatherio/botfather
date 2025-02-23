cmake_minimum_required(VERSION 3.5)
project(botfather)

# Allow find_package to use <PackageName>_ROOT  variables
cmake_policy(SET CMP0074 NEW)

# Enable compilation of libcef_dll_wrapper 3.3578.1861 with gcc 14
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-Wno-error=template-id-cdtor" COMPILER_SUPPORTS_TEMPLATE_ID_CDTOR)
IF (COMPILER_SUPPORTS_TEMPLATE_ID_CDTOR)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=template-id-cdtor -Wno-error=attributes -Wno-error=deprecated-declarations -Wno-error=unused-result")
ENDIF()

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
