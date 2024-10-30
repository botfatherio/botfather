cmake_minimum_required(VERSION 3.5)
project(webhelper LANGUAGES CXX)

set(WEBHELPER_SRCS
    main.cpp
    helper_app.hpp
    helper_app.cpp
    bf_converter.hpp
    bf_converter.cpp
    bf_debug_tools.hpp
    bf_debug_tools.cpp
    webhelper.exe.manifest
)
APPEND_PLATFORM_SOURCES(WEBHELPER_SRCS)
source_group(${PROJECT_NAME} FILES ${WEBHELPER_SRCS})

#
# Shared CEF configuration.
#

# Logical target used to link the libcef library
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

# Determine the target output directory.
#SET_CEF_TARGET_OUT_DIR()
set(CEF_TARGET_OUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

#
# Shared Qt5 configuration
#

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)
set(QTLIBS Qt5::Core)

#
# Linux configuration.
#

if(OS_LINUX)
    # Helper executable target.
    add_executable(${PROJECT_NAME} ${WEBHELPER_SRCS})
    SET_EXECUTABLE_TARGET_PROPERTIES(${PROJECT_NAME})
    add_dependencies(${PROJECT_NAME} libcef_dll_wrapper)
    target_link_libraries(${PROJECT_NAME} libcef_dll_wrapper libcef_lib ${CEF_STANDARD_LIBS})
    target_link_libraries(${PROJECT_NAME} ${QTLIBS})

    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
    set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BOTFATHER_BIN_OUT_DIR})

    # Copy binary and resource files to the target output directory.
    COPY_FILES("${PROJECT_NAME}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${BOTFATHER_BIN_OUT_DIR}")
    COPY_FILES("${PROJECT_NAME}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${BOTFATHER_BIN_OUT_DIR}")

    # Set SUID permissions on the chrome-sandbox target.
    SET_LINUX_SUID_PERMISSIONS("${PROJECT_NAME}" "${BOTFATHER_BIN_OUT_DIR}/chrome-sandbox")
endif()

#
# Windows configuration.
#

if(OS_WINDOWS)
    # Helper Executable target.
    add_executable(${PROJECT_NAME} WIN32 ${WEBHELPER_SRCS})
    SET_EXECUTABLE_TARGET_PROPERTIES(${PROJECT_NAME})
    add_dependencies(${PROJECT_NAME} libcef_dll_wrapper)
    target_link_libraries(${PROJECT_NAME} libcef_dll_wrapper libcef_lib ${CEF_STANDARD_LIBS})
    target_link_libraries(${PROJECT_NAME} ${QTLIBS})

	# Do not treat warnings as errors
	target_compile_options(${PROJECT_NAME} PRIVATE /WX-)

    if(USE_SANDBOX)
        # Logical target used to link the cef_sandbox library.
        ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
        target_link_libraries(${PROJECT_NAME} cef_sandbox_lib ${CEF_SANDBOX_STANDARD_LIBS})
    endif()

    # Add the custom manifest files to the executable.
    ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${PROJECT_NAME}" "exe")

    # Copy binary and resource files to the target output directory.
    COPY_FILES("${PROJECT_NAME}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${BOTFATHER_BIN_OUT_DIR}")
    COPY_FILES("${PROJECT_NAME}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${BOTFATHER_BIN_OUT_DIR}")
	
	# Deploy Qt DLLs and resources
	include(WinDeployQt)
	WinDeployQt(TARGET ${PROJECT_NAME} OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COMPILER_RUNTIME INCLUDE_MODULES ${QTLIBS})
endif()