QMAKE_TARGET_COMPANY = MyCompany
QMAKE_TARGET_COPYRIGHT = MyCompany
QMAKE_TARGET_PRODUCT = MyProg
RC_ICONS = MyProg.ico
VERSION = 1.2.3.4 #major.minor.patch.build

# если нужно выполнять от администратора
win32
{
    CONFIG += embed_manifest_exe
    QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:"level='requireAdministrator'"
}