TEMPLATE = subdirs
SUBDIRS = botfather webhelper
botfather.depends = webhelper

PACKAGE_PATH = $$OUT_PWD/package

botfather.files = $$OUT_PWD/botfather/BuildOutput/*
botfather.path = $${PACKAGE_PATH}

webhelper.files = $$OUT_PWD/webhelper/BuildOutput/*
webhelper.path = $${PACKAGE_PATH}

INSTALLS += botfather webhelper
