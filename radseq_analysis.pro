TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/process_file.cpp \
    src/structures.cpp \
    src/loci_to_extract.cpp \
    src/utils.cpp

HEADERS += \
    src/process_file.h \
    src/structures.h \
    src/loci_to_extract.h \
    src/utils.h

INCLUDEPATH += lib/zlib-1.2.11/
QMAKE_CXXFLAGS +=
QMAKE_LFLAGS += -lz
