#-------------------------------------------------
#
# Project created by QtCreator 2017-11-08T01:13:15
#
#-------------------------------------------------

QT += core gui qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = BotFather

unix:INCLUDEPATH += "/opt/cef/"
unix:LIBS += -L/opt/cef/libcef_dll_wrapper/ -lcef_dll_wrapper
unix:LIBS += -L./ -lcef
unix:LIBS += -lX11
unix:LIBS += -L/usr/lib -lopencv_stitching -lopencv_superres -lopencv_videostab \
	     -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib \
	     -lopencv_dpm -lopencv_freetype -lopencv_fuzzy -lopencv_line_descriptor \
	     -lopencv_optflow -lopencv_reg -lopencv_saliency -lopencv_stereo \
	     -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd \
	     -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text \
	     -lopencv_face -lopencv_plot -lopencv_dnn -lopencv_xfeatures2d -lopencv_shape \
	     -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d \
	     -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml \
	     -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs \
	     -lopencv_photo -lopencv_imgproc -lopencv_core
unix:QMAKE_RPATHDIR += .
unix:QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
unix:QMAKE_CFLAGS = -Wno-unused-parameter
unix:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

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
    scripting/bot.cpp \
    vision/blob_tpl.cpp \
    vision/blob_tpl_factory.cpp \
    vision/hsv_color.cpp \
    vision/hsv_color_factory.cpp \
    vision/image.cpp \
    vision/match.cpp \
    vision/vision.cpp \
    vision/vision_api.cpp \
    scripting/helper_api.cpp \
    browser/browser_api.cpp \
    scripting/bot_thread.cpp \
    gui/browser_window.cpp \
    gui/browser_widget.cpp \
    gui/browser_address_bar.cpp \
    updater/update_checker.cpp \
    gui/update_widget.cpp \
    android/android_api.cpp \
    android/adb_device_info.cpp \
    android/adb_wrapper.cpp \
    gui/android_dialog.cpp \
    gui/auth_dialog.cpp \
    auth/authenticator.cpp \
    updater/updater_dialog.cpp

HEADERS += \
    browser/browser.h \
    browser/browser_app.h \
    browser/browser_client.h \
    gui/control_window.h \
    gui/config_dialog.h \
    scripting/bot.h \
    vision/blob_tpl.h \
    vision/blob_tpl_factory.h \
    vision/hsv_color.h \
    vision/hsv_color_factory.h \
    vision/image.h \
    vision/match.h \
    vision/vision.h \
    vision/vision_api.h \
    scripting/helper_api.h \
    browser/browser_api.h \
    scripting/bot_thread.h \
    gui/browser_window.h \
    gui/browser_widget.h \
    gui/browser_address_bar.h \
    updater/update_checker.h \
    gui/update_widget.h \
    android/android_api.h \
    android/adb_device_info.h \
    android/adb_wrapper.h \
    gui/android_dialog.h \
    gui/auth_dialog.h \
    auth/authenticator.h \
    shared/settings.h \
    updater/updater_dialog.h

FORMS += \
    gui/control_window.ui \
    gui/config_dialog.ui \
    gui/browser_window.ui \
    gui/update_widget.ui \
    gui/android_dialog.ui \
    gui/auth_dialog.ui \
    updater/updater_dialog.ui

RESOURCES += \
    res/gui.qrc
