#-------------------------------------------------
#
# Project created by QtCreator 2019-03-06T08:33:45
#
#-------------------------------------------------

QT       += core gui widgets concurrent

CONFIG += c++11
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PauseAbortQt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    PythonLocker.cpp \
    PythonRunner.cpp

HEADERS += \
        mainwindow.h \
    PythonLocker.h \
    PythonRunner.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../../Aperture/Tools/WPy-3662/python-3.6.6.amd64/libs/ -lpython36_d
} else {
    LIBS += -L$$PWD/../../Aperture/Tools/WPy-3662/python-3.6.6.amd64/libs/ -lpython36
}

INCLUDEPATH += $$PWD/../../Aperture/Tools/WPy-3662/python-3.6.6.amd64/include
DEPENDPATH += $$PWD/../../Tools/WPy-3662/python-3.6.6.amd64/include
