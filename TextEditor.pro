QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = TextEditor
TEMPLATE = app

SOURCES += \
    main.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    usermanager.cpp \
    filemanager.cpp

HEADERS += \
    loginwindow.h \
    mainwindow.h \
    usermanager.h \
    filemanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
