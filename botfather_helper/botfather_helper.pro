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

unix:QMAKE_RPATHDIR += ./libs

# Newer distros set GCC default link flag -pie, which marks e_type as ET_DYN on the binary file.
# Consequently, the Operating System recognizes them as Shared Library.
# Using -no-pie our project will be a executable, not a shared libary.
unix:QMAKE_LFLAGS += -no-pie

SOURCES += main.cpp