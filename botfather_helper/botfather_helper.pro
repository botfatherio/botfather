TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = botfather_helper
DESTDIR = ./BuildOutput
OBJECTS_DIR += ./CompiledObjects
SOURCES += ./main.cpp

unix {
    INCLUDEPATH += $$(CEF_ROOT)
    LIBS += -L$$(CEF_ROOT)/libcef_dll_wrapper/ -lcef_dll_wrapper

    CONFIG(release, debug|release):LIBS += -L$$(CEF_ROOT)/Release -lcef
    else: CONFIG(debug, debug|release):LIBS += -L$$(CEF_ROOT)/Debug -lcef

    # Disables unused parameter warnings
    QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
    QMAKE_CFLAGS = -Wno-unused-parameter
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

    # Define where to look for extra libraries
    QMAKE_RPATHDIR += ./

    # Newer distros set GCC default link flag -pie, which marks e_type as ET_DYN on the binary file.
    # Consequently, the Operating System recognizes them as Shared Library.
    # Using -no-pie our project will be a executable, not a shared libary.
    #QMAKE_LFLAGS += -no-pie
}

win32 {
    DEFINES += _UNICODE WIN64 QT_DLL QT_WIDGETS_LIB QT_QML_LIB QT_NETWORK_LIB

    # Disables "unreferenced formal parameter" warning on windows
    QMAKE_CXXFLAGS_WARN_ON -= -w34100

    INCLUDEPATH += $$(CEF_ROOT)

    CONFIG(release, debug|release):LIBS += -L$$(CEF_ROOT)\libcef_dll_wrapper\Release -llibcef_dll_wrapper
    else:CONFIG(debug, debug|release):LIBS += -L$$(CEF_ROOT)\libcef_dll_wrapper\Debug -llibcef_dll_wrapper

    CONFIG(release, debug|release):LIBS += -L$$(CEF_ROOT)\Release -llibcef
    else: CONFIG(debug, debug|release):LIBS += -L$$(CEF_ROOT)\Debug -llibcef

    # Add app manifest required by CEF. See botfather.pro for details
    CONFIG += embed_manifest_exe
    QMAKE_MANIFEST = $$PWD/botfather_helper.exe.manifest
}

DISTFILES +=
