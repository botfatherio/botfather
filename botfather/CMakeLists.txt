cmake_minimum_required(VERSION 3.5)
project(botfather VERSION 6.7.0)

configure_file(version.hpp.in version.hpp)

set(BOTFATHER_SRCS
    auth/auth_settings.hpp
    auth/license_api_client.hpp
    auth/rest_api_client.hpp
    auth/scripts_api_client.hpp
    engine/modules/android/apis/android_api.hpp
    engine/modules/desktop/apis/desktop_api.hpp
    engine/modules/common/apis/helper_api.hpp
    engine/modules/vision/apis/vision_api.hpp
    engine/engine.hpp
    engine/modules/algorithm/dijkstra.hpp
    engine/modules/android/adb_device_info.hpp
    engine/modules/android/adb_wrapper.hpp
    engine/modules/android/android_settings.hpp
    engine/modules/browser/browser_life_span_handler.hpp
    engine/modules/browser/browser_load_handler.hpp
    engine/modules/browser/browser_render_handler.hpp
    engine/modules/browser/browser_request_handler.hpp
    engine/modules/browser/browser_settings.hpp
    engine/modules/desktop/desktop.hpp
    engine/modules/desktop/keymap.hpp
    engine/modules/vision/vision.hpp
    engine/modules/common/prototypes/abstract_prototype.hpp
    engine/modules/common/prototypes/color_prototype.hpp
    engine/modules/vision/prototypes/image_prototype.hpp
    engine/modules/vision/prototypes/match_prototype.hpp
    engine/modules/common/prototypes/point_prototype.hpp
    engine/modules/common/prototypes/rect_prototype.hpp
    engine/modules/common/prototypes/size_prototype.hpp
    engine/modules/vision/types/match.hpp
    git/abstract_git_operation.hpp
    git/git_behind_operation.hpp
    git/git_clone_operation.hpp
    git/git_fetch_operation.hpp
    git/git_reclone_operation.hpp
    gui/androiddialog/android_dialog.hpp
    gui/botwidgets/abstract_bot_widget.hpp
    gui/botwidgets/bot_browsers_widget.hpp
    gui/botwidgets/bot_config_widget.hpp
    gui/botwidgets/bot_log_widget.hpp
    gui/botwidgets/bot_settings_widget.hpp
    gui/botwidgets/bot_updates_widget.hpp
    gui/botwidgets/bot_widget.hpp
    gui/browserwindow/browser_address_bar.hpp
    gui/browserwindow/browser_widget.hpp
    gui/browserwindow/browser_window.hpp
    engine/modules/common/apis/config_api.hpp
    engine/modules/common/apis/stats_api.hpp
    engine/modules/browser/adapters/cef_key_event_adapter.hpp
    engine/modules/browser/browser_app.hpp
    engine/modules/browser/browser_client.hpp
    engine/modules/browser/browser_creator.hpp
    engine/modules/browser/browser_list_model.hpp
    engine/modules/browser/browser_manager.hpp
    engine/modules/browser/browser_util.hpp
    engine/modules/common/bf_key_mapper.hpp
    engine/modules/common/bf_key_traits.hpp
    engine/modules/common/bf_keymap.hpp
    engine/modules/common/qt_key_traits.hpp
    engine/modules/common/qt_keymaps.hpp
    engine/modules/common/windows_keyboard_codes.hpp
    engine/modules/browser/prototypes/browser_prototype.hpp
    engine/modules/common/prototypes/timer_prototype.hpp
    engine/modules/browser/types/browser.hpp
    gui/main_window.hpp
    gui/maintenancetool/maintenance_tool.hpp
    gui/models/bot.hpp
    gui/models/bot_config.hpp
    gui/models/bot_config_group.hpp
    gui/models/bot_config_option.hpp
    gui/models/bot_list_model.hpp
    gui/models/bot_manifest.hpp
    gui/models/bot_manifest_node.hpp
    gui/models/bot_repo.hpp
    gui/authdialog/auth_dialog.hpp
    engine/modules/common/apis/abstract_api.hpp
    engine/modules/vision/types/blob_tpl.hpp
    engine/modules/vision/prototypes/blob_tpl_prototype.hpp
    engine/modules/algorithm/apis/algorithm_api.hpp
    engine/modules/common/prototypes/margins_prototype.hpp
    engine/modules/common/prototypes/url_prototype.hpp
    gui/models/bot_repo_list_model.hpp
    gui/preferences/preferences_dialog.hpp
    gui/scriptmanager/git_progress_dialog.hpp
    gui/scriptmanager/script_manager_dialog.hpp
    res/resource.hpp
    shared/q_settings_json_format.hpp
    auth/license_api_client.cpp
    auth/rest_api_client.cpp
    auth/scripts_api_client.cpp
    engine/modules/android/apis/android_api.cpp
    engine/modules/desktop/apis/desktop_api.cpp
    engine/modules/common/apis/helper_api.cpp
    engine/modules/vision/apis/vision_api.cpp
    engine/engine.cpp
    engine/modules/algorithm/dijkstra.cpp
    engine/modules/android/adb_device_info.cpp
    engine/modules/android/adb_wrapper.cpp
    engine/modules/browser/browser_life_span_handler.cpp
    engine/modules/browser/browser_load_handler.cpp
    engine/modules/browser/browser_render_handler.cpp
    engine/modules/browser/browser_request_handler.cpp
    engine/modules/vision/vision.cpp
    engine/modules/common/prototypes/abstract_prototype.cpp
    engine/modules/common/prototypes/color_prototype.cpp
    engine/modules/vision/prototypes/image_prototype.cpp
    engine/modules/vision/prototypes/match_prototype.cpp
    engine/modules/common/prototypes/point_prototype.cpp
    engine/modules/common/prototypes/rect_prototype.cpp
    engine/modules/common/prototypes/size_prototype.cpp
    engine/modules/vision/types/match.cpp
    git/abstract_git_operation.cpp
    git/git_behind_operation.cpp
    git/git_clone_operation.cpp
    git/git_fetch_operation.cpp
    git/git_reclone_operation.cpp
    gui/androiddialog/android_dialog.cpp
    gui/botwidgets/abstract_bot_widget.cpp
    gui/botwidgets/bot_browsers_widget.cpp
    gui/botwidgets/bot_config_widget.cpp
    gui/botwidgets/bot_log_widget.cpp
    gui/botwidgets/bot_settings_widget.cpp
    gui/botwidgets/bot_updates_widget.cpp
    gui/botwidgets/bot_widget.cpp
    gui/browserwindow/browser_address_bar.cpp
    gui/browserwindow/browser_widget.cpp
    gui/browserwindow/browser_window.cpp
    engine/modules/browser/adapters/cef_key_event_adapter.cpp
    engine/modules/common/bf_key_mapper.cpp
    gui/main_window.cpp
    gui/maintenancetool/maintenance_tool.cpp
    gui/models/bot_config.cpp
    gui/models/bot_config_group.cpp
    gui/models/bot_config_option.cpp
    gui/models/bot_list_model.cpp
    gui/models/bot_manifest.cpp
    gui/models/bot_manifest_node.cpp
    gui/models/bot_repo.cpp
    gui/models/bot_repo_list_model.cpp
    gui/preferences/preferences_dialog.cpp
    gui/authdialog/auth_dialog.cpp
    gui/scriptmanager/git_progress_dialog.cpp
    gui/scriptmanager/script_manager_dialog.cpp
    version.hpp
    main.cpp
    engine/modules/common/apis/abstract_api.cpp
    engine/modules/vision/types/blob_tpl.cpp
    engine/modules/vision/prototypes/blob_tpl_prototype.cpp
    engine/modules/algorithm/apis/algorithm_api.cpp
    engine/modules/common/prototypes/margins_prototype.cpp
    engine/modules/common/prototypes/url_prototype.cpp
    engine/modules/browser/browser_list_model.cpp
    engine/modules/browser/browser_manager.cpp
    engine/modules/common/prototypes/timer_prototype.cpp
    gui/models/bot.cpp
    engine/modules/common/apis/config_api.cpp
    engine/modules/common/apis/stats_api.cpp
    engine/modules/browser/prototypes/browser_prototype.cpp
    engine/modules/browser/types/browser.cpp
    engine/modules/browser/browser_app.cpp
    engine/modules/browser/browser_client.cpp
    engine/modules/browser/browser_creator.cpp
    engine/modules/browser/browser_util.cpp
    shared/q_settings_json_format.cpp
    gui/androiddialog/android_dialog.ui
    gui/authdialog/auth_dialog.ui
    gui/botwidgets/bot_browsers_widget.ui
    gui/botwidgets/bot_config_widget.ui
    gui/botwidgets/bot_log_widget.ui
    gui/botwidgets/bot_settings_widget.ui
    gui/botwidgets/bot_updates_widget.ui
    gui/browserwindow/browser_window.ui
    gui/main_window.ui
    gui/preferences/preferences_dialog.ui
    gui/scriptmanager/git_progress_dialog.ui
    gui/scriptmanager/script_manager_dialog.ui
    gui/botwidgets/bot_stats_widget.hpp
    gui/botwidgets/bot_stats_widget.cpp
    gui/botwidgets/bot_stats_widget.ui
    gui/color_finder/color_finder.hpp
    gui/color_finder/color_finder.cpp
    gui/color_finder/color_finder.ui
    gui/color_finder_preview/color_finder_preview.hpp
    gui/color_finder_preview/color_finder_preview.cpp
    res/botfather.qrc
)
set(BOTFATHER_SRCS_LINUX
    engine/modules/desktop/desktop_x11.cpp
    engine/modules/desktop/linux/input-event-codes.hpp
    engine/modules/desktop/linux/uinput.hpp
)
set(BOTFATHER_SRCS_WINDOWS
    res/botfather.exe.manifest
    res/botfather.rc
    res/resource.hpp
    engine/modules/desktop/desktop_win.cpp
)
APPEND_PLATFORM_SOURCES(BOTFATHER_SRCS)
source_group(${PROJECT_NAME} FILES ${BOTFATHER_SRCS})

