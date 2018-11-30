linux {

}

win32 {
    INCLUDEPATH += $$(LIBGIT2_ROOT)\include

    CONFIG(release, debug|release):LIBS += -L$$(LIBGIT2_ROOT)\build\Release -lgit2
    else:CONFIG(debug, debug|release):LIBS += -L$$(LIBGIT2_ROOT)\build\Debug -lgit2

    CONFIG(release, debug|release):QMAKE_POST_LINK += $$QMAKE_COPY $$(LIBGIT2_ROOT)/build/Release/git2.dll $${DESTDIR}$$escape_expand(\\n\\t)
    CONFIG(debug, debug|release):QMAKE_POST_LINK += $$QMAKE_COPY $$(LIBGIT2_ROOT)/build/Debug/git2.dll $${DESTDIR}$$escape_expand(\\n\\t)
}
