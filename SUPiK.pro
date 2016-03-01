#-------------------------------------------------
#
# Project created by QtCreator 2013-10-03T09:48:29
#
#-------------------------------------------------

QMAKE_TARGET_COMPANY = EvelSoft
QMAKE_TARGET_COPYRIGHT = EvelSoft
QMAKE_TARGET_PRODUCT = SUPiK
RC_ICONS = res/SUPiK.ico
VERSION = 1.0.0307

QT       += core gui sql xlsx ftp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SUPiK
TEMPLATE = app

# RC_FILE = SUPiK.rc

SOURCES += main.cpp\
        startwindow.cpp \
    supik.cpp \
    dialogs/s_2cdialog.cpp \
    dialogs/s_2ctdialog.cpp \
    gen/publicclass.cpp \
    gen/publiclang.cpp \
    gen/s_sql.cpp \
    gen/wh_func.cpp \
    models/s_duniversal.cpp \
    models/s_ncitem.cpp \
    models/s_ncmodel.cpp \
    models/s_ntitem.cpp \
    models/s_ntmodel.cpp \
    widgets/s_tqcalendarwidget.cpp \
    widgets/s_tqcheckbox.cpp \
    widgets/s_tqchoosewidget.cpp \
    widgets/s_tqcombobox.cpp \
    widgets/s_tqframe.cpp \
    widgets/s_tqgroupbox.cpp \
    widgets/s_tqlabel.cpp \
    widgets/s_tqlineedit.cpp \
    widgets/s_tqpushbutton.cpp \
    widgets/s_tqspinbox.cpp \
    widgets/s_tqtableview.cpp \
    widgets/s_tqtreeview.cpp \
    widgets/s_tqtreewidget.cpp \
    widgets/s_tqwidget.cpp \
    widgets/s_colortabbar.cpp \
    widgets/s_colortabwidget.cpp \
    widgets/s_maskedle.cpp \
    gen/s_tablefields.cpp \
    gen/ftp.cpp \
    startwindow.cpp \
    widgets/s_tqsplitter.cpp \
    dialogs/cmp/cmp_compdialog.cpp \
    dialogs/cmp/cmp_maindialog.cpp \
    dialogs/cmp/cmp_newsubsection.cpp \
    dialogs/dev/dev_devdialog.cpp \
    dialogs/dev/dev_docdialog.cpp \
    dialogs/dir/dir_adddialog.cpp \
    dialogs/dir/dir_maindialog.cpp \
    dialogs/sys/sys_backuprestoredirdialog.cpp \
    dialogs/sys/sys_importclass.cpp \
    dialogs/sys/sys_probsdialog.cpp \
    dialogs/sys/sys_settingsdialog.cpp \
    dialogs/sys/sys_systemdialog.cpp \
    dialogs/sys/sysdireditor.cpp \
    dialogs/sys/sysmenueditor.cpp \
    dialogs/wh/wh_dialog.cpp \
    dialogs/wh/wh_editor.cpp \
    dialogs/messagebox.cpp \
    dialogs/s_2tdialog.cpp \
    models/placedelegate.cpp \
    models/proxymodel.cpp \
    models/treeitem.cpp \
    models/treemodel.cpp \
    models/whplacestreemodel.cpp \
    threads/sys_importclass_t.cpp \
    threads/waitthread.cpp \
    widgets/errorprotocolwidget.cpp \
    widgets/s_tqstackedwidget.cpp \
    widgets/s_tqtoolbar.cpp \
    widgets/treeview.cpp \
    widgets/waitwidget.cpp \
    dialogs/s_accessdialog.cpp \
    models/editmodel.cpp \
    models/flowmodel.cpp \
    models/griddelegate.cpp \
    dialogs/chooseitemdialog.cpp \
    dialogs/dev/dev_maindialog.cpp \
    threads/checkthread.cpp \
    gen/ethernet.cpp

HEADERS  += startwindow.h \
    supik.h \
    dialogs/s_2cdialog.h \
    dialogs/s_2ctdialog.h \
    gen/publicclass.h \
    gen/publiclang.h \
    gen/s_sql.h \
    gen/wh_func.h \
    models/s_duniversal.h \
    models/s_ncitem.h \
    models/s_ncmodel.h \
    models/s_ntitem.h \
    models/s_ntmodel.h \
    widgets/s_tqcalendarwidget.h \
    widgets/s_tqcheckbox.h \
    widgets/s_tqchoosewidget.h \
    widgets/s_tqcombobox.h \
    widgets/s_tqframe.h \
    widgets/s_tqgroupbox.h \
    widgets/s_tqlabel.h \
    widgets/s_tqlineedit.h \
    widgets/s_tqpushbutton.h \
    widgets/s_tqspinbox.h \
    widgets/s_tqtableview.h \
    widgets/s_tqtreeview.h \
    widgets/s_tqtreewidget.h \
    widgets/s_tqwidget.h \
    widgets/s_colortabbar.h \
    widgets/s_colortabwidget.h \
    widgets/s_maskedle.h \
    gen/s_tablefields.h \
    startwindow.h \
    widgets/s_tqsplitter.h \
    dialogs/cmp/cmp_compdialog.h \
    dialogs/cmp/cmp_maindialog.h \
    dialogs/cmp/cmp_newsubsection.h \
    dialogs/dev/dev_devdialog.h \
    dialogs/dev/dev_docdialog.h \
    dialogs/dir/dir_adddialog.h \
    dialogs/dir/dir_maindialog.h \
    dialogs/sys/sys_backuprestoredirdialog.h \
    dialogs/sys/sys_importclass.h \
    dialogs/sys/sys_probsdialog.h \
    dialogs/sys/sys_settingsdialog.h \
    dialogs/sys/sys_systemdialog.h \
    dialogs/sys/sysdireditor.h \
    dialogs/sys/sysmenueditor.h \
    dialogs/wh/wh_dialog.h \
    dialogs/wh/wh_editor.h \
    dialogs/messagebox.h \
    dialogs/s_2tdialog.h \
    gen/ftp.h \
    models/placedelegate.h \
    models/proxymodel.h \
    models/treeitem.h \
    models/treemodel.h \
    models/whplacestreemodel.h \
    threads/sys_importclass_t.h \
    threads/waitthread.h \
    widgets/errorprotocolwidget.h \
    widgets/s_tqstackedwidget.h \
    widgets/s_tqtoolbar.h \
    widgets/treeview.h \
    widgets/waitwidget.h \
    dialogs/s_accessdialog.h \
    models/editmodel.h \
    models/flowmodel.h \
    models/griddelegate.h \
    dialogs/chooseitemdialog.h \
    dialogs/dev/dev_maindialog.h \
    threads/checkthread.h \
    gen/ethernet.h

RESOURCES += \
    pic.qrc

CONFIG += embed_manifest_exe

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../mysql/lib/ -llibmysql
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../mysql/lib/ -llibmysqld
else:unix: LIBS += -L$$PWD/../../mysql/lib/ -llibmysql

INCLUDEPATH += $$PWD/../../mysql/include
DEPENDPATH += $$PWD/../../mysql/include

INCLUDEPATH += $$PWD/../xlsxwriter/src/xlsx
DEPENDPATH += $$PWD/../xlsxwriter/src/xlsx
