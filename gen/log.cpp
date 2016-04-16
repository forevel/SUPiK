/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <QFile>
#include <QDateTime>

#include "log.h"
#include "publicclass.h"

Log::Log(QObject *parent) : QObject(parent)
{
}

Log::~Log()
{
}

void Log::Init(QString Filename)
{
    LogFile = Filename;
    // тестовая проверка открытия файла на запись
    QFile *fp = new QFile(LogFile);
    if (!fp->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        LOGER("Невозможно создать log-файл: "+LogFile);
        CanLog = false;
        return;
    }
    CanLog = true;
    fp->close();
}

void Log::error(QString str)
{
    if (CanLog)
        WriteFile("Error", str);
}

void Log::info(QString str)
{
    if (CanLog)
        WriteFile("Info", str);
}

void Log::warning(QString str)
{
    if (CanLog)
        WriteFile("Warning",str);
}

void Log::intvarvalue(QString var, int value)
{
    if (CanLog)
        WriteFile(var, QString::number(value));
}

void Log::WriteFile(QString Prepend, QString msg)
{
    QFile *fp = new QFile(LogFile);
    if (!fp->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        return;
    QString tmps = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
    fp->write(tmps.toLocal8Bit());
    tmps = " "+Prepend+": ";
    fp->write(tmps.toLocal8Bit());
    fp->write(msg.toLocal8Bit());
    fp->write("\n");
    fp->close();
}
