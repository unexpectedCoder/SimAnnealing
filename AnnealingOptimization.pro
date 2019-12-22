QT += core

CONFIG += c++11 console

SOURCES += \
    annealing.cpp \
    main.cpp

HEADERS += \
    annealing.h

LIBS += -L$$PWD/../../../../usr/local/lib/ -lgsl
LIBS += -L$$PWD/../../../../usr/local/lib/ -lgslcblas

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/libgsl.a
PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/libgslcblas.a
