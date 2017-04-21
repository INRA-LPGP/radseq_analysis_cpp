TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/utils.cpp \
    src/bootstrap.cpp \
    src/haplotypes.cpp

HEADERS += \
    src/utils.h \
    src/bootstrap.h \
    src/haplotypes.h

INCLUDEPATH += lib/zlib-1.2.11/
QMAKE_CXXFLAGS +=
QMAKE_LFLAGS += -lz
