#ifndef PUBLICLANG_H
#define PUBLICLANG_H

#include "publicclass.h"

class publiclang
{
public:
    publiclang();

    bool InitLang();
    void SetDefaultLang();

    QString SplashMessages[5]; // сообщения на заставке
    QString WindowTitlesMessages[20]; // Заголовки окон
    QString DialogMessages[255]; // информационные и аварийные сообщения
    QString CompLabels[40]; // названия и заголовки в диалоге компонентов
};

extern publiclang pl;

#endif // PUBLICLANG_H
