#
# @class State
 #
 # @author Arthur Vallet
 #
 # @version 1.0
 #
 # @date $Date: 02/07/2019$
 #
 #
 # @brief Description of a state that caracterise the Markov Chain
 #
 # the state is caracterised by a number of variable that describe enterely the state.
 # Each state is caracterised by the number of photon for each mode stocked in @a photons,
 # the number of elctron in each band (Conduction and Valence bands) stocked in @a $
 # CB_electrons and @a VB_electrons.
 #
 # This file is part of Laser2D.
 #
 #   Laser2D is free software: you can redistribute it and/or modify
 #   it under the terms of the GNU General Public License as published by
 #   the Free Software Foundation, either version 3 of the License, or
 #   (at your option) any later version.
 #
 #   Laser2D is distributed in the hope that it will be useful,
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #   GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License
 #   along with Laser2D.  If not, see <https://www.gnu.org/licenses/>.
 #/

QT-= core
QT -= gui

CONFIG += c++11

TARGET = laser2D.exe
CONFIG += console
CONFIG -= app_bundle

QMAKE_CFLAGS -= -O2
QMAKE_LFLAGS =



TEMPLATE = app

SOURCES +=  \
            #QW_elementary_laser.main.cpp \
            Electron_presence.cpp \
            Laser_2D.cpp \
#            LaserTransitionStructure.cpp \
            rate_array.cpp \
            main.cpp \
            Emitter.cpp \
            QW_emitter.cpp \
            state.cpp \
            Markov_Chain.cpp \
            analyse_tool.cpp \
            io.cpp \
            ../lib/argtable3.c

#            QW_elementary_laser.main.cpp


HEADERS +=  ../inc/Electron_presence.h \
            ../inc/Laser_2D.h \
#            ../inc/LaserTransitionStructure.h \
            ../inc/rate_array.h \
            ../inc/Emitter.h \
            ../inc/QW_emitter.h \
            ../inc/state.h \
            ../inc/Markov_Chain.h \
            ../inc/analyse_tool.h \
            ../inc/io.h \
            ../lib/argtable3.h


   QMAKE_CXXFLAGS_RELEASE -= -O1
   QMAKE_CXXFLAGS_RELEASE -= -O2
   QMAKE_CXXFLAGS_RELEASE += -O3

   QMAKE_CXXFLAGS += -Wall -Wextra  -pedantic -ansi -Wwrite-strings -Wuninitialized -Wunreachable-code

#QMAKE_CXXFLAGS +=-Werror# Warning to Error at complilation

QMAKE_EXT_CPP += .main




#MARMOTE libraries
#LIBS += -L/home/thutur/Work/programmes/Marmote/marmotecore_1.2.4/lib -lMarmoteCore -lXborne -lpsi

#LIBS += -lboost_thread -lboost_system
#INCLUDEPATH += /home/thutur/Work/programmes/Marmote/marmotecore_1.2.4/



#dox.target = doc
#dox.commands = doxygen doc/doxyfile.cfg;

#dox.depends =

...
# somewhere else in the *.pro file
#QMAKE_EXTRA_UNIX_TARGETS += dox


unix|win32: LIBS += -lgsl

unix: LIBS += -L/usr/local/lib/ -lgslcblas

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include

unix: PRE_TARGETDEPS += /usr/local/lib/libgslcblas.a


unix|win32:CONFIG(profile): QMAKE_CXXFLAGS += -pg -g
