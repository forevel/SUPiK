#ifndef S_STATUSBAR_H
#define S_STATUSBAR_H

#include <QWidget>
#include <QTimer>

class s_StatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit s_StatusBar(QWidget *parent = 0);
    ~s_StatusBar();

    quint64 IncomeBytes();
    quint64 OutgoingBytes();

signals:

public slots:
    void UpdateIncomeBytes(quint64 bytes);
    void UpdateOutgoingBytes(quint64 bytes);

private slots:
/*    void UploadActive();
    void DownloadActive();
    void ClearUpload();
    void ClearDownload(); */

private:

    quint64 IncBytes, OutBytes;
//    QTimer *UTmr, *DTmr;

protected:
};

#endif // S_STATUSBAR_H
