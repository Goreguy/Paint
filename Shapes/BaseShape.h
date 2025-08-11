#ifndef BASESHAPE_H
#define BASESHAPE_H

#include "../States.h"
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QPolygon>
#include <QString>
#include <QJsonObject>

class Connection;

class BaseShape
{
public:
    BaseShape(ShapeType type);
    virtual ~BaseShape() = default;

    virtual void draw(QPainter* painter) = 0;
    virtual bool contains(const QPoint& point) const = 0;
    virtual void resize(const QRect& newBounds) = 0;
    virtual void move(const QPoint& delta) = 0;
    virtual QPoint center() const = 0;
    virtual QRect boundingRect() const = 0;

    virtual QJsonObject serialize() const = 0;
    static std::unique_ptr<BaseShape> deserialize(const QJsonObject& obj);//реализовать

protected:
    ShapeType shape_type;

};

#endif // BASESHAPE_H
