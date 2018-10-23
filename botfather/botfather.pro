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

unix {
    # Disables unused warnings
    QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
    QMAKE_CFLAGS = -Wno-unused-parameter
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

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

    SOURCES += ./engine/modules/desktop/desktop_x11.cpp
    HEADERS += \
        ./engine/modules/desktop/linux/input-event-codes.h \
        ./engine/modules/desktop/linux/uinput.h
}

win32 {
    QT += winextras
    DEFINES += _UNICODE WIN64 QT_DLL QT_WIDGETS_LIB QT_QML_LIB QT_NETWORK_LIB

    # Disable "unreferenced formal parameter" warning on windows
    QMAKE_CXXFLAGS_WARN_ON -= -w34100

    LIBS += -lUser32 -lGdi32

    WINRT_MANIFEST = ./res/compatibility.manifest # Use the compatibility manifest required by CEF
    RC_FILE = ./res/botfather.rc # Make the app icon be compiled into the bianary on windows
    HEADERS += ./res/resource.h
    SOURCES += ./engine/modules/desktop/desktop_win.cpp
}

HEADERS += \
    ./auth/auth_settings.h \
    ./auth/license_api_client.h \
    ./auth/rest_api_client.h \
    ./engine/apis/android_api.h \
    ./engine/apis/browser_api.h \
    ./engine/apis/desktop_api.h \
    ./engine/apis/helper_api.h \
    ./engine/apis/vision_api.h \
    ./engine/bot.h \
    ./engine/modules/algorithms/dijkstra.h \
    ./engine/modules/android/adb_device_info.h \
    ./engine/modules/android/adb_wrapper.h \
    ./engine/modules/android/android_settings.h \
    ./engine/modules/browser/browser.h \
    ./engine/modules/browser/browser_app.h \
    ./engine/modules/browser/browser_client.h \
    ./engine/modules/browser/browser_settings.h \
    ./engine/modules/desktop/desktop.h \
    ./engine/modules/desktop/keymap.h \
    ./engine/modules/vision/vision.h \
    ./engine/prototypes/abstract_prototype.h \
    ./engine/prototypes/color_prototype.h \
    ./engine/prototypes/image_prototype.h \
    ./engine/prototypes/match_prototype.h \
    ./engine/prototypes/point_prototype.h \
    ./engine/prototypes/rect_prototype.h \
    ./engine/prototypes/size_prototype.h \
    ./engine/types/match.h \
    ./gui/android_dialog.h \
    ./gui/browser_address_bar.h \
    ./gui/browser_widget.h \
    ./gui/browser_window.h \
    ./gui/config_dialog.h \
    ./gui/control_window.h \
    ./settings.h \
    ./engine/apis/abstract_api.h \
    ./engine/types/blob_tpl.h \
    ./engine/prototypes/blob_tpl_prototype.h \
    ./engine/apis/path_finder_api.h \
    ./engine/prototypes/margins_prototype.h \
    ./engine/prototypes/url_prototype.h \
    engine/prototypes/timer_prototype.h \
    tools/mtoolwrapper.h
SOURCES += \
    ./auth/license_api_client.cpp \
    ./auth/rest_api_client.cpp \
    ./engine/apis/android_api.cpp \
    ./engine/apis/browser_api.cpp \
    ./engine/apis/desktop_api.cpp \
    ./engine/apis/helper_api.cpp \
    ./engine/apis/vision_api.cpp \
    ./engine/bot.cpp \
    ./engine/modules/algorithms/dijkstra.cpp \
    ./engine/modules/android/adb_device_info.cpp \
    ./engine/modules/android/adb_wrapper.cpp \
    ./engine/modules/browser/browser.cpp \
    ./engine/modules/browser/browser_app.cpp \
    ./engine/modules/browser/browser_client.cpp \
    ./engine/modules/vision/vision.cpp \
    ./engine/prototypes/abstract_prototype.cpp \
    ./engine/prototypes/color_prototype.cpp \
    ./engine/prototypes/image_prototype.cpp \
    ./engine/prototypes/match_prototype.cpp \
    ./engine/prototypes/point_prototype.cpp \
    ./engine/prototypes/rect_prototype.cpp \
    ./engine/prototypes/size_prototype.cpp \
    ./engine/types/match.cpp \
    ./gui/android_dialog.cpp \
    ./gui/browser_address_bar.cpp \
    ./gui/browser_widget.cpp \
    ./gui/browser_window.cpp \
    ./gui/config_dialog.cpp \
    ./gui/control_window.cpp \
    ./main.cpp \
    ./engine/apis/abstract_api.cpp \
    ./engine/types/blob_tpl.cpp \
    ./engine/prototypes/blob_tpl_prototype.cpp \
    ./engine/apis/path_finder_api.cpp \
    ./engine/prototypes/margins_prototype.cpp \
    ./engine/prototypes/url_prototype.cpp \
    engine/prototypes/timer_prototype.cpp \
    tools/mtoolwrapper.cpp
FORMS += \
    ./gui/android_dialog.ui \
    ./gui/auth_dialog.ui \
    ./gui/browser_window.ui \
    ./gui/config_dialog.ui \
    ./gui/control_window.ui
RESOURCES += \
    ./res/gui.qrc
