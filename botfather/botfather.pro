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

    # CEF requires us to include an app manifest. If the manifest isn't included correctly, CEF will crash (atleast in debug mode).
    # Both the main application and the helper application (botfather_helper) must have their own manifests.
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

    RC_FILE = ./res/botfather.rc
    HEADERS += ./res/resource.h
    SOURCES += ./engine/modules/desktop/desktop_win.cpp
}

HEADERS += \
    ./auth/auth_settings.h \
    ./auth/license_api_client.h \
    ./auth/rest_api_client.h \
    ./engine/apis/android_api.h \
    ./engine/apis/desktop_api.h \
    ./engine/apis/helper_api.h \
    ./engine/apis/vision_api.h \
    ./engine/engine.h \
    ./engine/modules/algorithm/dijkstra.h \
    ./engine/modules/android/adb_device_info.h \
    ./engine/modules/android/adb_wrapper.h \
    ./engine/modules/android/android_settings.h \
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
    ./gui/androiddialog/android_dialog.h \
    ./gui/browserwindow/browser_address_bar.h \
    ./gui/browserwindow/browser_widget.h \
    ./gui/browserwindow/browser_window.h \
    gui/preferences/preferences_dialog.h \
    ./gui/authdialog/auth_dialog.h \
    ./settings.h \
    ./engine/apis/abstract_api.h \
    ./engine/types/blob_tpl.h \
    ./engine/prototypes/blob_tpl_prototype.h \
    ./engine/apis/algorithm_api.h \
    ./engine/prototypes/margins_prototype.h \
    ./engine/prototypes/url_prototype.h \
    engine/modules/browser/browser_manager.h \
    engine/prototypes/timer_prototype.h \
    gui/scriptmanager/scriptmanagerdialog.h \
    git/gitfetchoperation.h \
    git/abstractgitoperation.h \
    git/gitcloneoperation.h \
    git/gitbehindoperation.h \
    git/gitrecloneoperation.h \
    gui/scriptmanager/gitprogressdialog.h \
    auth/scriptsapiclient.h \
    gui/mainwindow.h \
    gui/models/botlistmodel.h \
    gui/models/bot.h \
    gui/models/botrepo.h \
    gui/models/botrepolistmodel.h \
    gui/botwidgets/botwidget.h \
    gui/botwidgets/botlogwidget.h \
    gui/botwidgets/botsettingswidget.h \
    gui/botwidgets/abstractbotwidget.h \
    gui/botwidgets/botupdateswidget.h \
    gui/models/botmanifest.h \
    gui/models/botconfig.h \
    gui/models/botconfiggroup.h \
    gui/botwidgets/botconfigwidget.h \
    gui/models/botmanifestnode.h \
    engine/apis/config_api.h \
    gui/models/botconfigoption.h \
    gui/maintenancetool/maintenancetool.h \
    shared/qsettingsjsonformat.h \
    engine/prototypes/browser_prototype.h \
    engine/types/browser.h \
    engine/modules/browser/browser_app.h \
    engine/modules/browser/browser_client.h \
    engine/modules/browser/browser_creator.h \
    engine/modules/browser/browser_util.h \
    engine/modules/browser/browser_model.h \
    gui/botwidgets/botbrowserswidget.h
SOURCES += \
    ./auth/license_api_client.cpp \
    ./auth/rest_api_client.cpp \
    ./engine/apis/android_api.cpp \
    ./engine/apis/desktop_api.cpp \
    ./engine/apis/helper_api.cpp \
    ./engine/apis/vision_api.cpp \
    ./engine/engine.cpp \
    ./engine/modules/algorithm/dijkstra.cpp \
    ./engine/modules/android/adb_device_info.cpp \
    ./engine/modules/android/adb_wrapper.cpp \
    ./engine/modules/vision/vision.cpp \
    ./engine/prototypes/abstract_prototype.cpp \
    ./engine/prototypes/color_prototype.cpp \
    ./engine/prototypes/image_prototype.cpp \
    ./engine/prototypes/match_prototype.cpp \
    ./engine/prototypes/point_prototype.cpp \
    ./engine/prototypes/rect_prototype.cpp \
    ./engine/prototypes/size_prototype.cpp \
    ./engine/types/match.cpp \
    ./gui/androiddialog/android_dialog.cpp \
    ./gui/browserwindow/browser_address_bar.cpp \
    ./gui/browserwindow/browser_widget.cpp \
    ./gui/browserwindow/browser_window.cpp \
    gui/preferences/preferences_dialog.cpp \
    ./gui/authdialog/auth_dialog.cpp \
    ./main.cpp \
    ./engine/apis/abstract_api.cpp \
    ./engine/types/blob_tpl.cpp \
    ./engine/prototypes/blob_tpl_prototype.cpp \
    ./engine/apis/algorithm_api.cpp \
    ./engine/prototypes/margins_prototype.cpp \
    ./engine/prototypes/url_prototype.cpp \
    engine/modules/browser/browser_manager.cpp \
    engine/prototypes/timer_prototype.cpp \
    gui/scriptmanager/scriptmanagerdialog.cpp \
    git/gitfetchoperation.cpp \
    git/abstractgitoperation.cpp \
    git/gitcloneoperation.cpp \
    git/gitbehindoperation.cpp \
    git/gitrecloneoperation.cpp \
    gui/scriptmanager/gitprogressdialog.cpp \
    auth/scriptsapiclient.cpp \
    gui/mainwindow.cpp \
    gui/models/botlistmodel.cpp \
    gui/models/bot.cpp \
    gui/models/botrepo.cpp \
    gui/models/botrepolistmodel.cpp \
    gui/botwidgets/botwidget.cpp \
    gui/botwidgets/botlogwidget.cpp \
    gui/botwidgets/botsettingswidget.cpp \
    gui/botwidgets/abstractbotwidget.cpp \
    gui/botwidgets/botupdateswidget.cpp \
    gui/models/botmanifest.cpp \
    gui/models/botconfig.cpp \
    gui/models/botconfiggroup.cpp \
    gui/botwidgets/botconfigwidget.cpp \
    gui/models/botmanifestnode.cpp \
    engine/apis/config_api.cpp \
    gui/models/botconfigoption.cpp \
    gui/maintenancetool/maintenancetool.cpp \
    shared/qsettingsjsonformat.cpp \
    engine/prototypes/browser_prototype.cpp \
    engine/types/browser.cpp \
    engine/modules/browser/browser_app.cpp \
    engine/modules/browser/browser_client.cpp \
    engine/modules/browser/browser_creator.cpp \
    engine/modules/browser/browser_util.cpp \
    engine/modules/browser/browser_model.cpp \
    gui/botwidgets/botbrowserswidget.cpp
FORMS += \
    ./gui/androiddialog/android_dialog.ui \
    ./gui/authdialog/auth_dialog.ui \
    ./gui/browserwindow/browser_window.ui \
    gui/preferences/preferences_dialog.ui \
    gui/scriptmanager/scriptmanagerdialog.ui \
    gui/scriptmanager/gitprogressdialog.ui \
    gui/mainwindow.ui \
    gui/botwidgets/botlogwidget.ui \
    gui/botwidgets/botsettingswidget.ui \
    gui/botwidgets/botupdateswidget.ui \
    gui/botwidgets/botconfigwidget.ui \
    gui/botwidgets/botbrowserswidget.ui
RESOURCES += \
    ./res/gui.qrc \
    res/botfather.qrc
