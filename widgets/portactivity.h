#ifndef PORTACTIVITY_H
#define PORTACTIVITY_H

#include <QWidget>
#include <QTimer>

class PortActivity : public QWidget
{
    Q_OBJECT
public:
    explicit PortActivity(QWidget *parent = 0);
    ~PortActivity();
    void Start();

signals:

public slots:

private slots:
    void UploadActive();
    void DownloadActive();
    void ClearUpload();
    void ClearDownload();

private:
    bool UploadIsActive, DownloadIsActive;
    QTimer *UTmr, *DTmr;

protected:
};

#endif // PORTACTIVITY_H
