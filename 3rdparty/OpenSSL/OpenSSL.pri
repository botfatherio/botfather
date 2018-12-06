linux {

}

win32 {
    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g

    #QMAKE_POST_LINK +=$$quote(copy $$(OPENSSL_ROOT)\\libeay32.dll $${DESTDIR_WIN}$$escape_expand(\\n\\t))
    #QMAKE_POST_LINK +=$$quote(copy $$(OPENSSL_ROOT)\\ssleay32.dll $${DESTDIR_WIN}$$escape_expand(\\n\\t))

    PWD_WIN = $$PWD
    PWD_WIN ~= s,/,\\,g
    QMAKE_POST_LINK +=$$quote(cmd /c xcopy /E /y $${PWD_WIN}\\windows\\* $${DESTDIR_WIN}$$escape_expand(\\n\\t))
}
