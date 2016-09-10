#include "s_tqlineedit.h"

#include <QStyleOptionFrameV2>
#include <QPointer>

s_tqLineEdit::s_tqLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    setStyleSheet("QLineEdit {background-color: rgba(0,0,0,0); border: 1px solid gray; border-radius: 5px;}");
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(changetext(QString)));
    connect(this,SIGNAL(editingFinished()),this,SLOT(emiteditingfinished()));
}

s_tqLineEdit::s_tqLineEdit(QString txt, QWidget *parent) : QLineEdit(parent)
{
    setText(txt);
    setStyleSheet("QLineEdit {background-color: rgba(0,0,0,0);}");
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(changetext(QString)));
}

void s_tqLineEdit::setData(QVariant dat)
{
    this->data = dat;
}

void s_tqLineEdit::setAData(QVariant adata)
{
    this->adata = adata;
}

QVariant s_tqLineEdit::getData()
{
    return this->data;
}

QVariant s_tqLineEdit::getAData()
{
    return this->adata;
}

void s_tqLineEdit::changetext(const QString &str)
{
    // resizing lineedit to contents
    QFontMetrics fm = this->fontMetrics();
    int w = fm.boundingRect(str).width();
    this->resize(w, this->height());
    this->updateGeometry();
}

void s_tqLineEdit::emiteditingfinished()
{
    emit editingFinished(this->text());
}

/*QSize s_tqLineEdit::sizeHint() const
{
    ensurePolished();
    QFontMetrics fm(font());
    int h = qMax(fm.height(), 14) + 2*1 // verticalmargin
            + 1 + 1 // top & bottomtextmargins
            + 1 + 1; // top & bottom margins
    int w = fm.width(QLatin1Char('x')) * this->text().size() + 2*2 // horizontalmargin
            + 1 + 1 // effective left & right margins (include icons)
            + 1 + 1; // left & right margins
    QStyleOptionFrameV2 opt;
    initStyleOption(&opt);
    return (style()->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(w, h)));
}*/

/*
QSize QLineEdit::minimumSizeHint() const
{
    Q_D(const QLineEdit);
    ensurePolished();
    QFontMetrics fm = fontMetrics();
    int h = fm.height() + qMax(2*d->verticalMargin, fm.leading())
            + d->topmargin + d->bottommargin;
    int w = fm.maxWidth() + d->leftmargin + d->rightmargin;
    QStyleOptionFrameV2 opt;
    initStyleOption(&opt);
    return (style()->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(w, h).
                                      expandedTo(QApplication::globalStrut()), this));
}
*/
