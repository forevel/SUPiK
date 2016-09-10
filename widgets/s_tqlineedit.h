#ifndef S_TQLINEEDIT_H
#define S_TQLINEEDIT_H

#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

class s_tqLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit s_tqLineEdit(QWidget *parent = 0);
    s_tqLineEdit(QString txt, QWidget *parent = 0);
    void setData (QVariant);
    void setAData(QVariant adata);
    QVariant getData();
    QVariant getAData();

signals:
//    void textChanged(const QString &, s_tqLineEdit *ptr);
    void editingFinished(const QString &);

public slots:

private:
    QVariant data, adata;

private slots:
    void changetext(const QString &);
    void emiteditingfinished();

protected:
//    QSize sizeHint() const;

};

#endif // S_TQLINEEDIT_H
