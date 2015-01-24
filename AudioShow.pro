#-------------------------------------------------
#
# Project created by QtCreator 2014-06-29T23:10:58
#
#-------------------------------------------------

QT      += core gui network webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = AudioShow
TEMPLATE = app

QMAKE_CXXFLAGS = -std=c++11

SOURCES += main.cpp\
        widget.cpp \
    loginwidget.cpp \
    utils.cpp \
    runningline.cpp \
    programconfigwindow.cpp

HEADERS  += widget.h \
    loginwidget.h \
    utils.h \
    runningline.h \
    programconfigwindow.h

lessThan(QT_MAJOR_VERSION, 5){ #Qt 4.*
    LIBS += -LD:\\libs\\qjson-master-build\\src -llibqjson #path of QJson libs
    INCLUDEPATH += D:\\libs\\qjson-master\\include #path of QJson includes
}
