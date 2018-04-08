TARGET = QtInstagram

load(qt_module)
QT -= gui
QT += network

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

DEFINES += LIBQINSTAGRAM_LIBRARY

SOURCES += \
    $$PWD/instagram.cpp \
    $$PWD/instagramquery.cpp

HEADERS += \
    $$PWD/instagram.h \
    $$PWD/libqinstagram_global.h \
    $$PWD/instagramquery.h
