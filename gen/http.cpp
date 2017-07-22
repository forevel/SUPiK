/****************************************************************************
  **
  ** Copyright (C) 2016 The Qt Company Ltd.
  ** Contact: https://www.qt.io/licensing/
  **
  ** This file is part of the examples of the Qt Toolkit.
  **
  ** $QT_BEGIN_LICENSE:BSD$
  ** Commercial License Usage
  ** Licensees holding valid commercial Qt licenses may use this file in
  ** accordance with the commercial license agreement provided with the
  ** Software or, alternatively, in accordance with the terms contained in
  ** a written agreement between you and The Qt Company. For licensing terms
  ** and conditions see https://www.qt.io/terms-conditions. For further
  ** information use the contact form at https://www.qt.io/contact-us.
  **
  ** BSD License Usage
  ** Alternatively, you may use this file under the terms of the BSD license
  ** as follows:
  **
  ** "Redistribution and use in source and binary forms, with or without
  ** modification, are permitted provided that the following conditions are
  ** met:
  **   * Redistributions of source code must retain the above copyright
  **     notice, this list of conditions and the following disclaimer.
  **   * Redistributions in binary form must reproduce the above copyright
  **     notice, this list of conditions and the following disclaimer in
  **     the documentation and/or other materials provided with the
  **     distribution.
  **   * Neither the name of The Qt Company Ltd nor the names of its
  **     contributors may be used to endorse or promote products derived
  **     from this software without specific prior written permission.
  **
  **
  ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
  **
  ** $QT_END_LICENSE$
  **
  ****************************************************************************/

#include <QtWidgets>
#include <QtNetwork>
#include <QUrl>

#include "http.h"
#include "files.h"
#include "publicclass.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqpushbutton.h"

static const char defaultFileName[] = "index.html";

Http::Http(QObject *parent) : QObject(parent)
{
    connect(&NAM, &QNetworkAccessManager::authenticationRequired, this, &Http::AuthenticationRequired);
#ifndef QT_NO_SSL
    connect(&NAM, &QNetworkAccessManager::sslErrors, this, &Http::SSLErrors);
#endif
}

void Http::DownloadFile(int thrnum, const QString &httpurl)
{
    ThrNum = thrnum;
    const QString urlSpec = httpurl.trimmed(); // removes whitespaces
    if (urlSpec.isEmpty())
    {
        WARNMSG("Url is empty");
        return;
    }

    const QUrl newUrl = QUrl::fromUserInput(urlSpec);
    if (!newUrl.isValid())
    {
        WARNMSG("Invalid url");
        return;
    }
    FileName = pc.HomeDir + "/tmp/";
    for (int i=0; i<FILENAMEMAXSYMBOLS; ++i)
    {
        QChar chr = (qrand() % 0x19) + 0x61; // from 0x61 to 0x7A
        FileName += chr;
    }
    Files file;
    Fp = file.openFileForWrite(FileName);
    if (!Fp)
    {
        emit Error(ThrNum, FILEERROR);
        return;
    }
    StartRequest(newUrl);
}

void Http::StartRequest(const QUrl &requestedUrl)
{
    Url = requestedUrl;
    HttpRequestAborted = false;
    Reply = NAM.get(QNetworkRequest(Url));
    connect(Reply, &QNetworkReply::downloadProgress, this, &Http::DownloadProgress);
    connect(Reply, &QNetworkReply::finished, this, &Http::HttpFinished);
    connect(Reply, &QIODevice::readyRead, this, &Http::HttpReadyRead);
}

void Http::CancelDownload()
{
    HttpRequestAborted = true;
    Reply->abort();
    emit Error(ThrNum, ABORTED);
}

void Http::DownloadProgress(qint64 bytesreceived, qint64 bytesoverall)
{
    emit CurrentFileDownloaded(Url.toString());
    emit BytesOverall(bytesoverall);
    emit BytesRead(bytesreceived);
}

void Http::HttpFinished()
{
    QFileInfo fi;
    if (Fp)
    {
        fi.setFile(Fp->fileName());
        Fp->close();
        delete Fp;
        Fp = Q_NULLPTR;
    }

    if (HttpRequestAborted)
    {
        Reply->deleteLater();
        Reply = Q_NULLPTR;
        emit Error(ThrNum, ABORTED);
        return;
    }

    if (Reply->error())
    {
        QFile::remove(fi.absoluteFilePath());
        WARNMSG("Download failed: " + Reply->errorString());
        Reply->deleteLater();
        Reply = Q_NULLPTR;
        emit Error(ThrNum, FAILED);
        return;
    }

    const QVariant redirectionTarget = Reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    Reply->deleteLater();
    Reply = Q_NULLPTR;

    if (!redirectionTarget.isNull()) {
        const QUrl redirectedUrl = Url.resolved(redirectionTarget.toUrl());
        Files file;
        Fp = file.openFileForWrite(FileName);
        if (!Fp)
        {
            emit Error(ThrNum, FILEERROR);
            return;
        }
        StartRequest(redirectedUrl);
//        emit Finished();
        return;
    }
    emit Finished(ThrNum, FileName);
}

void Http::HttpReadyRead()
{
      // this slot gets called every time the QNetworkReply has new data.
      // We read all of its new data and write it into the file.
      // That way we use less RAM than when reading it at the finished()
      // signal of the QNetworkReply
    if (Fp)
        Fp->write(Reply->readAll());
}

void Http::AuthenticationRequired(QNetworkReply*, QAuthenticator *authenticator)
{
    QDialog *dlg = new QDialog;
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout *vlyout = new QVBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Введите данные для доступа к " + Url.host());
    vlyout->addWidget(lbl, Qt::AlignHCenter);
    // Did the URL have information? Fill the UI
    // This is only relevant if the URL-supplied credentials were wrong
    lbl = new s_tqLabel("Имя:");
    vlyout->addWidget(lbl, Qt::AlignLeft);
    s_tqLineEdit *namele = new s_tqLineEdit(Url.userName());
    vlyout->addWidget(namele, Qt::AlignLeft);
    lbl = new s_tqLabel("Пароль:");
    vlyout->addWidget(lbl, Qt::AlignLeft);
    s_tqLineEdit *pswle = new s_tqLineEdit(Url.password());
    vlyout->addWidget(pswle, Qt::AlignLeft);
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton("Ага");
    connect(pb,SIGNAL(clicked(bool)),dlg,SLOT(accept()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton("Неа");
    connect(pb,SIGNAL(clicked(bool)),dlg,SLOT(reject()));
    hlyout->addWidget(pb);
    vlyout->addLayout(hlyout);
    dlg->setLayout(vlyout);
    if (dlg->exec() == QDialog::Accepted)
    {
        authenticator->setUser(namele->text());
        authenticator->setPassword(pswle->text());
    }
    else
        CancelDownload();
    dlg->close();
 }

#ifndef QT_NO_SSL
void Http::SSLErrors(QNetworkReply*,const QList<QSslError> &errors)
{
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += '\n';
        errorString += error.errorString();
    }
    WARNMSG("One or more SSL errors has occurred: " + errorString);
}
#endif
