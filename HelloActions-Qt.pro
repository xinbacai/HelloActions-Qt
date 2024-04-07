QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets core gui sql network

CONFIG += c++11
TARGET= HelloActions-Qt

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    PublicFunction.cpp  \
    src/hlt_socket_demo.cpp \
    src/hlt_widget_demo.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
#    PublicFunction.h  \
    src/hlt_socket_demo.h \
    src/hlt_widget_demo.h \
    src/mainwindow.h

FORMS += \
    src/mainwindow.ui


INCLUDEPATH += $$PWD/include/hl7Interface

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
