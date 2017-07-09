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
#include "publicclass.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqpushbutton.h"

#ifndef QT_NO_SSL
static const char defaultUrl[] = "https://www.qt.io/";
#else
static const char defaultUrl[] = "http://www.qt.io/";
#endif
static const char defaultFileName[] = "index.html";

Http::Http(QObject *parent) : QObject(parent)
{
    connect(&qnam, &QNetworkAccessManager::authenticationRequired, this, &Http::slotAuthenticationRequired);
#ifndef QT_NO_SSL
    connect(&qnam, &QNetworkAccessManager::sslErrors, this, &Http::sslErrors);
#endif
}

void Http::startRequest(const QUrl &requestedUrl)
{
    url = requestedUrl;
    httpRequestAborted = false;
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::downloadProgress, this, &Http::DownloadProgress);
    connect(reply, &QNetworkReply::finished, this, &Http::httpFinished);
    connect(reply, &QIODevice::readyRead, this, &Http::httpReadyRead);
}

void Http::downloadFile(const QString &httpurl, const QString &filename)
{
    QString fname = filename;
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
    if (fname.isEmpty())
        fname = newUrl.fileName();
    if (QFile::exists(fname))
        QFile::remove(fname);
    file = openFileForWrite(fname);
    if (!file)
        return;
    startRequest(newUrl);
}

QFile *Http::openFileForWrite(const QString &fileName)
{
    QScopedPointer<QFile> file(new QFile(fileName));
    if (!file->open(QIODevice::WriteOnly))
    {
        WARNMSG("Ошибка открытия файла "+fileName+" для записи");
        return Q_NULLPTR;
    }
    return file.take();
}

void Http::cancelDownload()
{
    httpRequestAborted = true;
    reply->abort();
}

void Http::DownloadProgress(qint64 bytesreceived, qint64 bytesoverall)
{
    emit CurrentFileDownloaded(url.toString());
    emit BytesOverall(bytesoverall);
    emit BytesRead(bytesreceived);
}

void Http::httpFinished()
{
    QFileInfo fi;
    if (file)
    {
        fi.setFile(file->fileName());
        file->close();
        delete file;
        file = Q_NULLPTR;
    }

    if (httpRequestAborted)
    {
        reply->deleteLater();
        reply = Q_NULLPTR;
        emit Finished();
        return;
    }

    if (reply->error())
    {
        QFile::remove(fi.absoluteFilePath());
        WARNMSG("Download failed: " + reply->errorString());
        reply->deleteLater();
        reply = Q_NULLPTR;
        emit Finished();
        return;
    }

    const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    reply->deleteLater();
    reply = Q_NULLPTR;

    if (!redirectionTarget.isNull()) {
        const QUrl redirectedUrl = url.resolved(redirectionTarget.toUrl());
        file = openFileForWrite(fi.absoluteFilePath());
        if (!file)
        {
            emit Finished();
            return;
        }
        startRequest(redirectedUrl);
        emit Finished();
        return;
    }
    emit Finished();
}

void Http::httpReadyRead()
{
      // this slot gets called every time the QNetworkReply has new data.
      // We read all of its new data and write it into the file.
      // That way we use less RAM than when reading it at the finished()
      // signal of the QNetworkReply
    if (file)
        file->write(reply->readAll());
}

void Http::slotAuthenticationRequired(QNetworkReply*, QAuthenticator *authenticator)
{
    QDialog *dlg = new QDialog;
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout *vlyout = new QVBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Введите данные для доступа к " + url.host());
    vlyout->addWidget(lbl, Qt::AlignHCenter);
    // Did the URL have information? Fill the UI
    // This is only relevant if the URL-supplied credentials were wrong
    lbl = new s_tqLabel("Имя:");
    vlyout->addWidget(lbl, Qt::AlignLeft);
    s_tqLineEdit *namele = new s_tqLineEdit(url.userName());
    vlyout->addWidget(namele, Qt::AlignLeft);
    lbl = new s_tqLabel("Пароль:");
    vlyout->addWidget(lbl, Qt::AlignLeft);
    s_tqLineEdit *pswle = new s_tqLineEdit(url.password());
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
        cancelDownload();
    dlg->close();
 }

#ifndef QT_NO_SSL
void Http::sslErrors(QNetworkReply*,const QList<QSslError> &errors)
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
