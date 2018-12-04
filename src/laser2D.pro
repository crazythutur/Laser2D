QT += core
QT -= gui

CONFIG += c++17

TARGET = laser2D
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  Marmote.main.cpp \
            #QW_elementary_laser.main.cpp \
            QW_elementary_laser.cpp \
            Electron_presence.cpp \
            Elementary_laser.cpp \
            Laser_2D.cpp



HEADERS +=  ../inc/Electron_presence.h \
            ../inc/Elementary_laser.h \
            ../inc/QW_elementary_laser.h\
            ../inc/Laser.h

QMAKE_CXXFLAGS += -Wall -Wextra  -pedantic -ansi -Wwrite-strings -Wuninitialized -Wunreachable-code

#QMAKE_CXXFLAGS +=-Werror# Warning to Error at complilation

#DISTFILES += QW_elementary_laser.cpp Electron_presence.cpp Elementary_laser.cpp
