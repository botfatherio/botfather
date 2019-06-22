TEMPLATE = app
TARGET = botfather
QT += core network widgets script gui multimedia

DESTDIR = ./BuildOutput
OBJECTS_DIR += ./CompiledObjects
MOC_DIR += ./GeneratedFiles
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Disable output from qDebug() like mecros in release builds.
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

include(../3rdparty/QHotkey/qhotkey.pri)
include(../3rdparty/CEF/CEF.pri)
include(../3rdparty/OpenCV/OpenCV.pri)
include(../3rdparty/ADB/ADB.pri)
include(../3rdparty/libgit2/libgit2.pri)
include(../3rdparty/OpenSSL/OpenSSL.pri)

unix {
    # Disables some warnings
    QMAKE_CXXFLAGS += -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wno-missing-field-initializers # libgit2 would otherwise spam a lot

    # Newer distros set GCC default link flag -pie, which marks e_type as ET_DYN on the binary file.
    # Consequently, the Operating System recognizes them as Shared Library.
    # Using -no-pie our project will be a executable, not a shared libary.
    #QMAKE_LFLAGS += -no-pie

    # This tells the ELF to look for required libraries right next to the binary first.
    # That way libcef.so and other shared objects can be loaded which otherwise couldn't be found.
    QMAKE_RPATHDIR += ./

    LIBS += -lX11

    # Dramatically reduces shared libary file size after linking
    CONFIG(release, debug|release):QMAKE_POST_LINK += find $${DESTDIR} -type f -name \*.so | xargs strip --strip-all

    SOURCES += engine/modules/desktop/desktop_x11.cpp
    HEADERS += \
        engine/modules/desktop/linux/input-event-codes.hpp \
        engine/modules/desktop/linux/uinput.hpp
}

win32 {
    QT += winextras
    DEFINES += _UNICODE WIN64 QT_DLL QT_WIDGETS_LIB QT_QML_LIB QT_NETWORK_LIB

    # Disable "unreferenced formal parameter" warning on windows
    QMAKE_CXXFLAGS_WARN_ON -= -w34100

    LIBS += -lUser32 -lGdi32

    # CEF requires us to include an app manifest. If the manifest isn't included correctly, CEF will crash (atleast in debug mode).
    # Both the main application and the helper application (webhelper) must have their own manifests.
    # In case the solution below stops working for some reason, here are two alternative methods:
    # 1. Alternative:
    #    Add this to qmake: CONFIG -= embed_manifest_exe
    #    Create an .rc file and add it to the qmake project via RC_FILE = /path/to/file.rc
    #    Add this to the rc file: 1 24 botfather.exe.manifest
    # 2. Alternative:
    #    Call this command after compiling:
    #    mt.exe -nologo -manifest botfather.exe.manifest -outputresource:botfather.exe;#1
    CONFIG += embed_manifest_exe
    QMAKE_MANIFEST = $$PWD/res/botfather.exe.manifest

    RC_FILE = res/botfather.rc
    HEADERS += res/resource.hpp
    SOURCES += engine/modules/desktop/desktop_win.cpp
}

