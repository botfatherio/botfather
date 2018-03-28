TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = botfather_helper
DESTDIR = ./BuildOutput
OBJECTS_DIR += ./CompiledObjects
SOURCES += ./src/main.cpp

win32 {
    DEFINES += _UNICODE WIN64 QT_DLL QT_WIDGETS_LIB QT_QML_LIB QT_NETWORK_LIB

    # Disables "unreferenced formal parameter" warning on windows
    QMAKE_CXXFLAGS_WARN_ON -= -w34100

    CEF_DIR = C:\CEF\3.3202.1682
    INCLUDEPATH += $${CEF_DIR}

    CONFIG(release, debug|release):LIBS += -L$${CEF_DIR}\libcef_dll_wrapper\Release -llibcef_dll_wrapper
    else:CONFIG(debug, debug|release):LIBS += -L$${CEF_DIR}\libcef_dll_wrapper\Debug -llibcef_dll_wrapper

    CONFIG(release, debug|release):LIBS += -L$${CEF_DIR}\Release -llibcef
    else: CONFIG(debug, debug|release):LIBS += -L$${CEF_DIR}\Debug -llibcef

    # Use the compatibility manifest required by CEF
    WINRT_MANIFEST = ./src/compatibility.manifest
}

unix {
	INCLUDEPATH += "/opt/cef/"
	LIBS += -L/opt/cef/libcef_dll_wrapper/ -lcef_dll_wrapper
	LIBS += -L./ -lcef
	
	# Disables unused parameter warnings
	QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
	QMAKE_CFLAGS = -Wno-unused-parameter
	QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

	# Define where to look for extra libraries
	QMAKE_RPATHDIR += ./libs

	# Newer distros set GCC default link flag -pie, which marks e_type as ET_DYN on the binary file.
	# Consequently, the Operating System recognizes them as Shared Library.
	# Using -no-pie our project will be a executable, not a shared libary.
	QMAKE_LFLAGS += -no-pie
}