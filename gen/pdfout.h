#ifndef PDFOUT_H
#define PDFOUT_H

#include <QPrinter>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QString>
#include <QFont>
#include <QStringList>

class PdfOut
{
public:
    PdfOut(QString &filename);
    void SetTextFont(QFont font);
    void InsertText(const QString &text);
    void InsertImage(const QString &img);
    void InsertTable(const QList<QStringList> &lsl);
    void WritePdf();

private:
    QPrinter PdfPrinter;
    QTextDocument PdfDoc;
    QTextCursor PdfDocCursor;
};

#endif // PDFOUT_H
