QT-= core
QT -= gui

CONFIG += c++17

TARGET = laser2D
CONFIG += console
#CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
            #QW_elementary_laser.main.cpp \
            QW_elementary_laser.cpp \
            Electron_presence.cpp \
            Elementary_laser.cpp \
            Laser_2D.cpp \
            LaserTransitionStructure.cpp \
            rate_array.cpp \
            Marmote.main.cpp \

#            QW_elementary_laser.main.cpp


HEADERS +=  ../inc/Electron_presence.h \
            ../inc/Elementary_laser.h \
            ../inc/QW_elementary_laser.h\
            ../inc/Laser_2D.h \
            ../inc/LaserTransitionStructure.h \
            ../inc/rate_array.h

QMAKE_CXXFLAGS += -Wall -Wextra  -pedantic -ansi -Wwrite-strings -Wuninitialized -Wunreachable-code

#QMAKE_CXXFLAGS +=-Werror# Warning to Error at complilation

QMAKE_EXT_CPP += .main




#MARMOTE libraries
LIBS += -L/home/thutur/marmotecore_1.2.3/lib -lMarmoteCore -lXborne -lpsi

LIBS += -lboost_thread -lboost_system
INCLUDEPATH += /home/thutur/marmotecore_1.2.3
