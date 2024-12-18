QT       += core gui network
QT += quickwidgets
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
    clientpersoninfo.cpp \
    creategrouppage.cpp \
    emojiselector.cpp \
    findfriendpage.cpp \
    framelessWidget.cpp \
    friendpage.cpp \
    ftp/ftpmanager.cpp \
    ftp/qftp.cpp \
    ftp/qurlinfo.cpp \
    ftpsender.cpp \
    globalvaria.cpp \
    groupchatwindow.cpp \
    main.cpp \
    mainpage.cpp \
    mainwindow.cpp \
    netthread.cpp \
    network.cpp \
    personcache.cpp \
    personcardcon.cpp \
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
    clientpersoninfo.h \
    creategrouppage.h \
    emojiselector.h \
    findfriendpage.h \
    framelessWidget.h \
    friendpage.h \
    ftp/FtpManager.h \
    ftp/qftp.h \
    ftp/qurlinfo.h \
    ftpsender.h \
    globalvaria.h \
    groupchatwindow.h \
    mainpage.h \
    mainwindow.h \
    netthread.h \
    network.h \
    personcache.h \
    personcardcon.h \
    processor.h \
    processqueue.h \
    registerpage.h \
    request.h \
    soft.h \
    threadtest.h \
    tiplabel.h \
    userbutton.h

FORMS += \
    chatwindow.ui \
    creategrouppage.ui \
    emojiselector.ui \
    findfriendpage.ui \
    framelessWidget.ui \
    friendpage.ui \
    groupchatwindow.ui \
    mainpage.ui \
    mainwindow.ui \
    registerpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    source.qrc
