#ifndef PIXMAPOBJECT_H
#define PIXMAPOBJECT_H

#include <QObject>
#include "MapGraphicsObject.h"
#include "MapGraphics_global.h"
class MAPGRAPHICSSHARED_EXPORT PixmapObject : public MapGraphicsObject
{
    Q_OBJECT
public:
    explicit PixmapObject(const QString& filePath ,bool sizeIsZoomInvariant=true,
                          QSize size=QSize(0,0),MapGraphicsObject *parent = 0);
    explicit PixmapObject(const QPixmap pix,bool sizeIsZoomInvariant=true,
                          QSize size=QSize(0,0),MapGraphicsObject *parent = 0);
    virtual ~PixmapObject();

    //pure-virtual from MapGraphicsObject
    QRectF boundingRect() const;

    //pure-virtual from MapGraphicsObject
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    //virtual from MapGraphicsObject
    virtual void keyReleaseEvent(QKeyEvent *event);
signals:
private:
    QPixmap _pixmap;
};

#endif // PIXMAPOBJECT_H
