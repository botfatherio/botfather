TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
QT += core

TARGET = webhelper
DESTDIR = ./BuildOutput
OBJECTS_DIR += ./CompiledObjects

include(../3rdparty/CEF/CEF.pri)

unix {
    # Disables unused parameter warnings
    QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
    QMAKE_CFLAGS = -Wno-unused-parameter
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

    # Define where to look for extra libraries
    QMAKE_RPATHDIR += ./
}

win32 {
    DEFINES += _UNICODE WIN64 QT_DLL QT_WIDGETS_LIB QT_QML_LIB QT_NETWORK_LIB

    # Disables "unreferenced formal parameter" warning on windows
    QMAKE_CXXFLAGS_WARN_ON -= -w34100

    # Add app manifest required by CEF. See botfather.pro for details
    CONFIG += embed_manifest_exe
    QMAKE_MANIFEST = $$PWD/webhelper.exe.manifest
}

HEADERS += \
    bf_debug_tools.hpp \
    bf_serializer.hpp \
    helper_app.hpp

SOURCES += ./main.cpp \
    bf_debug_tools.cpp \
    bf_serializer.cpp \
    helper_app.cpp
