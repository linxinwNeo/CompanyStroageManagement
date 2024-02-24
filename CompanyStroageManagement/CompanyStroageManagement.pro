QT       -= core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    AddNewModelWindow.cpp \
    Adjust_List_Item_Win.cpp \
    Algorithm/QuickSort.cpp \
    CN_Strings.cpp \
    CreateListWin.cpp \
    DataStructures/Container.cpp \
    DataStructures/Inventory.cpp \
    DataStructures/List.cpp \
    DataStructures/Model.cpp \
    DataStructures/client_info.cpp \
    DataStructures/entry.cpp \
    DataStructures/entrylist.cpp \
    FileLoader/IO_Base.cpp \
    FileLoader/ReadFile.cpp \
    FileLoader/WriteFile.cpp \
    GlobalVars.cpp \
    Others/create_pdf.cpp \
    Search_List_Win.cpp \
    SpanStrings.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AddNewModelWindow.h \
    Adjust_List_Item_Win.h \
    Algorithm/QuickSort.h \
    CN_Strings.h \
    CreateListWin.h \
    DataStructures/Container.h \
    DataStructures/Inventory.h \
    DataStructures/List.h \
    DataStructures/Model.h \
    DataStructures/client_info.h \
    DataStructures/entry.h \
    DataStructures/entrylist.h \
    FileLoader/IO_Base.h \
    FileLoader/ReadFile.h \
    FileLoader/WriteFile.h \
    GlobalVars.h \
    Others/create_pdf.h \
    Others/get_save_filePath.h \
    Others/is_int_float.h \
    Others/output_error_file.h \
    Predefined.h \
    Search_List_Win.h \
    SpanStrings.h \
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
DEPENDPATH += $$PWD/QXlsx

macx{
    LIBS += -L$$PWD/QXlsx/bin64/ -lQXlsx_Mac

    PRE_TARGETDEPS += $$PWD/QXlsx/bin64/libQXlsx_Mac.a
}


win32 {
    LIBS += -L$$PWD/QXlsx/bin64/ -lQXlsx_Win

    PRE_TARGETDEPS += $$PWD/QXlsx/bin64/libQXlsx_Win.a
}
