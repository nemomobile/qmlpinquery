TEMPLATE = app
CONFIG += ordered hide_symbols
QT += dbus
TARGET = $$PROJECT_NAME
CONFIG -= app_bundle # OS X

CONFIG += link_pkgconfig

packagesExist(qdeclarative-boostable) {
    message("Building with qdeclarative-boostable support")
    DEFINES += HAS_BOOSTER
    PKGCONFIG += qdeclarative-boostable
} else {
    warning("qdeclarative-boostable not available; startup times will be slower")
}

equals(QT_MAJOR_VERSION, 4):  {
    PKGCONFIG += qofono
    QT += declarative
    PROJECT_NAME = qmlpinquery
    QML_FILES = qml/*.qml
    JS_FILES = *qml/.js
}

equals(QT_MAJOR_VERSION, ):  {
    PKGCONFIG += qofono-qt5
    QT += qml
    PROJECT_NAME = qmlpinquery-qt5
    QML_FILES = qml2/*.qml
    JS_FILES = *qml2/.js
}

SOURCES += src/main.cpp \
           src/ofonosimif.cpp

HEADERS += src/ofonosimif.h
RESOURCES += res.qrc


OTHER_FILES += $${QML_FILES} $${JS_FILES}

target.path = $$INSTALL_ROOT/usr/bin/
INSTALLS += target

desktop.files = $${PROJECT_NAME}.desktop
desktop.path = $$INSTALL_ROOT/usr/share/applications
INSTALLS += desktop

# qml API we provide
qml_api.files = qml/api/*
qml_api.path = $$[QT_INSTALL_IMPORTS]/org/nemomobile/$$PROJECT_NAME
INSTALLS += qml_api

TRANSLATIONS += *.qml
dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION} &&
dist.commands += git clone . $${PROJECT_NAME}-$${VERSION} &&
dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION}/.git &&
dist.commands += rm -f $${PROJECT_NAME}-$${VERSION}/.gitignore &&
dist.commands += mkdir -p $${PROJECT_NAME}-$${VERSION}/ts &&
dist.commands += lupdate $${TRANSLATIONS} -ts $${PROJECT_NAME}-$${VERSION}/ts/$${PROJECT_NAME}.ts &&
dist.commands += tar jcpvf $${PROJECT_NAME}-$${VERSION}.tar.bz2 $${PROJECT_NAME}-$${VERSION} &&
dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION}
QMAKE_EXTRA_TARGETS += dist

OTHER_FILES += rpm/*
