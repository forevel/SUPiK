#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

/*
    Copyright (c) 2009-10 Qtrac Ltd. All rights reserved.

    This module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version. It is provided
    for educational purposes and is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
    the GNU General Public License for more details.
*/

#if QT_VERSION >= 0x040600
#include <QScopedPointer>
#else
#include <QSharedPointer>
#endif

#include <QMessageBox>

namespace MessageBox
{

void information(QWidget *parent, const QString &title,
                 const QString &text, const QString &detailedText=QString());

bool question(QWidget *parent, const QString &title,
              const QString &text, const QString &detailedText=QString(),
              const QString &yesText=QObject::tr("&Ага"),
              const QString &noText=QObject::tr("&Неа"));
}

#endif // MESSAGEBOX_H
