linux {
    INCLUDEPATH += $$(OPENCV_ROOT)/include
    LIBS += -L$$(OPENCV_ROOT)/build/lib -lopencv_features2d -lopencv_flann -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui

    # OpenCV must be installed on the system
    #LIBS += -lopencv_features2d -lopencv_flann -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui
}

win32 {
    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g
    INCLUDEPATH += $$(OPENCV_ROOT)\include

    CONFIG(release, debug|release):LIBS += -L$$(OPENCV_ROOT)\lib\Release -lopencv_world340
    else:CONFIG(debug, debug|release):LIBS += -L$$(OPENCV_ROOT)\lib\Debug -lopencv_world340d

    CONFIG(release, debug|release):QMAKE_POST_LINK +=$$quote(copy $$(OPENCV_ROOT)\\bin\\Release\\opencv_world*.dll $${DESTDIR_WIN}$$escape_expand(\\n\\t))
    CONFIG(debug, debug|release):QMAKE_POST_LINK +=$$quote(copy $$(OPENCV_ROOT)\\bin\\Debug\\opencv_world*.dll $${DESTDIR_WIN}$$escape_expand(\\n\\t))
}
