#include "pdfout.h"

#include <QTextTable>
#include <QImage>
#include <QTextImageFormat>
#include <QUrl>

PdfOut::PdfOut(QString &filename)
{
    PdfPrinter.setOutputFormat(QPrinter::PdfFormat);
    PdfPrinter.setPaperSize(QPrinter::A4);
    PdfPrinter.setOutputFileName(filename);
    //    PdfPrinter.setPageSize(PdfOut.pageRect().size()); // This is necessary if you want to hide the page number
    PdfDoc.clear();
    QTextCursor curs(&PdfDoc);
    PdfDocCursor = curs;
}

void PdfOut::SetTextFont(QFont font)
{
    QTextCharFormat fmt;
    fmt.setFont(font);
    PdfDocCursor.setCharFormat(fmt);
}

void PdfOut::InsertText(const QString &text)
{
    PdfDocCursor.insertText(text);
}

void PdfOut::InsertImage(const QString &img)
{
    QImage image;
    if (!image.load(img))
        return;
    PdfDoc.addResource(QTextDocument::ImageResource, QUrl("image"), image);
    QTextImageFormat ImageFormat;
    ImageFormat.setWidth(image.width() );
    ImageFormat.setHeight(image.height() );
    ImageFormat.setName("image");
    PdfDocCursor.insertImage(ImageFormat);
}

// количество рядов - по размеру lsl
// количество столбцов - по размеру первого элемента lsl

void PdfOut::InsertTable(const QList<QStringList> &lsl)
{
    if (lsl.size() == 0)
        return;
    int rowcount = lsl.size();
    if (lsl.at(0).size() == 0)
        return;
    int columncount = lsl.at(0).size();
    QTextTable *table = PdfDocCursor.insertTable(rowcount, columncount);
    for (int i=0; i<rowcount; ++i)
    {
        for (int j=0; j<lsl.at(i).size(); ++j)
            table->cellAt(i, j).firstCursorPosition().insertText(lsl.at(i).at(j));
    }
}

void PdfOut::WritePdf()
{
    PdfDoc.print(&PdfPrinter);
}
