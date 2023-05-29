QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=./QXlsx                # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./QXlsx/header/    # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./QXlsx/source/    # current QXlsx source path is ./source/
include(./QXlsx/QXlsx.pri)

SOURCES += \
    DataStructures/client_info.cpp \
    DataStructures/entry.cpp \
    DataStructures/entrylist.cpp \
    DataStructures/model.cpp \
    DataStructures/stroage.cpp \
    GlobalVars.cpp \
    QXlsx/Excel.cpp \
    addbackwindow.cpp \
    create_PDF.cpp \
    flags.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    DataStructures/client_info.h \
    DataStructures/entry.h \
    DataStructures/entrylist.h \
    DataStructures/model.h \
    DataStructures/stroage.h \
    GlobalVars.h \
    Predefined.h \
    QXlsx/Excel.h \
    QXlsx/QXlsxHeaders.h \
    addbackwindow.h \
    flags.h \
    helper_functions.h \
    mainwindow.h

FORMS += \
    addbackwindow.ui \
    mainwindow.ui

win32 {
    RC_FILE = exe_icon.rc
}

macx: {
    ICON = ./resources/icon.icns
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES += \

RESOURCES += \
    resources.qrc

