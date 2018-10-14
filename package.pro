TEMPLATE = subdirs
SUBDIRS = botfather botfather_helper
botfather.depends = botfather_helper

PACKAGE_PATH = $$OUT_PWD/package


botfather.files = $$OUT_PWD/botfather/BuildOutput/*
botfather.path = $${PACKAGE_PATH}

botfather_helper.files = $$OUT_PWD/botfather_helper/BuildOutput/*
botfather_helper.path = $${PACKAGE_PATH}

INSTALLS += botfather botfather_helper