HEADERS += \
    auth/auth_settings.hpp \
    auth/license_api_client.hpp \
    auth/rest_api_client.hpp \
    auth/scripts_api_client.hpp \
    engine/modules/android/apis/android_api.hpp \
    engine/modules/browser/browser_render_process_handler.hpp \
    engine/modules/desktop/apis/desktop_api.hpp \
    engine/modules/common/apis/helper_api.hpp \
    engine/modules/vision/apis/vision_api.hpp \
    engine/engine.hpp \
    engine/modules/algorithm/dijkstra.hpp \
    engine/modules/android/adb_device_info.hpp \
    engine/modules/android/adb_wrapper.hpp \
    engine/modules/android/android_settings.hpp \
    engine/modules/browser/browser_life_span_handler.hpp \
    engine/modules/browser/browser_load_handler.hpp \
    engine/modules/browser/browser_render_handler.hpp \
    engine/modules/browser/browser_request_handler.hpp \
    engine/modules/browser/browser_settings.hpp \
    engine/modules/desktop/desktop.hpp \
    engine/modules/desktop/keymap.hpp \
    engine/modules/vision/vision.hpp \
    engine/modules/common/prototypes/abstract_prototype.hpp \
    engine/modules/common/prototypes/color_prototype.hpp \
    engine/modules/vision/prototypes/image_prototype.hpp \
    engine/modules/vision/prototypes/match_prototype.hpp \
    engine/modules/common/prototypes/point_prototype.hpp \
    engine/modules/common/prototypes/rect_prototype.hpp \
    engine/modules/common/prototypes/size_prototype.hpp \
    engine/modules/vision/types/match.hpp \
    git/abstract_git_operation.hpp \
    git/git_behind_operation.hpp \
    git/git_clone_operation.hpp \
    git/git_fetch_operation.hpp \
    git/git_reclone_operation.hpp \
    gui/androiddialog/android_dialog.hpp \
    gui/botwidgets/abstract_bot_widget.hpp \
    gui/botwidgets/bot_browsers_widget.hpp \
    gui/botwidgets/bot_config_widget.hpp \
    gui/botwidgets/bot_log_widget.hpp \
    gui/botwidgets/bot_settings_widget.hpp \
    gui/botwidgets/bot_updates_widget.hpp \
    gui/botwidgets/bot_widget.hpp \
    gui/browserwindow/browser_address_bar.hpp \
    gui/browserwindow/browser_widget.hpp \
    gui/browserwindow/browser_window.hpp \
    engine/modules/common/apis/config_api.hpp \
    engine/modules/browser/adapters/cef_key_event_adapter.hpp \
    engine/modules/browser/browser_app.hpp \
    engine/modules/browser/browser_client.hpp \
    engine/modules/browser/browser_creator.hpp \
    engine/modules/browser/browser_list_model.hpp \
    engine/modules/browser/browser_manager.hpp \
    engine/modules/browser/browser_util.hpp \
    engine/modules/common/bf_key_mapper.hpp \
    engine/modules/common/bf_key_traits.hpp \
    engine/modules/common/bf_keymap.hpp \
    engine/modules/common/qt_key_traits.hpp \
    engine/modules/common/qt_keymaps.hpp \
    engine/modules/common/windows_keyboard_codes.hpp \
    engine/modules/browser/prototypes/browser_prototype.hpp \
    engine/modules/common/prototypes/timer_prototype.hpp \
    engine/modules/browser/types/browser.hpp \
    gui/main_window.hpp \
    gui/maintenancetool/maintenance_tool.hpp \
    gui/models/bot.hpp \
    gui/models/bot_config.hpp \
    gui/models/bot_config_group.hpp \
    gui/models/bot_config_option.hpp \
    gui/models/bot_list_model.hpp \
    gui/models/bot_manifest.hpp \
    gui/models/bot_manifest_node.hpp \
    gui/models/bot_repo.hpp \
    gui/authdialog/auth_dialog.hpp \
    engine/modules/common/apis/abstract_api.hpp \
    engine/modules/vision/types/blob_tpl.hpp \
    engine/modules/vision/prototypes/blob_tpl_prototype.hpp \
    engine/modules/algorithm/apis/algorithm_api.hpp \
    engine/modules/common/prototypes/margins_prototype.hpp \
    engine/modules/common/prototypes/url_prototype.hpp \
    gui/models/bot_repo_list_model.hpp \
    gui/preferences/preferences_dialog.hpp \
    gui/scriptmanager/git_progress_dialog.hpp \
    gui/scriptmanager/script_manager_dialog.hpp \
    res/resource.hpp \
    shared/q_settings_json_format.hpp
