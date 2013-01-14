#-------------------------------------------------
#
# Project created by QtCreator 2013-01-05T17:54:41
#
#-------------------------------------------------

QT       += core gui network xml

TARGET = QNaviFirm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serverinterface.cpp \
    qtsoap.cpp \
    models/productlistmodel.cpp \
    models/releaselistmodel.cpp

HEADERS  += mainwindow.h \
    serverinterface.h \
    qtsoap.h \
    models/productlistmodel.h \
    defines.h \
    models/releaselistmodel.h

FORMS    += mainwindow.ui
