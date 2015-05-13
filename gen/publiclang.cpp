#include "publiclang.h"

#include <QTextStream>
#include <QFile>
#include <QSettings>
#include <QDir>

publiclang pl;

publiclang::publiclang()
{
}

bool publiclang::InitLang()
{
    QFile file;
    QString tmpString;
//    int i;

    pc.CurLang = pc.LandP->value("settings/lang","RU").toString();

/*    tmpString = QDir::currentPath();
    file.setFileName("lang\\lang." + pc.CurLang);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream streamfile(&file);
    streamfile.setCodec("WIN-1251");
    tmpString = streamfile.readLine();
    i = 0;
    while ((tmpString = streamfile.readLine()) != "=WINDOWTITLES=")
        SplashMessages[i++] = tmpString;

    i = 0;
    while ((tmpString = streamfile.readLine()) != "=DIALOGS=")
        WindowTitlesMessages[i++] = tmpString;

    i = 0;
    while ((tmpString = streamfile.readLine()) != "=COMPLABELS=")
    {
        if (tmpString == "---Общие---")
            i = 200;
        if (tmpString.left(3) != "---")
            DialogMessages[i++] = tmpString;
    }
*/
    return true;
}

void publiclang::SetDefaultLang()
{
    // заполнить все массивы из файла по-умолчанию (English?)
}
