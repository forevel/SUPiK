#-------------------------------------------------
#
# Project created by QtCreator 2013-10-03T09:48:29
#
#-------------------------------------------------

QMAKE_TARGET_COMPANY = EvelSoft
QMAKE_TARGET_COPYRIGHT = EvelSoft
QMAKE_TARGET_PRODUCT = SUPiK
RC_ICONS = res/SUPiK.ico
VERSION = 2.1.24

QT       += widgets core gui sql network printsupport

TARGET = SUPiK
TEMPLATE = app

SOURCES += main.cpp\
    startwindow.cpp \
    supik.cpp \
    dialogs/cmp/cmp_compdialog.cpp \
    dialogs/cmp/cmp_maindialog.cpp \
    dialogs/cmp/cmp_newsubsection.cpp \
    dialogs/dev/dev_devdialog.cpp \
    dialogs/dev/dev_docdialog.cpp \
    dialogs/dev/dev_maindialog.cpp \
    dialogs/dir/dir_adddialog.cpp \
    dialogs/dir/dir_maindialog.cpp \
    dialogs/gen/accessdialog.cpp \
    dialogs/gen/chooseitemdialog.cpp \
    dialogs/gen/messagebox.cpp \
    dialogs/gen/specialdialog.cpp \
    dialogs/gen/twocoldialog.cpp \
    dialogs/pers/persdialog.cpp \
    dialogs/sys/sys_backuprestoredirdialog.cpp \
    dialogs/sys/sys_erdialog.cpp \
    dialogs/sys/sys_importclass.cpp \
    dialogs/sys/sys_persdialog.cpp \
    dialogs/sys/sys_probsdialog.cpp \
    dialogs/sys/sys_settingsdialog.cpp \
    dialogs/sys/sys_systemdialog.cpp \
    dialogs/sys/sysdireditor.cpp \
    dialogs/sys/sysmenueditor.cpp \
    dialogs/tb/tb_examdialog.cpp \
    dialogs/tb/tb_func.cpp \
    dialogs/tb/tb_maindialog.cpp \
    dialogs/wh/wh_dialog.cpp \
    dialogs/wh/wh_editor.cpp \
    gen/client.cpp \
    gen/ftp.cpp \
    gen/log.cpp \
    gen/pdfout.cpp \
    gen/publicclass.cpp \
    gen/publiclang.cpp \
    gen/s_sql.cpp \
    gen/s_tablefields.cpp \
    gen/wh_func.cpp \
    models/baseitem.cpp \
    models/baseitemmodel.cpp \
    models/editmodel.cpp \
    models/errorprotocolmodel.cpp \
    models/griddelegate.cpp \
    models/proxymodel.cpp \
    models/s_duniversal.cpp \
    models/treeitem.cpp \
    models/treemodel.cpp \
    threads/ethernet.cpp \
    threads/sys_importclass_t.cpp \
    threads/waitthread.cpp \
    widgets/clevertimer.cpp \
    widgets/goodbadwidget.cpp \
    widgets/s_colortabbar.cpp \
    widgets/s_colortabwidget.cpp \
    widgets/s_maskedle.cpp \
    widgets/s_tqcalendarwidget.cpp \
    widgets/s_tqcheckbox.cpp \
    widgets/s_tqchoosewidget.cpp \
    widgets/s_tqcombobox.cpp \
    widgets/s_tqdatetimewidget.cpp \
    widgets/s_tqframe.cpp \
    widgets/s_tqgroupbox.cpp \
    widgets/s_tqlabel.cpp \
    widgets/s_tqlineedit.cpp \
    widgets/s_tqpushbutton.cpp \
    widgets/s_tqradiobutton.cpp \
    widgets/s_tqscrollarea.cpp \
    widgets/s_tqspinbox.cpp \
    widgets/s_tqsplitter.cpp \
    widgets/s_tqstackedwidget.cpp \
    widgets/s_tqtextedit.cpp \
    widgets/s_tqtoolbar.cpp \
    widgets/s_tqwidget.cpp \
    widgets/treeview.cpp \
    widgets/waitwidget.cpp \
    widgets/wd_func.cpp \
    widgets/s_statusbar.cpp

