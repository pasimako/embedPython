QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    pyrun.cpp

HEADERS += \
    mainwindow.h \
    pyrun.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    embedPython.qrc

QMAKE_CXXFLAGS += -no-pie
QMAKE_LFLAGS += -no-pie
INCLUDEPATH += /usr/include/python3.11
LIBS += -L/usr/lib/x86_64-linux-gnu
LIBS += -Wl,-Bstatic -lpython3.11 -Wl,-Bdynamic
LIBS += -lz -lexpat -ldl -lutil
