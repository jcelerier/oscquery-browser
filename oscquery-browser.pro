QT       += core gui network widgets

TARGET = oscquery-browser
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QMAKE_LFLAGS += -fsanitize=address -fsanitize=undefined
SOURCES += \
        main.cpp \
        mainwindow.cpp \
        QJsonModel/qjsonmodel.cpp

HEADERS += \
        mainwindow.h \
        QJsonModel/qjsonmodel.h

LIBS += -lossia

FORMS += \
        mainwindow.ui
