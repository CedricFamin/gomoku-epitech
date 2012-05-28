#-------------------------------------------------
#
# Project created by QtCreator 2012-05-08T10:52:18
#
#-------------------------------------------------

QT       += core gui

TARGET = gomoku
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++0x

SOURCES +=\
        mainwindow.cpp \
    main.cpp \
    TakingRules.cpp \
    VictoryAlignment.cpp \
    VictoryCapturesRule.cpp \
    NotEmptyRule.cpp \
    Referrer.cpp \
    LastTaking.cpp \
    GobanIterator.cpp \
    Goban.cpp \
    EachInTurnRule.cpp \
    DoubleThree.cpp \
    Finished.cpp \
    qtgoban.cpp

HEADERS  += mainwindow.h \
    Referrer.h \
    TakingRules.h \
    VictoryCapturesRule.h \
    VictoryAlignment.h \
    NotEmptyRule.h \
    Goban.h \
    IRule.h \
    LastTaking.h \
    EachInTurnRule.h \
    DoubleThree.h \
    Finished.h \
    qtgoban.h

FORMS    += mainwindow.ui \
    Finished.ui

RESOURCES += \
    images.qrc
