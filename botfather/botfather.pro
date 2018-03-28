TEMPLATE = app
TARGET = botfather
QT += core network widgets qml gui

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

unix {
    LINUX_CEF_DIR = /opt/cef

    # Disables unused warnings
    QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
    QMAKE_CFLAGS = -Wno-unused-parameter
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
    
    # Newer distros set GCC default link flag -pie, which marks e_type as ET_DYN on the binary file.
    # Consequently, the Operating System recognizes them as Shared Library.
    # Using -no-pie our project will be a executable, not a shared libary.
    QMAKE_LFLAGS += -no-pie

    # This tells the ELF to look for required libraries right next to the binary first.
    # That way libcef.so and other shared objects can be loaded which otherwise couldn't be found.
    QMAKE_RPATHDIR += .

    INCLUDEPATH += $${LINUX_CEF_DIR}
    LIBS += -L$${LINUX_CEF_DIR}/libcef_dll_wrapper/ -lcef_dll_wrapper

    CONFIG(release, debug|release):LIBS += -L$${LINUX_CEF_DIR}/Release -lcef
    else: CONFIG(debug, debug|release):LIBS += -L$${LINUX_CEF_DIR}/Debug -lcef

    LIBS += -lX11

    # TODO: remove some of these
    LIBS += -L/usr/lib -lopencv_stitching -lopencv_superres -lopencv_videostab \
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

    # Example of copying files after linking
    CONFIG(release, debug|release):QMAKE_POST_LINK += ../botfather/tools/collect_deps.sh $${DESTDIR} Release $${LINUX_CEF_DIR}
    else:CONFIG(debug, debug|release):QMAKE_POST_LINK += ../botfather/tools/collect_deps.sh $${DESTDIR} Debug $${LINUX_CEF_DIR}
}

win32 {
    DEFINES += _UNICODE WIN64 QT_DLL QT_WIDGETS_LIB QT_QML_LIB QT_NETWORK_LIB

    # Disables "unreferenced formal parameter" warning on windows
    QMAKE_CXXFLAGS_WARN_ON -= -w34100

    INCLUDEPATH += C:\OPENCV\3.4.0\build\include
    INCLUDEPATH += C:\CEF\3.3202.1682

    CONFIG(release, debug|release):LIBS += -LC:\CEF\3.3202.1682\libcef_dll_wrapper\Release -llibcef_dll_wrapper
    else:CONFIG(debug, debug|release):LIBS += -LC:\CEF\3.3202.1682\libcef_dll_wrapper\Debug -llibcef_dll_wrapper

    CONFIG(release, debug|release):LIBS += -LC:\CEF\3.3202.1682\Release -llibcef
    else: CONFIG(debug, debug|release):LIBS += -LC:\CEF\3.3202.1682\Debug -llibcef

    CONFIG(release, debug|release):LIBS += -LC:\OPENCV\3.4.0\build\lib\Release -lopencv_world340
    else:CONFIG(debug, debug|release):LIBS += -LC:\OPENCV\3.4.0\build\lib\Debug -lopencv_world340d

    # Make the app icon be compiled into the bianary on windows
    HEADERS += ./src/res/resource.h
    RC_FILE = ./src/res/botfather.rc

    # Use the compatibility manifest required by CEF
    WINRT_MANIFEST = ./src/res/compatibility.manifest

    # Copy required dlls and CEF ressources
    CEF_DIR = C:/CEF/3.3202.1682

    CONFIG(release, debug|release):CEF_EXTRA_DIR = $${CEF_DIR}/Release/*
    else:CONFIG(debug, debug|release):CEF_EXTRA_DIR = $${CEF_DIR}/Debug/*
}

HEADERS += \
    ./src/android/adb_device_info.h \
    ./src/browser/browser.h \
    ./src/browser/browser_app.h \
    ./src/scripting/bot_thread.h \
    ./src/shared/settings.h \
    ./src/vision/vision.h \
    ./src/auth/authenticator.h \
    ./src/android/adb_wrapper.h \
    ./src/android/android_api.h \
    ./src/gui/browser_address_bar.h \
    ./src/gui/browser_window.h \
    ./src/gui/android_dialog.h \
    ./src/gui/auth_dialog.h \
    ./src/updater/update_checker.h \
    ./src/updater/updater_dialog.h \
    ./src/vision/vision_api.h \
    ./src/vision/match.h \
    ./src/vision/image.h \
    ./src/vision/hsv_color_factory.h \
    ./src/vision/hsv_color.h \
    ./src/vision/blob_tpl_factory.h \
    ./src/vision/blob_tpl.h \
    ./src/scripting/helper_api.h \
    ./src/scripting/bot.h \
    ./src/gui/control_window.h \
    ./src/gui/config_dialog.h \
    ./src/gui/browser_widget.h \
    ./src/browser/browser_client.h \
    ./src/browser/browser_api.h
SOURCES += ./src/android/adb_device_info.cpp \
    ./src/android/adb_wrapper.cpp \
    ./src/android/android_api.cpp \
    ./src/auth/authenticator.cpp \
    ./src/browser/browser.cpp \
    ./src/browser/browser_api.cpp \
    ./src/browser/browser_app.cpp \
    ./src/browser/browser_client.cpp \
    ./src/gui/android_dialog.cpp \
    ./src/gui/auth_dialog.cpp \
    ./src/gui/browser_address_bar.cpp \
    ./src/gui/browser_widget.cpp \
    ./src/gui/browser_window.cpp \
    ./src/gui/config_dialog.cpp \
    ./src/gui/control_window.cpp \
    ./src/main.cpp \
    ./src/scripting/bot.cpp \
    ./src/scripting/bot_thread.cpp \
    ./src/scripting/helper_api.cpp \
    ./src/updater/updater_dialog.cpp \
    ./src/updater/update_checker.cpp \
    ./src/vision/blob_tpl.cpp \
    ./src/vision/blob_tpl_factory.cpp \
    ./src/vision/hsv_color.cpp \
    ./src/vision/hsv_color_factory.cpp \
    ./src/vision/image.cpp \
    ./src/vision/match.cpp \
    ./src/vision/vision.cpp \
    ./src/vision/vision_api.cpp
FORMS += ./src/gui/config_dialog.ui \
    ./src/gui/control_window.ui \
    ./src/gui/browser_window.ui \
    ./src/updater/updater_dialog.ui \
    ./src/gui/auth_dialog.ui \
    ./src/gui/android_dialog.ui
RESOURCES += ./src/res/gui.qrc
