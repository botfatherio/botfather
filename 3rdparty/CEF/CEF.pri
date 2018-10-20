linux {
    INCLUDEPATH += $$(CEF_ROOT)
    LIBS += -L$$(CEF_ROOT)/libcef_dll_wrapper/ -lcef_dll_wrapper

    CONFIG(release, debug|release):LIBS += -L$$(CEF_ROOT)/Release -lcef
    else: CONFIG(debug, debug|release):LIBS += -L$$(CEF_ROOT)/Debug -lcef

    # Delete 'libwidevinecdmadapter.so' and 'chrome-sandbox'
    QMAKE_POST_LINK += $$QMAKE_DEL_FILE $${DESTDIR}/libwidevinecdmadapter.so$$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_DEL_FILE $${DESTDIR}/chrome-sandbox$$escape_expand(\\n\\t)

    # Copy CEF resources and dependencies into the DESTDIR after building
    CONFIG(release, debug|release):QMAKE_POST_LINK += cp -Ru $$(CEF_ROOT)/Release/* $${DESTDIR}$$escape_expand(\\n\\t)
    CONFIG(debug, debug|release):QMAKE_POST_LINK += cp -Ru $$(CEF_ROOT)/Debug/* $${DESTDIR}$$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$(CEF_ROOT)/Resources/* $${DESTDIR}$$escape_expand(\\n\\t)
}

win32 {
    INCLUDEPATH += $$CEF_ROOT

    CONFIG(release, debug|release):LIBS += -L$$CEF_ROOT\libcef_dll_wrapper\Release -llibcef_dll_wrapper
    else:CONFIG(debug, debug|release):LIBS += -L$$CEF_ROOT\libcef_dll_wrapper\Debug -llibcef_dll_wrapper

    CONFIG(release, debug|release):LIBS += -L$$CEF_ROOT\Release -llibcef
    else: CONFIG(debug, debug|release):LIBS += -L$$CEF_ROOT\Debug -llibcef

    # Delete 'libwidevinecdmadapter.dll' and 'chrome-sandbox'
    QMAKE_POST_LINK += $$QMAKE_DEL_FILE $${DESTDIR}/libwidevinecdmadapter.so$$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_DEL_FILE $${DESTDIR}/chrome-sandbox$$escape_expand(\\n\\t)

    # Copy CEF resources and dependencies into the DESTDIR after building
    CONFIG(release, debug|release):QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$(CEF_ROOT)/Release/* $${DESTDIR}$$escape_expand(\\n\\t)
    CONFIG(debug, debug|release):QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$(CEF_ROOT)/Debug/* $${DESTDIR}$$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$(CEF_ROOT)/Resources/* $${DESTDIR}$$escape_expand(\\n\\t)
}
