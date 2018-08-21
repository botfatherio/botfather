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

CEF_VERSION = 3.3325.1758

unix {
    LINUX_CEF_DIR = /opt/cef/$${CEF_VERSION}

    # Disables unused warnings
    QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
    QMAKE_CFLAGS = -Wno-unused-parameter
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
    
    # Newer distros set GCC default link flag -pie, which marks e_type as ET_DYN on the binary file.
    # Consequently, the Operating System recognizes them as Shared Library.
    # Using -no-pie our project will be a executable, not a shared libary.
    QMAKE_LFLAGS += -no-pie

    # This tells the ELF to look for required libraries right next to the binary first.
    # That way libcef.so and other shared objects can be loaded which otherwise couldn't be found.
    QMAKE_RPATHDIR += ./

    INCLUDEPATH += $${LINUX_CEF_DIR}
    LIBS += -L$${LINUX_CEF_DIR}/libcef_dll_wrapper/ -lcef_dll_wrapper

    CONFIG(release, debug|release):LIBS += -L$${LINUX_CEF_DIR}/Release -lcef
    else: CONFIG(debug, debug|release):LIBS += -L$${LINUX_CEF_DIR}/Debug -lcef

    LIBS += -lX11
    LIBS += -L/usr/lib -lopencv_features2d -lopencv_flann -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui

    # Dramatically reduces shared libary file size after linking
    CONFIG(release, debug|release):QMAKE_POST_LINK += ../tools/strip_all_so_files.sh $${DESTDIR}

    # Example of copying files after linking
    #CONFIG(release, debug|release):QMAKE_POST_LINK += ../botfather/tools/collect_deps.sh $${DESTDIR} Release $${LINUX_CEF_DIR}
    #else:CONFIG(debug, debug|release):QMAKE_POST_LINK += ../botfather/tools/collect_deps.sh $${DESTDIR} Debug $${LINUX_CEF_DIR}

    SOURCES += src/engine/modules/desktop/desktop_x11.cpp
}

