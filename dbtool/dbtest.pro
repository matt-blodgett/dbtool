TEMPLATE = app
TARGET = dbtool

CONFIG -= qt
CONFIG -= app_bundle

CONFIG += c++1x
CONFIG += console

LIBS += $$PWD/lib/libuser32.a
LIBS += $$PWD/lib/libodbc32.a

#LIBS += C:/dev/dbtool/build-dbtool-Desktop_Qt_5_11_1_MinGW_32bit-Debug/debug/libdbtool.a

SOURCES += main.cpp
HEADERS += dbtool.h
SOURCES += dbtool.cpp
