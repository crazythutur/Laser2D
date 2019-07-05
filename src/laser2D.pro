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
            Markov.main.cpp \
            Emitter.cpp \
            QW_emitter.cpp \
            state.cpp \
            Markov_Chain.cpp \
            analyse_tool.cpp

#            QW_elementary_laser.main.cpp


HEADERS +=  ../inc/Electron_presence.h \
            ../inc/Laser_2D.h \
            ../inc/LaserTransitionStructure.h \
            ../inc/rate_array.h \
            ../inc/Emitter.h \
            ../inc/QW_emitter.h \
            ../inc/state.h \
            ../inc/Markov_Chain.h \
            ../inc/analyse_tool.h


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


unix|win32: LIBS += -lgsl

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/release/ -lgslcblas
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/debug/ -lgslcblas
else:unix: LIBS += -L$$PWD/../../../../../../usr/local/lib/ -lgslcblas

INCLUDEPATH += $$PWD/../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../usr/local/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../usr/local/lib/release/libgslcblas.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../usr/local/lib/debug/libgslcblas.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../usr/local/lib/release/gslcblas.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../usr/local/lib/debug/gslcblas.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../../usr/local/lib/libgslcblas.a
