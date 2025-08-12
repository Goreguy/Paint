#include "rectangle.h"

Rectangle::Rectangle(const QRect& r)
    : BaseShape(ShapeType::Rectangle, r)
{}

void Rectangle::draw(QPainter* painter)
{
    painter->drawRect(rectangle);
}

bool Rectangle::contains(const QPoint& point) const
{
    return rectangle.contains(point);
}

void Rectangle::resize(const QRect& newBounds)
{
    rectangle = newBounds;
}

void Rectangle::move(const QPoint& delta)
{
    rectangle.translate(delta);
}

QPoint Rectangle::center() const
{
    return rectangle.center();
}

QRect Rectangle::boundingRect() const
{
    return rectangle;
}

QJsonObject Rectangle::serialize() const
{
    QJsonObject obj;
    obj["type"] = "Rectangle";
    obj["x"] = rectangle.x();
    obj["y"] = rectangle.y();
    obj["width"] = rectangle.width();
    obj["height"] = rectangle.height();

    return obj;
}
