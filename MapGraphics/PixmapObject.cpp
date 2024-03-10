#include "PixmapObject.h"
#include <QtDebug>
#include <QKeyEvent>
PixmapObject::PixmapObject(const QString &filePath,bool sizeIsZoomInvariant,QSize size,
                           MapGraphicsObject *parent ):
    MapGraphicsObject(sizeIsZoomInvariant,parent)
{
    _pixmap.load(filePath);
    if(_pixmap.isNull())
    {
      qDebug()<<filePath<<" is invalid";
    }
    if(!size.isNull())
    {
      _pixmap.scaled(size);
    }

}

PixmapObject::PixmapObject(const QPixmap pix, bool sizeIsZoomInvariant, QSize size, MapGraphicsObject *parent)
    : MapGraphicsObject(sizeIsZoomInvariant,parent)
{
    _pixmap=pix;
    if(_pixmap.isNull())
    {
      qDebug()<<"pixmap is invalid";
    }
    if(!size.isNull())
    {
      _pixmap.scaled(size);
    }
}

PixmapObject::~PixmapObject()
{

}

void PixmapObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->drawPixmap(-(_pixmap.width()/2),-(_pixmap.height()/2),_pixmap.width(),_pixmap.height(),_pixmap);
//    painter->setBrush(QColor(255, 0, 0, 100));
//    painter->drawEllipse(QPointF(0,0),
//                         5000,
//                         5000);
}
//protected
//virtual from MapGraphicsObject
void PixmapObject::keyReleaseEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Delete))
    {
        this->deleteLater();
        event->accept();
    }
    else
        event->ignore();
}
QRectF PixmapObject::boundingRect() const
{
    return QRectF(-(_pixmap.width()/2.0),-(_pixmap.height()/2.0),_pixmap.width(),_pixmap.height());
//    return QRectF(0,0,50000,50000);

}
