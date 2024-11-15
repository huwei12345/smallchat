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
    chateditfile.cpp \
    chateditimage.cpp \
    chatedittext.cpp \
    chatviewdelegate.cpp \
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
    loginsettingpage.cpp \
    loginwindow.cpp \
    main.cpp \
    mainpage.cpp \
    netthread.cpp \
    network.cpp \
    personcache.cpp \
    personcardcon.cpp \
    processor.cpp \
    processqueue.cpp \
    registerpage.cpp \
    request.cpp \
    settingpage.cpp \
    threadtest.cpp \
    tiplabel.cpp \
    userbutton.cpp

HEADERS += \
    MyProtocolStream.h \
    Protocol.h \
    Trans.h \
    chateditfile.h \
    chateditimage.h \
    chatedittext.h \
    chatviewdelegate.h \
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
    loginsettingpage.h \
    loginwindow.h \
    mainpage.h \
    netthread.h \
    network.h \
    personcache.h \
    personcardcon.h \
    processor.h \
    processqueue.h \
    registerpage.h \
    request.h \
    settingpage.h \
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
    loginsettingpage.ui \
    loginwindow.ui \
    mainpage.ui \
    registerpage.ui \
    settingpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    source.qrc
