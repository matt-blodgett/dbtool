TEMPLATE = app
TARGET = dbtool

CONFIG -= qt
CONFIG -= app_bundle
CONFIG += c++1x
CONFIG += console

LIBS += $$PWD/lib/libuser32.a
LIBS += $$PWD/lib/libodbc32.a

SOURCES += main.cpp
HEADERS += db/dbtool.h
SOURCES += db/dberrors.cpp
HEADERS += db/dberrors.h
SOURCES += db/dbtool.cpp
