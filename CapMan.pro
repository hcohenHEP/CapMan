#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T15:51:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
INCLUDEPATH +=$(ROOTSYS)/include
LIBS += -L$(ROOTSYS)/lib -lCore -lThread -lRIO -lNet \
        -lHist -lGraf -lGraf3d -lGpad -lTree \
        -lRint -lPostscript -lMatrix -lPhysics \
        -lGui -lRGL -lMathCore -lGeom -lEve -lRGL -lGed -lMatrix -lRooFit -lRooFitCore -lMinuit -lASImage\
        -lTreePlayer -lMLP -lXMLIO -lTMVA

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

TARGET = CapMan
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    RootStaticDisplay.C \
    cutsdialog.cpp \
    imagedatabuilder.cpp \
    mvatrainer.cpp \
    imagehelper.cpp \
    singlefileprogressdialog.cpp

HEADERS  += mainwindow.h \
    RootStaticDisplayInterface.h \
    general.h \
    cutsdialog.h \
    imagedatabuilder.h \
    mvatrainer.h \
    imagehelper.h \
    singlefileprogressdialog.h

FORMS    += mainwindow.ui \
    cutsdialog.ui \
    singlefileprogressdialog.ui
