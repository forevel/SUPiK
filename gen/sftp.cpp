#include <QApplication>

#include "sftp.h"


s_ftp sftp;

s_ftp::s_ftp(QObject *parent) : QObject(parent)
{
    Busy = false;
    ReadData.resize(65535);
}

bool s_ftp::IsFtpAvailable()
{
    Busy = true;
    QString CheckUrl = QString(FTP_SERVER)+"/xmHXP_FW~h";
    GetFile(CheckUrl);
    while (Busy)
        qApp->processEvents(QEventLoop::AllEvents);
    if (QString(ReadData.data()) == "supik\n")
        return true;
    return false;
}

void s_ftp::GetFile(QString Url)
{
    ReadData.clear();
    QNetworkAccessManager *nam = new QNetworkAccessManager();
    QUrl url2(Url);
    url2.setPassword("wdbpy(WcTtTZzA_TEc-<");
    url2.setUserName("supik");
    QNetworkRequest req(url2);
    QNetworkReply *reply = nam->get(req);
    connect(reply, SIGNAL(readyRead()), this, SLOT(ReadDataFromUrl()));
    connect(reply,SIGNAL(finished()),this,SLOT(FinishRead()));
    connect(this,SIGNAL(ReadFinished()),reply,SLOT(deleteLater()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(SetError(QNetworkReply::NetworkError)));
}

void s_ftp::SetError(QNetworkReply::NetworkError err)
{
    QStringList sl = QStringList() << "FTP: Нет ошибок" << "FTP: Удалённый сервер отказал в соединении" << \
                                      "FTP: Удалённый сервер неожиданно закрыл соединение" << "FTP: Сервер с таким адресом не найден" << \
                                      "FTP: Превышено время ожидания ответа" << "FTP: Операция была отменена" << "FTP: ошибка SSL" << \
                                      "FTP: Временные неполадки в связи" << "FTP: Соединение не удалось" << "FTP: Соединение не разрешено сервером" << \
                                      "FTP: Прочие неполадки в сети" << "" << "FTP: Отказано в соединении с прокси" << "FTP: Прокси-сервер закрыл соединение" << \
                                      "FTP: Не найден прокси-сервер" << "FTP: Превышено время ожидания связи с прокси-сервером" << \
                                      "FTP: Прокси-сервер требует аутентификации" << "FTP: Прочие проблемы с прокси" << \
                                      "" << "FTP: Отказ в доступе" << "FTP: Операция не разрешена" << "FTP: Файл не найден на удалённом сервере" << \
                                      "FTP: Требуется аутентификация" << "FTP: Ошибка при передаче" << "FTP: Конфликт состояния" << \
                                      "FTP: Требуемого файла больше нет на удалённом сервере" << "FTP: Прочие ошибки удалённого доступа" << \
                                      "" << "FTP: Неизвестный протокол доступа" << "FTP: Некорректная операция для выбранного протокола" << \
                                      "FTP: Ошибка протокола" << "" << "FTP: Удалённый сервер вышел в неожиданное состояние" << \
                                      "FTP: Удалённый сервер не имеет должного функционала для обработки запроса" << "FTP: В данное время невозможно обработать запрос" << \
                                      "FTP: Неизвестная ошибка ответа сервера";
    int ErrNum = static_cast<int>(err);
    if (ErrNum > 9) // 99>
        ErrNum -= 89; // с 99 по 105 обрабатываем в диапазоне 10-16
    if (ErrNum > 16) // 199>
        ErrNum -= 93; // с 199 по 207 обрабатываем в диапазоне 17-25
    if (ErrNum > 25) // 299>
        ErrNum -= 91; // с 299 по 302 обрабатываем в диапазоне 26-29
    if (ErrNum > 29) // 399>
        ErrNum -= 96; // с 399 по 403 обрабатываем в диапазоне 30-34
    if (ErrNum > 34) // 499
        ErrNum = 35;
    SFTPER(sl.at(ErrNum));
    Busy = false;
}

void s_ftp::ReadDataFromUrl()
{
    qint64 RDSize = ReadData.size();
    if (RDSize > 4000000L)
    {
        emit ReadBufferFull();
        return;
    }
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    QByteArray ba = reply->read(65535);
    ReadData.append(ba);
    emit NewDataAvailable();
}

void s_ftp::FinishRead()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    QByteArray ba = reply->readAll();
    ReadData.append(ba);
    emit ReadFinished();
    Busy = false;
}
