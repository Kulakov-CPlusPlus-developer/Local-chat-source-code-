QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ClientChatWidget.cpp \
    ClientInfo.cpp \
    ClientManager.cpp \
    LoginWindow.cpp \
    MessageWidget.cpp \
    MessageWidgetMe.cpp \
    ipPort.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    ClientChatWidget.h \
    ClientInfo.h \
    ClientManager.h \
    LoginWindow.h \
    MainWindow.h \
    MessageWidget.h \
    MessageWidgetMe.h \
    ipPort.h

FORMS += \
    ClientChatWidget.ui \
    ClientInfo.ui \
    LoginWindow.ui \
    MessageWidget.ui \
    MessageWidgetMe.ui \
    ipPort.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Background.qrc
