QT       += core gui charts xml printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addcommand.cpp \
    adjustfeedstream.cpp \
    arrowlineitem.cpp \
    customdelegate.cpp \
    customgraphicsview.cpp \
    custompixmapitem.cpp \
    customshapeitem.cpp \
    main.cpp \
    mainwindow.cpp \
    userpreferences.cpp

HEADERS += \
    addcommand.h \
    adjustfeedstream.h \
    arrowlineitem.h \
    customdelegate.h \
    customgraphicsview.h \
    custompixmapitem.h \
    customshapeitem.h \
    mainwindow.h \
    userpreferences.h

FORMS += \
    adjustfeedstream.ui \
    mainwindow.ui \
    userpreferences.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

