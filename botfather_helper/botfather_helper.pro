TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

unix:INCLUDEPATH += "/opt/cef/"
unix:LIBS += -L/opt/cef/libcef_dll_wrapper/ -lcef_dll_wrapper
unix:LIBS += -L./ -lcef
unix:QMAKE_RPATHDIR += .
unix:QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
unix:QMAKE_CFLAGS = -Wno-unused-parameter
unix:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

SOURCES += main.cpp