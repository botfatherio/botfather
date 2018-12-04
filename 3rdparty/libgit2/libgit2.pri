linux {

}

win32 {
    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g
    INCLUDEPATH += $$(LIBGIT2_ROOT)\include

    CONFIG(release, debug|release):LIBS += -L$$(LIBGIT2_ROOT)\build\Release -lgit2
    else:CONFIG(debug, debug|release):LIBS += -L$$(LIBGIT2_ROOT)\build\Debug -lgit2

    CONFIG(release, debug|release):QMAKE_POST_LINK +=$$quote(copy $$(LIBGIT2_ROOT)\\build\\Release\\git2.dll $${DESTDIR_WIN}$$escape_expand(\\n\\t))
    CONFIG(debug, debug|release):QMAKE_POST_LINK +=$$quote(copy $$(LIBGIT2_ROOT)\\build\\Debug\\git2.dll $${DESTDIR_WIN}$$escape_expand(\\n\\t))
}
