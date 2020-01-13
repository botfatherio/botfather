# ISC License
#
# Copyright (c) 2015 Rui Abreu Ferreira <raf-ep@gmx.com>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.

#
# CMake wrapper to call windeployqt in Windows
#

function(WinDeployQt)
	cmake_parse_arguments(_deploy
		"COMPILER_RUNTIME;FORCE"
		"TARGET"
		"INCLUDE_MODULES;EXCLUDE_MODULES"
		${ARGN}
		)

	if(NOT _deploy_TARGET)
		message(FATAL_ERROR "A TARGET must be specified")
	endif()
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		list(APPEND _ARGS --debug)
	elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		list(APPEND _ARGS --release-with-debug-info)
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		list(APPEND _ARGS --release)
	endif()
	if(_deploy_COMPILER_RUNTIME)
		list(APPEND _ARGS --compiler-runtime)
	endif()
	if(_deploy_FORCE)
		list(APPEND _ARGS --force)
	endif()

	foreach(mod ${_deploy_INCLUDE_MODULES})
		string(TOLOWER ${mod} mod)
		string(REPLACE "qt5::" "" mod ${mod})
		list(APPEND _ARGS "--${mod}")
	endforeach()
	foreach(mod ${_deploy_EXCLUDE_MODULES})
		string(TOLOWER ${mod} mod)
		string(REPLACE "qt5::" "" mod ${mod})
		list(APPEND _ARGS "--no-${mod}")
	endforeach()

	find_program(_deploy_PROGRAM windeployqt
		PATHS $ENV{QTDIR}/bin/)
	if(_deploy_PROGRAM)
		message(STATUS "Found ${_deploy_PROGRAM}")
	else()
		message(FATAL_ERROR "Unable to find windeployqt")
	endif()

	if(COMPILER_RUNTIME AND NOT $ENV{VVVV})
		message(STATUS "not set, the VC++ redistributable installer will NOT be bundled")
	endif()

	add_custom_target(windeployqt_${_deploy_TARGET} ALL ${_deploy_PROGRAM} --dir ${PROJECT_BINARY_DIR}/windeployqt ${_ARGS}
		$<TARGET_FILE:${_deploy_TARGET}>
		DEPENDS ${_deploy_TARGET}
		COMMENT "Preparing Qt runtime dependencies")
endfunction()
