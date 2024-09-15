QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += $$PWD/libs/libcomponents.a
INCLUDEPATH += $$PWD/libs/include/

SOURCES += \
    MyProtocolStream.cpp \
    chatwindow.cpp \
    creategrouppage.cpp \
    findfriendpage.cpp \
    framelessWidget.cpp \
    friendpage.cpp \
    main.cpp \
    mainpage.cpp \
    mainwindow.cpp \
    netthread.cpp \
    network.cpp \
    processor.cpp \
    processqueue.cpp \
    registerpage.cpp \
    request.cpp \
    threadtest.cpp \
    tiplabel.cpp \
    userbutton.cpp

HEADERS += \
    MyProtocolStream.h \
    Protocol.h \
    Trans.h \
    chatwindow.h \
    creategrouppage.h \
    findfriendpage.h \
    framelessWidget.h \
    friendpage.h \
    mainpage.h \
    mainwindow.h \
    netthread.h \
    network.h \
    processor.h \
    processqueue.h \
    registerpage.h \
    request.h \
    threadtest.h \
    tiplabel.h \
    userbutton.h

FORMS += \
    chatwindow.ui \
    creategrouppage.ui \
    findfriendpage.ui \
    framelessWidget.ui \
    friendpage.ui \
    mainpage.ui \
    mainwindow.ui \
    registerpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    source.qrc