HEADERS  += startwindow.h \
    supik.h \
    dialogs/cmp/cmp_compdialog.h \
    dialogs/cmp/cmp_maindialog.h \
    dialogs/cmp/cmp_newsubsection.h \
    dialogs/dev/dev_devdialog.h \
    dialogs/dev/dev_docdialog.h \
    dialogs/dev/dev_maindialog.h \
    dialogs/dir/dir_adddialog.h \
    dialogs/dir/dir_maindialog.h \
    dialogs/gen/accessdialog.h \
    dialogs/gen/chooseitemdialog.h \
    dialogs/gen/messagebox.h \
    dialogs/gen/specialdialog.h \
    dialogs/gen/twocoldialog.h \
    dialogs/pers/persdialog.h \
    dialogs/sys/sys_backuprestoredirdialog.h \
    dialogs/sys/sys_erdialog.h \
    dialogs/sys/sys_importclass.h \
    dialogs/sys/sys_persdialog.h \
    dialogs/sys/sys_probsdialog.h \
    dialogs/sys/sys_settingsdialog.h \
    dialogs/sys/sys_systemdialog.h \
    dialogs/sys/sysdireditor.h \
    dialogs/sys/sysmenueditor.h \
    dialogs/tb/tb_examdialog.h \
    dialogs/tb/tb_func.h \
    dialogs/tb/tb_maindialog.h \
    dialogs/wh/wh_dialog.h \
    dialogs/wh/wh_editor.h \
    gen/client.h \
    gen/ftp.h \
    gen/log.h \
    gen/pdfout.h \
    gen/publicclass.h \
    gen/publiclang.h \
    gen/s_sql.h \
    gen/s_tablefields.h \
    gen/wh_func.h \
    models/baseitem.h \
    models/baseitemmodel.h \
    models/editmodel.h \
    models/errorprotocolmodel.h \
    models/griddelegate.h \
    models/proxymodel.h \
    models/s_duniversal.h \
    models/treeitem.h \
    models/treemodel.h \
    threads/ethernet.h \
    threads/sys_importclass_t.h \
    threads/waitthread.h \
    widgets/clevertimer.h \
    widgets/goodbadwidget.h \
    widgets/s_colortabbar.h \
    widgets/s_colortabwidget.h \
    widgets/s_maskedle.h \
    widgets/s_tqcalendarwidget.h \
    widgets/s_tqcheckbox.h \
    widgets/s_tqchoosewidget.h \
    widgets/s_tqcombobox.h \
    widgets/s_tqdatetimewidget.h \
    widgets/s_tqframe.h \
    widgets/s_tqgroupbox.h \
    widgets/s_tqlabel.h \
    widgets/s_tqlineedit.h \
    widgets/s_tqpushbutton.h \
    widgets/s_tqradiobutton.h \
    widgets/s_tqscrollarea.h \
    widgets/s_tqspinbox.h \
    widgets/s_tqsplitter.h \
    widgets/s_tqstackedwidget.h \
    widgets/s_tqtextedit.h \
    widgets/s_tqtoolbar.h \
    widgets/s_tqwidget.h \
    widgets/treeview.h \
    widgets/waitwidget.h \
    widgets/wd_func.h \
    widgets/s_statusbar.h

RESOURCES += \
    pic.qrc

CONFIG += embed_manifest_exe

win32:CONFIG(release, debug|release): LIBS += -LD:/mysql/lib/ -llibmysql -lqt5xlsx
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/mysql/lib/ -llibmysqld -lqt5xlsxd
else:unix: LIBS += -LD:/mysql/lib/ -llibmysql

INCLUDEPATH += D:/mysql/include
DEPENDPATH += D:/mysql/include

LIBS += -llibeay32 -lssleay32
LIBS += -lliblzma
