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

#ifndef LOG_H
#define LOG_H

#include <QObject>

#define LOGER(a)   ERMSG(PublicClass::ER_LOG,__LINE__,a)
#define LOGWARN    WARNMSG(PublicClass::ER_LOG,__LINE__)
#define LOGINFO(a) INFOMSG(PublicClass::ER_LOG,__LINE__,a)

class Log : public QObject
{
    Q_OBJECT

public:
    explicit Log(QObject *parent=0);
    ~Log();
    void Init(QString Filename);
    void error(QString str);
    void warning(QString str);
    void info(QString str);
    void intvarvalue(QString var, int value);
    void WriteFile(QString Prepend, QString msg);
    
private:
    QString LogFile;
    bool CanLog;
};

#endif // LOG_H
