DEFINES += DBTOOL_LIB

TEMPLATE = lib
TARGET = dbtool

CONFIG -= qt
CONFIG -= app_bundle
CONFIG += c++1x

LIBS += $$PWD/lib/libuser32.a
LIBS += $$PWD/lib/libodbc32.a

HEADERS += db/dbtool.h
SOURCES += db/dbtool.cpp