set(CEFSIMPLE_RES_SRCS_WINDOWS
    res/appicon.ico
)
APPEND_PLATFORM_SOURCES(CEFSIMPLE_RES_SRCS)
source_group(${PROJECT_NAME}\\\\res FILES ${CEFSIMPLE_RES_SRCS})

set(BOTFATHER_SRCS
    ${BOTFATHER_SRCS}
    ${CEFSIMPLE_RES_SRCS}
)

#
# Shared CEF configuration.
#

# Logical target used to link the libcef library
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

# Override otherwise breaking settings set by CEF
if (OS_LINUX)
	list(REMOVE_ITEM CEF_CXX_COMPILER_FLAGS -fno-rttii -fno-exceptions)
	list(APPEND CEF_CXX_COMPILER_FLAGS -frtti -fexceptions)
endif()

# Determine the target output directory.
#SET_CEF_TARGET_OUT_DIR()
set(CEF_TARGET_OUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

#
# Shared Qt5 configuration.
#

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Widgets Script Gui Multimedia Network REQUIRED)
set(QTLIBS Qt5::Core Qt5::Widgets Qt5::Script Qt5::Gui Qt5::Multimedia Qt5::Network)

#
# Linux configuration.
#

if(OS_LINUX)
    find_package(X11 REQUIRED)
    find_package(Qt5X11Extras REQUIRED)
	set(QTLIBS ${QTLIBS} Qt5::X11Extras)

    # Main executable target.
    add_executable(${PROJECT_NAME} ${BOTFATHER_SRCS})
    SET_EXECUTABLE_TARGET_PROPERTIES(${PROJECT_NAME})
    add_dependencies(${PROJECT_NAME} libcef_dll_wrapper qhotkey)
    target_link_libraries(${PROJECT_NAME} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})
    target_link_libraries(${PROJECT_NAME} ${QTLIBS})
    target_link_libraries(${PROJECT_NAME} qhotkey)
    target_link_libraries(${PROJECT_NAME} ${LIBGIT2_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} opencv_features2d opencv_flann opencv_imgcodecs opencv_imgproc opencv_core opencv_highgui)

    # Set rpath so that libraries can be placed next to the executable.
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
    set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})

    # Copy binary and resource files to the target output directory.
    COPY_FILES("${PROJECT_NAME}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
    COPY_FILES("${PROJECT_NAME}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
    COPY_FILES("${PROJECT_NAME}" "adb" "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/ADB/linux/" "${CEF_TARGET_OUT_DIR}")
endif()

#
# Windows configuration.
#

if(OS_WINDOWS)
    find_package(Qt5WinExtras REQUIRED)
	set(QTLIBS ${QTLIBS} Qt5::WinExtras)

    # Executable target.
    add_executable(${PROJECT_NAME} WIN32 ${BOTFATHER_SRCS})
    SET_EXECUTABLE_TARGET_PROPERTIES(${PROJECT_NAME})
    add_dependencies(${PROJECT_NAME} libcef_dll_wrapper qhotkey)
    target_link_libraries(${PROJECT_NAME} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})
    target_link_libraries(${PROJECT_NAME} ${QTLIBS})
    target_link_libraries(${PROJECT_NAME} qhotkey)
    target_link_libraries(${PROJECT_NAME} ${LIBGIT2_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} opencv_features2d opencv_flann opencv_imgcodecs opencv_imgproc opencv_core opencv_highgui)

	# Do not tread warnings as errors
	target_compile_options(${PROJECT_NAME} PRIVATE /WX-)

    # Add the custom manifest files to the executable.
    ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}/res" "${PROJECT_NAME}" "exe")

    # Copy binary and resource files to the target output directory.
    COPY_FILES("${PROJECT_NAME}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
    COPY_FILES("${PROJECT_NAME}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
    COPY_FILES("${PROJECT_NAME}" "adb" "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/ADB/windows/" "${CEF_TARGET_OUT_DIR}")

	# Deploy Qt DLLs and resources
	include(WinDeployQt)
	WinDeployQt(TARGET ${PROJECT_NAME} OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COMPILER_RUNTIME INCLUDE_MODULES ${QTLIBS})
endif()
