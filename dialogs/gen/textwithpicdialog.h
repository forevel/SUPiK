#ifndef TEXTWITHPICDIALOG_H
#define TEXTWITHPICDIALOG_H

#include <QDialog>

class TextWithPicDialog : public QDialog
{
    Q_OBJECT
public:
    enum TETypes
    {
        TWPD_TEONLY,
        TWPD_PICONLY,
        TWPD_TEWITHPIC
    };

    explicit TextWithPicDialog(int TEType, QWidget *parent = 0);

    void SetTEField(const QString &tefield);
    void SetFileField(const QString &filename);
    void SetLink(QStringList &link);
signals:
    void ChangesHasBeenMade(QString);

public slots:

private:
    QStringList link;

    void GetDirStringFromLink(QStringList &link, QString &out);
private slots:
    void tepbclicked();
    void ChooseTEFile();
    void TEFileChoosed(QString filename);
//    void TEDirCheck(QString dir);

protected:
    void paintEvent(QPaintEvent *e);

};

#endif // TEXTWITHPICDIALOG_H
