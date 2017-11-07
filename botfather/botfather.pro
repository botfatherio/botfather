#-------------------------------------------------
#
# Project created by QtCreator 2017-11-08T01:13:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = BotFather

unix:INCLUDEPATH += "/opt/cef/"
unix:LIBS += -L/opt/cef/libcef_dll_wrapper/ -lcef_dll_wrapper
unix:LIBS += -L./ -lcef
unix:LIBS += -lX11
unix:QMAKE_RPATHDIR += .

QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
QMAKE_CFLAGS = -Wno-unused-parameter
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

# Newer distros set GCC default link flag -pie, which marks e_type as ET_DYN on the binary file.
# Consequently, the Operating System recognizes them as Shared Library.
# Using -no-pie our project will be a executable, not a shared libary.
unix:QMAKE_LFLAGS += -no-pie

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    browser/browser.cpp \
    browser/browser_app.cpp \
    browser/browser_client.cpp \
    gui/control_window.cpp \
    gui/config_dialog.cpp \
    gui/browser_widget.cpp \
    scripting/bot.cpp

HEADERS += \
    shared/constants.h \
    browser/browser.h \
    browser/browser_app.h \
    browser/browser_client.h \
    gui/control_window.h \
    gui/config_dialog.h \
    gui/browser_widget.h \
    scripting/bot.h

FORMS += \
    gui/control_window.ui \
    gui/config_dialog.ui

RESOURCES += \
    res/gui.qrc
