linux {
    QMAKE_POST_LINK += cp -Ru $$PWD/linux/* $${DESTDIR}$$escape_expand(\\n\\t)
}

win32 {
    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g
    PWD_WIN = $$PWD
    PWD_WIN ~= s,/,\\,g
    QMAKE_POST_LINK +=$$quote(cmd /c xcopy /E /y $${PWD_WIN}\\windows\\* $${DESTDIR_WIN}$$escape_expand(\\n\\t))
}
