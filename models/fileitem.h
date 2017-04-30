#ifndef FILEITEM_H
#define FILEITEM_H

#include <QList>
#include <QColor>
#include <QFont>
#include <QIcon>
#include <QDateTime>

class FileItem
{
public:
    FileItem(FileItem *parent=0);
    ~FileItem();

    QString Name() const;
    QString Size() const;
    QString Attr() const;
    QString Datetime() const;
    bool SetName(const QString &Value);
    bool SetSize(const QString &Value);
    bool SetAttr(const QString &Value);
    bool SetDatetime(const QDateTime &Value);

    void SetColor(QColor Color);
    void SetFont(QFont Font);
    void SetIcon(QIcon Icon);
    QColor Color();
    QFont Font();
    QIcon Icon();

private:
    QString name;
    QDateTime datetime;
    quint64 size;
    quint16 attr;
    QColor color;
    QFont font;
    QIcon icon;
};
#endif // FILEITEM_H
