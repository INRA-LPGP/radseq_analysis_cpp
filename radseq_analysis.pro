TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp

HEADERS +=

INCLUDEPATH += lib/zlib-1.2.11/
QMAKE_CXXFLAGS +=
QMAKE_LFLAGS += -lz