win32 {
    QT += winextras
    DEFINES += _UNICODE WIN64 QT_DLL QT_WIDGETS_LIB QT_QML_LIB QT_NETWORK_LIB

    # Disables "unreferenced formal parameter" warning on windows
    QMAKE_CXXFLAGS_WARN_ON -= -w34100

    # The seconds \ is required cause qts syntac sucks
    WIN_CEF_DIR =  C:\CEF\\$${CEF_VERSION}

    INCLUDEPATH += C:\OPENCV\3.4.0\build\include
    INCLUDEPATH += $$WIN_CEF_DIR

    CONFIG(release, debug|release):LIBS += -L$$WIN_CEF_DIR\libcef_dll_wrapper\Release -llibcef_dll_wrapper
    else:CONFIG(debug, debug|release):LIBS += -L$$WIN_CEF_DIR\libcef_dll_wrapper\Debug -llibcef_dll_wrapper

    CONFIG(release, debug|release):LIBS += -L$$WIN_CEF_DIR\Release -llibcef
    else: CONFIG(debug, debug|release):LIBS += -L$$WIN_CEF_DIR\Debug -llibcef

    CONFIG(release, debug|release):LIBS += -LC:\OPENCV\3.4.0\build\lib\Release -lopencv_world340
    else:CONFIG(debug, debug|release):LIBS += -LC:\OPENCV\3.4.0\build\lib\Debug -lopencv_world340d

    LIBS += -lUser32 -lGdi32

    # Make the app icon be compiled into the bianary on windows
    HEADERS += ./src/res/resource.h
    RC_FILE = ./src/res/botfather.rc

    # Use the compatibility manifest required by CEF
    WINRT_MANIFEST = ./src/res/compatibility.manifest

    # Copy required dlls and CEF ressources
    CONFIG(release, debug|release):CEF_EXTRA_DIR = $${CEF_DIR}/Release/*
    else:CONFIG(debug, debug|release):CEF_EXTRA_DIR = $${CEF_DIR}/Debug/*

    SOURCES += src/engine/modules/desktop/desktop_win.cpp
}

include(src/3rdparty/qhotkey.pri)

HEADERS += \
    src/auth/auth_settings.h \
    src/auth/license_api_client.h \
    src/auth/rest_api_client.h \
    src/auth/status_api_client.h \
    src/engine/apis/android_api.h \
    src/engine/apis/browser_api.h \
    src/engine/apis/desktop_api.h \
    src/engine/apis/helper_api.h \
    src/engine/apis/vision_api.h \
    src/engine/bot.h \
    src/engine/general_settings.h \
    src/engine/modules/android/adb_device_info.h \
    src/engine/modules/android/adb_wrapper.h \
    src/engine/modules/android/android_settings.h \
    src/engine/modules/browser/browser.h \
    src/engine/modules/browser/browser_app.h \
    src/engine/modules/browser/browser_client.h \
    src/engine/modules/browser/browser_settings.h \
    src/engine/modules/desktop/desktop.h \
    src/engine/modules/desktop/keymap.h \
    src/engine/modules/vision/vision.h \
    src/engine/prototypes/abstract_prototype.h \
    src/engine/prototypes/color_prototype.h \
    src/engine/prototypes/image_prototype.h \
    src/engine/prototypes/match_prototype.h \
    src/engine/prototypes/point_prototype.h \
    src/engine/prototypes/rect_prototype.h \
    src/engine/prototypes/size_prototype.h \
    src/engine/types/match.h \
    src/gui/android_dialog.h \
    src/gui/browser_address_bar.h \
    src/gui/browser_widget.h \
    src/gui/browser_window.h \
    src/gui/config_dialog.h \
    src/gui/control_window.h \
    src/gui/status_dialog.h \
    src/updater/update_checker.h \
    src/updater/updater_dialog.h
SOURCES += \
    src/auth/license_api_client.cpp \
    src/auth/rest_api_client.cpp \
    src/auth/status_api_client.cpp \
    src/engine/apis/android_api.cpp \
    src/engine/apis/browser_api.cpp \
    src/engine/apis/desktop_api.cpp \
    src/engine/apis/helper_api.cpp \
    src/engine/apis/vision_api.cpp \
    src/engine/bot.cpp \
    src/engine/modules/android/adb_device_info.cpp \
    src/engine/modules/android/adb_wrapper.cpp \
    src/engine/modules/browser/browser.cpp \
    src/engine/modules/browser/browser_app.cpp \
    src/engine/modules/browser/browser_client.cpp \
    src/engine/modules/vision/vision.cpp \
    src/engine/prototypes/abstract_prototype.cpp \
    src/engine/prototypes/color_prototype.cpp \
    src/engine/prototypes/image_prototype.cpp \
    src/engine/prototypes/match_prototype.cpp \
    src/engine/prototypes/point_prototype.cpp \
    src/engine/prototypes/rect_prototype.cpp \
    src/engine/prototypes/size_prototype.cpp \
    src/engine/types/match.cpp \
    src/gui/android_dialog.cpp \
    src/gui/browser_address_bar.cpp \
    src/gui/browser_widget.cpp \
    src/gui/browser_window.cpp \
    src/gui/config_dialog.cpp \
    src/gui/control_window.cpp \
    src/gui/status_dialog.cpp \
    src/main.cpp \
    src/updater/update_checker.cpp \
    src/updater/updater_dialog.cpp
FORMS += \
    src/gui/android_dialog.ui \
    src/gui/auth_dialog.ui \
    src/gui/browser_window.ui \
    src/gui/config_dialog.ui \
    src/gui/control_window.ui \
    src/gui/status_dialog.ui \
    src/updater/updater_dialog.ui
RESOURCES += \
    src/res/gui.qrc
