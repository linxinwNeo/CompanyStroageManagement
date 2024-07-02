QT -= core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    AddNewModelWindow.cpp \
    Adjust_List_Item_Win.cpp \
    Algorithm/QuickSort.cpp \
    CreateListWin.cpp \
    DataStructures/Container.cpp \
    DataStructures/Entry.cpp \
    DataStructures/Entrylist.cpp \
    DataStructures/Inventory.cpp \
    DataStructures/List.cpp \
    DataStructures/Model.cpp \
    DataStructures/client_info.cpp \
    FileLoader/IO_Base.cpp \
    FileLoader/ReadFile.cpp \
    FileLoader/WriteFile.cpp \
    GlobalVars.cpp \
    LanguageStrings.cpp \
    Menu/MainWindow_Actions.cpp \
    Others/create_pdf.cpp \
    Search_List_Win.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AddNewModelWindow.h \
    Adjust_List_Item_Win.h \
    Algorithm/QuickSort.h \
    CreateListWin.h \
    CustomWidgets/EnglishDoubleSpinBox.h \
    CustomWidgets/EnglishSpinBox.h \
    DataStructures/Container.h \
    DataStructures/Entry.h \
    DataStructures/Entrylist.h \
    DataStructures/Inventory.h \
    DataStructures/List.h \
    DataStructures/Model.h \
    DataStructures/client_info.h \
    FileLoader/IO_Base.h \
    FileLoader/ReadFile.h \
    FileLoader/WriteFile.h \
    GlobalVars.h \
    LanguageStrings.h \
    Others/create_pdf.h \
    Others/is_int_float.h \
    Others/write_error_file.h \
    Search_List_Win.h \
    helper_functions.h \
    mainwindow.h

FORMS += \
    AddNewModelWindow.ui \
    Adjust_List_Item_Win.ui \
    CreateListWin.ui \
    Search_List_Win.ui \
    mainwindow.ui


RC_FILE = exe_icon.rc



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    resources.qrc


INCLUDEPATH += $$PWD/QXlsx

macx{
    LIBS += -L$$PWD/QXlsx/bin64/ -lQXlsx_Mac
    PRE_TARGETDEPS += $$PWD/QXlsx/bin64/libQXlsx_Mac.a
}


win32 {
    LIBS += -L$$PWD/QXlsx/bin64/ -lQXlsx_Win
    PRE_TARGETDEPS += $$PWD/QXlsx/bin64/libQXlsx_Win.a
}
