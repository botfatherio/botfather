linux {
    QMAKE_POST_LINK += cp -Ru $$PWD/linux/* $${DESTDIR}$$escape_expand(\\n\\t)
}

win32 {
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$PWD/windows/* $${DESTDIR}$$escape_expand(\\n\\t)
}
