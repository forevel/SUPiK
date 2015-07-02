#ifndef SYSMENUEDITOR_H
#define SYSMENUEDITOR_H

#include <QDialog>
#include <QModelIndex>
#include <QPoint>

class SysmenuEditor : public QDialog
{
    Q_OBJECT
public:
    SysmenuEditor(QWidget *parent = 0);

    void SetupUI(QString tble);

signals:
    void error(int);

private slots:
    void ChangeFields(QModelIndex);
    void ChangeFields();
    void Context(QPoint);
    void AddChild();
    void AddRoot();
    void ChangeName();
    void Delete();
    void AddToTree(QString);

private:
    QStringList AdditionalFields;
    QString tble, als;

    QString GetIndex(int);
    void UpdateTree();
};

#endif // SYSMENUEDITOR_H
