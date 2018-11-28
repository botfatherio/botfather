linux {
    QMAKE_POST_LINK += cp -Ru $$PWD/linux/* $${DESTDIR}
}

win32 {
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$PWD/windows/* $${DESTDIR}
}
