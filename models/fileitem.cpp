#include "fileitem.h"

#include <QVariant>

FileItem::FileItem(FileItem *parent)
{
    Q_UNUSED(parent);
}

FileItem::~FileItem()
{
}

QString FileItem::Name() const
{
    return name;
}

bool FileItem::SetName(const QString &Value)
{
    name = Value;
    return true;
}

QString FileItem::Size() const
{
    return QString::number(size, 10);
}

bool FileItem::SetSize(const QString &Value)
{
    bool ok;
    size = Value.toInt(&ok);
    if (!ok)
    {
        size = 0;
        return false;
    }
    return true;
}

QString FileItem::Attr() const
{
    // quint16 attr is the number of bits: drwxrwxrwx
    QString tmps;
    const QString templ = "drwxrwxrwx";
    for (int i=0; i<10; ++i)
    {
        if ((0x0001 << i) & attr)
            tmps += templ.at(i);
        else
            tmps += '-';
    }
    return tmps;
}

bool FileItem::SetAttr(const QString &Value)
{
    if (Value.size() < 10)
        return false;
    quint16 tmpi = 0;
    for (int i=0; i<10; ++i)
    {
        if (Value.at(i) != '-')
            tmpi |= (0x0001 << i);
    }
    return true;
}

QString FileItem::Datetime() const
{
    return datetime.toString("dd-MM-yyyy hh:mm:ss");
}

bool FileItem::SetDatetime(const QDateTime &Value)
{
    if (Value.isValid())
    {
        datetime = Value;
        return true;
    }
    return false;
}

void FileItem::SetColor(QColor Color)
{
    color = Color;
}

void FileItem::SetFont(QFont Font)
{
    font = Font;
}

void FileItem::SetIcon(QIcon Icon)
{
    icon = Icon;
}

QColor FileItem::Color()
{
    return color;
}

QFont FileItem::Font()
{
    return font;
}

QIcon FileItem::Icon()
{
    return icon;
}
