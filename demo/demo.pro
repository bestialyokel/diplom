QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport concurrent

CONFIG += c++17 c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    encoder.cpp \
    main.cpp \
    machine.cpp \
    qcustomplot.cpp \
    mainwindow.cpp

HEADERS += \
    encoder.h \
    machine.h \
    qcustomplot.h \
    types.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Resource/loading.gif

RESOURCES +=
