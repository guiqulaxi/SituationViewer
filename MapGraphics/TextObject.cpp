#include "TextObject.h"

#include "CircleObject.h"

#include <QtDebug>
#include <QKeyEvent>

TextObject::TextObject( const QString& text,
                        QColor color,
                        bool sizeIsZoomInvariant,
                        QFont font,
                        MapGraphicsObject *parent  ) :
    MapGraphicsObject(sizeIsZoomInvariant,parent)
{
    _font=font;
    _color=color;
    _text=text;
    //    this->setFlag(MapGraphicsObject::ObjectIsSelectable);
    //    this->setFlag(MapGraphicsObject::ObjectIsMovable);
    //    this->setFlag(MapGraphicsObject::ObjectIsFocusable);
}

TextObject::~TextObject()
{
}

QRectF TextObject::boundingRect() const
{
    QFontMetrics fm(_font);
    int w=fm.horizontalAdvance(_text);
    int h =fm.height();

    return QRect(-w/2,-h/2,w,h);
}

void TextObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setFont(_font);
    painter->setPen(_color);
    QFontMetrics fm(_font);
    int w=fm.horizontalAdvance(_text);
    int h =fm.height();
    //这里先让字体水平居中
    painter->drawText(QRect(-w/2,-h/2,w,h),_text);
}




//protected
//virtual from MapGraphicsObject
void TextObject::keyReleaseEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Delete))
    {
        this->deleteLater();
        event->accept();
    }
    else
        event->ignore();
}
