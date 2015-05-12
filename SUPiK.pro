#-------------------------------------------------
#
# Project created by QtCreator 2013-10-03T09:48:29
#
#-------------------------------------------------

QT       += core gui sql xlsx

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SUPiK
TEMPLATE = app

RC_FILE = SUPiK.rc

SOURCES += main.cpp\
        startwindow.cpp \
    supik.cpp \
    dialogs/cmp_compdialog.cpp \
    dialogs/dir_adddialog.cpp \
    dialogs/dir_maindialog.cpp \
    dialogs/s_2cdialog.cpp \
    dialogs/s_2ctdialog.cpp \
    dialogs/s_acceptdialog.cpp \
    dialogs/s_accessdialog.cpp \
    dialogs/s_ncdialog.cpp \
    dialogs/s_sqlfieldsdialog.cpp \
    dialogs/s_sqltreedialog.cpp \
    dialogs/s_tablefilterdialog.cpp \
    dialogs/sys_backuprestoredirdialog.cpp \
    dialogs/sys_cantdeletedialog.cpp \
    dialogs/sys_probsdialog.cpp \
    dialogs/sys_settingsdialog.cpp \
    dialogs/sys_systemdialog.cpp \
    dialogs/wh_dialog.cpp \
    gen/publicclass.cpp \
    gen/publiclang.cpp \
    gen/s_sql.cpp \
    gen/wh_func.cpp \
    models/s_aitem.cpp \
    models/s_aitemmodel.cpp \
    models/s_duniversal.cpp \
    models/s_ncitem.cpp \
    models/s_ncmodel.cpp \
    models/s_ntitem.cpp \
    models/s_ntmodel.cpp \
    models/s_whitem.cpp \
    models/s_whitemmodel.cpp \
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
    dialogs/sys_acceptexist.cpp

HEADERS  += startwindow.h \
    supik.h \
    dialogs/cmp_compdialog.h \
    dialogs/dir_adddialog.h \
    dialogs/dir_maindialog.h \
    dialogs/s_2cdialog.h \
    dialogs/s_2ctdialog.h \
    dialogs/s_acceptdialog.h \
    dialogs/s_accessdialog.h \
    dialogs/s_ncdialog.h \
    dialogs/s_sqlfieldsdialog.h \
    dialogs/s_sqltreedialog.h \
    dialogs/s_tablefilterdialog.h \
    dialogs/sys_backuprestoredirdialog.h \
    dialogs/sys_cantdeletedialog.h \
    dialogs/sys_probsdialog.h \
    dialogs/sys_settingsdialog.h \
    dialogs/sys_systemdialog.h \
    dialogs/wh_dialog.h \
    gen/publicclass.h \
    gen/publiclang.h \
    gen/s_sql.h \
    gen/wh_func.h \
    models/s_aitem.h \
    models/s_aitemmodel.h \
    models/s_duniversal.h \
    models/s_ncitem.h \
    models/s_ncmodel.h \
    models/s_ntitem.h \
    models/s_ntmodel.h \
    models/s_whitem.h \
    models/s_whitemmodel.h \
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
    dialogs/sys_acceptexist.h

RESOURCES += \
    pic.qrc

win32:CONFIG(release, debug|release) : LIBS += -L$$PWD/../../Build/libarchive-2.4.12-1-bin/lib/ -llibarchive -l$$PWD/../xlsxwriter/lib/

CONFIG += embed_manifest_exe

INCLUDEPATH += $$PWD/../../Build/libarchive-2.4.12-1-bin/include
DEPENDPATH += $$PWD/../../Build/libarchive-2.4.12-1-bin/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../mysql-5.6.22-winx64/lib/ -llibmysql
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../mysql-5.6.22-winx64/lib/ -llibmysqld
else:unix: LIBS += -L$$PWD/../../mysql-5.6.22-winx64/lib/ -llibmysql

INCLUDEPATH += $$PWD/../../mysql-5.6.22-winx64/include
DEPENDPATH += $$PWD/../../mysql-5.6.22-winx64/include

INCLUDEPATH += $$PWD/../xlsxwriter/src/xlsx
DEPENDPATH += $$PWD/../xlsxwriter/src/xlsx
