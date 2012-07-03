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
    Referrer.cpp \
    GobanIterator.cpp \
    Goban.cpp \
    Finished.cpp \
    qtgoban.cpp \
    Rules/DoubleThree.cpp \
    Rules/VictoryCapturesRule.cpp \
    Rules/VictoryAlignment.cpp \
    Rules/TakingRules.cpp \
    Rules/NotEmptyRule.cpp \
    Rules/LastTaking.cpp \
    Rules/EachInTurnRule.cpp \
    realplayer.cpp \
    AI/aiplayer.cpp \
    connect_functor_helper.cpp \
    AI/alphabetathreading.cpp \
    Evaluator.cpp \
    PatternIdentifier.cpp \
    selectgametype.cpp

HEADERS  += mainwindow.h \
    Referrer.h \
    Goban.h \
    Finished.h \
    qtgoban.h \
    Rules/DoubleThree.h \
    Rules/VictoryCapturesRule.h \
    Rules/VictoryAlignment.h \
    Rules/TakingRules.h \
    Rules/NotEmptyRule.h \
    Rules/LastTaking.h \
    Rules/IRule.h \
    Rules/EachInTurnRule.h \
    GobanIterator.h \
    iplayer.h \
    realplayer.h \
    AI/aiplayer.h \
    connect_functor_helper.h \
    AI/alphabetathreading.h \
    PatternIdentifier.h \
    Evaluator.h \
    selectgametype.h

FORMS    += mainwindow.ui \
    Finished.ui \
    selectgametype.ui

RESOURCES += \
    images.qrc
