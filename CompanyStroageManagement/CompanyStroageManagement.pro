QT       -= core gui

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
    Algorithm/QuickSort.cpp \
    CreateList.cpp \
    DataStructures/Container.cpp \
    DataStructures/Inventory.cpp \
    DataStructures/Model.cpp \
    DataStructures/client_info.cpp \
    DataStructures/entry.cpp \
    DataStructures/entrylist.cpp \
    FileLoader/ReadFile.cpp \
    FileLoader/WriteFile.cpp \
    GlobalVars.cpp \
    QXlsx/Excel.cpp \
    create_PDF.cpp \
    flags.cpp \
    main.cpp \
    mainwindow.cpp \
    submitEntry.cpp

HEADERS += \
    Algorithm/QuickSort.h \
    CreateList.h \
    DataStructures/Container.h \
    DataStructures/Inventory.h \
    DataStructures/Model.h \
    DataStructures/client_info.h \
    DataStructures/entry.h \
    DataStructures/entrylist.h \
    FileLoader/ReadFile.h \
    FileLoader/WriteFile.h \
    GlobalVars.h \
    Predefined.h \
    QXlsx/Excel.h \
    QXlsx/QXlsxHeaders.h \
    flags.h \
    helper_functions.h \
    mainwindow.h \
    submitEntry.h

FORMS += \
    CreateList.ui \
    mainwindow.ui \
    submitEntry.ui

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


DISTFILES +=

RESOURCES += \
    resources.qrc

