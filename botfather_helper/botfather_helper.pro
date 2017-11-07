TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

unix:INCLUDEPATH += "/opt/cef/"
unix:LIBS += -L/opt/cef/libcef_dll_wrapper/ -lcef_dll_wrapper
unix:LIBS += -L./ -lcef
unix:QMAKE_RPATHDIR += .

QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
QMAKE_CFLAGS = -Wno-unused-parameter
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

SOURCES += main.cpp