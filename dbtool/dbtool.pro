TEMPLATE = lib
TARGET = dbtool

CONFIG += c++1x
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += DBTOOL_LIB

LIBS += $$PWD/lib/libuser32.a
LIBS += $$PWD/lib/libodbc32.a

HEADERS += dbtool.h
SOURCES += dbtool.cpp
