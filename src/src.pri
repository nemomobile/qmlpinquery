QT += dbus declarative
CONFIG += mobility debug

SOURCES += $$PWD/main.cpp \
           $$PWD/ofonosimif.cpp

HEADERS += $$PWD/ofonosimif.h

INCLUDEPATH += $$PWD

LIBS += /usr/lib/libqofono.so
