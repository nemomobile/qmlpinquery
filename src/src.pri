QT += dbus declarative
CONFIG += mobility debug
//MOBILITY += systeminfo

SOURCES += $$PWD/main.cpp \
           $$PWD/ofonosimif.cpp

HEADERS += $$PWD/ofonosimif.h

INCLUDEPATH += $$PWD

LIBS += /usr/lib/libofono-qt.so
