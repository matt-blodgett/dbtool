TEMPLATE = app
TARGET = dbtool

CONFIG -= qt
CONFIG -= app_bundle
CONFIG += c++1x
CONFIG += console

LIBS += $$PWD/lib/libuser32.a
LIBS += $$PWD/lib/libodbc32.a

SOURCES += main.cpp \
    db/dberrors.cpp
HEADERS += db/dbtool.h \
    db/dberrors.h
SOURCES += db/dbtool.cpp

DISTFILES += \
    tests/getmaniinfo \
    tests/misc.txt \
    tests/dbconsts.txt
