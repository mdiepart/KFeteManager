TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        sha256.cpp

HEADERS += \
    sha256.h

DISTFILES += \
    LICENSE.txt
