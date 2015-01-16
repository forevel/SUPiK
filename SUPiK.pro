#-------------------------------------------------
#
# Project created by QtCreator 2013-10-03T09:48:29
#
#-------------------------------------------------

QT       += core gui sql xlsx

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SUPiK
TEMPLATE = app

RC_FILE = res/SUPiK.rc

SOURCES += cpp/main.cpp\
        cpp/qtms_mw.cpp \
    cpp/supik.cpp \
    cpp/publicclass.cpp \
    cpp/publiclang.cpp \
    cpp/sys_settingsdialog.cpp \
    cpp/s_colortabwidget.cpp \
    cpp/s_colortabbar.cpp \
    cpp/s_tqlabel.cpp \
    cpp/s_tqlineedit.cpp \
    cpp/s_tqpushbutton.cpp \
    cpp/s_tqcombobox.cpp \
    cpp/sys_systemdialog.cpp \
    cpp/s_tqtreewidget.cpp \
    cpp/sys_cantdeletedialog.cpp \
    cpp/s_tqgroupbox.cpp \
    cpp/s_tqcheckbox.cpp \
    cpp/sys_acceptexist.cpp \
    cpp/s_tqtableview.cpp \
    cpp/s_tqframe.cpp \
    cpp/cmp_compdialog.cpp \
    cpp/dir_maindialog.cpp \
    cpp/s_aitemmodel.cpp \
    cpp/s_aitem.cpp \
    cpp/s_tqtreeview.cpp \
    cpp/s_sqlfieldsdialog.cpp \
    cpp/s_sql.cpp \
    cpp/s_sqltreedialog.cpp \
    cpp/s_tablefilterdialog.cpp \
    cpp/s_accessdialog.cpp \
    cpp/wh_dialog.cpp \
    cpp/s_whitemmodel.cpp \
    cpp/s_whitem.cpp \
    cpp/s_tqwidget.cpp \
    cpp/dir_adddialog.cpp \
    cpp/s_tqspinbox.cpp \
    cpp/s_acceptdialog.cpp \
    cpp/s_tqcalendarwidget.cpp \
    cpp/sys_backuprestoredirdialog.cpp \
    cpp/wh_func.cpp \
    cpp/sys_probsdialog.cpp \
    cpp/s_2cdialog.cpp \
    cpp/s_ncitem.cpp \
    cpp/s_ncmodel.cpp \
    cpp/s_duniversal.cpp \
    cpp/s_maskedle.cpp \
    cpp/s_ncdialog.cpp \
    cpp/s_tqchoosewidget.cpp \
    cpp/s_dlgeditdialog.cpp \
    cpp/s_ntitem.cpp \
    cpp/s_ntmodel.cpp \
    cpp/s_2ctdialog.cpp \
    cpp/sys_dialogconstdialog.cpp

HEADERS  += inc/qtms_mw.h \
    inc/supik.h \
    inc/publicclass.h \
    inc/publiclang.h \
    inc/sys_settingsdialog.h \
    inc/s_colortabwidget.h \
    inc/s_colortabbar.h \
    inc/s_tqlabel.h \
    inc/s_tqlineedit.h \
    inc/s_tqpushbutton.h \
    inc/s_tqcombobox.h \
    inc/sys_systemdialog.h \
    inc/s_tqtreewidget.h \
    inc/sys_cantdeletedialog.h \
    inc/s_tqgroupbox.h \
    inc/s_tqcheckbox.h \
    inc/sys_acceptexist.h \
    inc/s_tqtableview.h \
    inc/s_tqframe.h \
    inc/cmp_compdialog.h \
    inc/dir_maindialog.h \
    inc/s_aitemmodel.h \
    inc/s_aitem.h \
    inc/s_tqtreeview.h \
    inc/s_sqlfieldsdialog.h \
    inc/s_sql.h \
    inc/s_sqltreedialog.h \
    inc/s_tablefilterdialog.h \
    inc/s_accessdialog.h \
    inc/wh_dialog.h \
    inc/s_whitemmodel.h \
    inc/s_whitem.h \
    inc/s_tqwidget.h \
    inc/dir_adddialog.h \
    inc/s_tqspinbox.h \
    inc/s_acceptdialog.h \
    inc/s_tqcalendarwidget.h \
    inc/sys_backuprestoredirdialog.h \
    inc/wh_func.h \
    inc/sys_probsdialog.h \
    inc/s_2cdialog.h \
    inc/s_ncitem.h \
    inc/s_ncmodel.h \
    inc/s_duniversal.h \
    inc/s_maskedle.h \
    inc/s_ncdialog.h \
    inc/s_tqchoosewidget.h \
    inc/s_dlgeditdialog.h \
    inc/s_ntitem.h \
    inc/s_ntmodel.h \
    inc/s_2ctdialog.h \
    inc/sys_dialogconstdialog.h

FORMS    += \
    ui/qt_alt_addcompdialog.ui \
    ui/qt_alt_editcompmodeldialog.ui \
    ui/qt_sge_addcompdialog.ui

RESOURCES += \
    res/pic.qrc

win32:CONFIG(release, debug|release) : LIBS += -L$$PWD/../../Build/libarchive-2.4.12-1-bin/lib/ -llibarchive -l$$PWD/../xlsxwriter/lib/

CONFIG += embed_manifest_exe

INCLUDEPATH += $$PWD/../../Build/libarchive-2.4.12-1-bin/include
DEPENDPATH += $$PWD/../../Build/libarchive-2.4.12-1-bin/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../mysql-5.6.20-winx64/lib/ -llibmysql
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../mysql-5.6.20-winx64/lib/ -llibmysqld
else:unix: LIBS += -L$$PWD/../../mysql-5.6.20-winx64/lib/ -llibmysql

INCLUDEPATH += $$PWD/../../mysql-5.6.20-winx64/include
DEPENDPATH += $$PWD/../../mysql-5.6.20-winx64/include

INCLUDEPATH += $$PWD/../xlsxwriter/src/xlsx
DEPENDPATH += $$PWD/../xlsxwriter/src/xlsx
