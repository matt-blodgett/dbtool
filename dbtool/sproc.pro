TEMPLATE = app
TARGET = sproc

CONFIG -= qt
CONFIG -= app_bundle

CONFIG += c++1x
CONFIG += console

LIBS += $$PWD/lib/libuser32.a
LIBS += $$PWD/lib/libodbc32.a

SOURCES += sproc.cpp
HEADERS += dbtool.h
SOURCES += dbtool.cpp
