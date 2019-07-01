QT-= core
QT -= gui

CONFIG += c++17

TARGET = laser2D
CONFIG += console#CONFIG -= app_bundle



TEMPLATE = app

SOURCES +=  \
            #QW_elementary_laser.main.cpp \
            Electron_presence.cpp \
            Laser_2D.cpp \
            LaserTransitionStructure.cpp \
            rate_array.cpp \
            Marmote.main.cpp \
            Emitter.cpp \
            QW_emitter.cpp

#            QW_elementary_laser.main.cpp


HEADERS +=  ../inc/Electron_presence.h \
            ../inc/Laser_2D.h \
            ../inc/LaserTransitionStructure.h \
            ../inc/rate_array.h \
            ../inc/Emitter.h \
            ../inc/QW_emitter.h

QMAKE_CXXFLAGS += -Wall -Wextra  -pedantic -ansi -Wwrite-strings -Wuninitialized -Wunreachable-code

#QMAKE_CXXFLAGS +=-Werror# Warning to Error at complilation

QMAKE_EXT_CPP += .main




#MARMOTE libraries
LIBS += -L/home/thutur/Work/programmes/Marmote/marmotecore_1.2.4/lib -lMarmoteCore -lXborne -lpsi

LIBS += -lboost_thread -lboost_system
INCLUDEPATH += /home/thutur/Work/programmes/Marmote/marmotecore_1.2.4/



#dox.target = doc
#dox.commands = doxygen doc/doxyfile.cfg;

#dox.depends =

...
# somewhere else in the *.pro file
#QMAKE_EXTRA_UNIX_TARGETS += dox

