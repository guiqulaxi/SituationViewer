#ifndef ENITYOBJECT_H
#define ENITYOBJECT_H

#include <QObject>
#include "MapGraphics_global.h"
#include "MapGraphicsObject.h"
#include <QString>
#include <QFont>
#include <QColor>
class MAPGRAPHICSSHARED_EXPORT TextObject : public MapGraphicsObject
{
    Q_OBJECT
public:

    explicit TextObject(const QString& text,
                        QColor color,
                        bool sizeIsZoomInvariant=true,
                        QFont font=QFont("微软雅黑",15,QFont::Bold)
            ,MapGraphicsObject *parent = 0);
    virtual ~TextObject();

    //pure-virtual from MapGraphicsObject
    QRectF boundingRect() const;

    //pure-virtual from MapGraphicsObject
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    //virtual from MapGraphicsObject
    virtual void keyReleaseEvent(QKeyEvent *event);
    QSize _size;
    QFont _font;
    QColor _color;
    QString _text;
signals:
};

#endif // ENITYOBJECT_H
