#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T20:24:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = embedPython
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pyrun.cpp

HEADERS  += mainwindow.h \
    pyrun.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/include/python3.4m

LIBS += -L/usr/lib/x86_64-linux-gnu

LIBS += -Wl,-Bstatic -lpython3.4m -Wl,-Bdynamic

LIBS += -lz -lexpat -ldl -lutil

RESOURCES += \
    embedPython.qrc
