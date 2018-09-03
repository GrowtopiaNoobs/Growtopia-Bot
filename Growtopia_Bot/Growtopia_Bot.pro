#-------------------------------------------------
#
# Project created by QtCreator 2018-07-31T20:51:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Growtopia_Bot
TEMPLATE = app

INCLUDEPATH += $$PWD/include
LIBS += -L"$$PWD/enet2" -lenet
LIBS += -lws2_32
LIBS += -lwinmm

LIBS += -L"$$PWD/lib"

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

#LIBS += -lopengl32
#LIBS += -L"$$PWD/lib" -lsfml-graphics-s
#LIBS += -lsfml-window-s
#LIBS += -lsfml-system-s
#LIBS += -l

RC_ICONS = sethmumuico.ico



SOURCES += main.cpp\
        mainwindow.cpp \
    addbots.cpp

HEADERS  += mainwindow.h \
    corefunc.h \
    userfunc.h \
    utilsfunc.h \
    addbots.h

FORMS    += mainwindow.ui \
    addbots.ui