SOURCES += \
    auth/license_api_client.cpp \
    auth/rest_api_client.cpp \
    auth/scripts_api_client.cpp \
    engine/modules/android/apis/android_api.cpp \
    engine/modules/browser/browser_render_process_handler.cpp \
    engine/modules/desktop/apis/desktop_api.cpp \
    engine/modules/common/apis/helper_api.cpp \
    engine/modules/vision/apis/vision_api.cpp \
    engine/engine.cpp \
    engine/modules/algorithm/dijkstra.cpp \
    engine/modules/android/adb_device_info.cpp \
    engine/modules/android/adb_wrapper.cpp \
    engine/modules/browser/browser_life_span_handler.cpp \
    engine/modules/browser/browser_load_handler.cpp \
    engine/modules/browser/browser_render_handler.cpp \
    engine/modules/browser/browser_request_handler.cpp \
    engine/modules/vision/vision.cpp \
    engine/modules/common/prototypes/abstract_prototype.cpp \
    engine/modules/common/prototypes/color_prototype.cpp \
    engine/modules/vision/prototypes/image_prototype.cpp \
    engine/modules/vision/prototypes/match_prototype.cpp \
    engine/modules/common/prototypes/point_prototype.cpp \
    engine/modules/common/prototypes/rect_prototype.cpp \
    engine/modules/common/prototypes/size_prototype.cpp \
    engine/modules/vision/types/match.cpp \
    git/abstract_git_operation.cpp \
    git/git_behind_operation.cpp \
    git/git_clone_operation.cpp \
    git/git_fetch_operation.cpp \
    git/git_reclone_operation.cpp \
    gui/androiddialog/android_dialog.cpp \
    gui/botwidgets/abstract_bot_widget.cpp \
    gui/botwidgets/bot_browsers_widget.cpp \
    gui/botwidgets/bot_config_widget.cpp \
    gui/botwidgets/bot_log_widget.cpp \
    gui/botwidgets/bot_settings_widget.cpp \
    gui/botwidgets/bot_updates_widget.cpp \
    gui/botwidgets/bot_widget.cpp \
    gui/browserwindow/browser_address_bar.cpp \
    gui/browserwindow/browser_widget.cpp \
    gui/browserwindow/browser_window.cpp \
    engine/modules/browser/adapters/cef_key_event_adapter.cpp \
    engine/modules/common/bf_key_mapper.cpp \
    gui/main_window.cpp \
    gui/maintenancetool/maintenance_tool.cpp \
    gui/models/bot_config.cpp \
    gui/models/bot_config_group.cpp \
    gui/models/bot_config_option.cpp \
    gui/models/bot_list_model.cpp \
    gui/models/bot_manifest.cpp \
    gui/models/bot_manifest_node.cpp \
    gui/models/bot_repo.cpp \
    gui/models/bot_repo_list_model.cpp \
    gui/preferences/preferences_dialog.cpp \
    gui/authdialog/auth_dialog.cpp \
    gui/scriptmanager/git_progress_dialog.cpp \
    gui/scriptmanager/script_manager_dialog.cpp \
    main.cpp \
    engine/modules/common/apis/abstract_api.cpp \
    engine/modules/vision/types/blob_tpl.cpp \
    engine/modules/vision/prototypes/blob_tpl_prototype.cpp \
    engine/modules/algorithm/apis/algorithm_api.cpp \
    engine/modules/common/prototypes/margins_prototype.cpp \
    engine/modules/common/prototypes/url_prototype.cpp \
    engine/modules/browser/browser_list_model.cpp \
    engine/modules/browser/browser_manager.cpp \
    engine/modules/common/prototypes/timer_prototype.cpp \
    gui/models/bot.cpp \
    engine/modules/common/apis/config_api.cpp \
    engine/modules/browser/prototypes/browser_prototype.cpp \
    engine/modules/browser/types/browser.cpp \
    engine/modules/browser/browser_app.cpp \
    engine/modules/browser/browser_client.cpp \
    engine/modules/browser/browser_creator.cpp \
    engine/modules/browser/browser_util.cpp \
    shared/q_settings_json_format.cpp
FORMS += \
    gui/androiddialog/android_dialog.ui \
    gui/authdialog/auth_dialog.ui \
    gui/botwidgets/bot_browsers_widget.ui \
    gui/botwidgets/bot_config_widget.ui \
    gui/botwidgets/bot_log_widget.ui \
    gui/botwidgets/bot_settings_widget.ui \
    gui/botwidgets/bot_updates_widget.ui \
    gui/browserwindow/browser_window.ui \
    gui/main_window.ui \
    gui/preferences/preferences_dialog.ui \
    gui/scriptmanager/git_progress_dialog.ui \
    gui/scriptmanager/script_manager_dialog.ui
RESOURCES += \
    res/botfather.qrc
