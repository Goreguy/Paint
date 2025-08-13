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

void Rectangle::moveShape(const QPoint& delta)
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

void Rectangle::serialize(QDataStream &out) const
{
    out << shapeId;
    out << rectangle;
}

std::unique_ptr<BaseShape> Rectangle::deserialize(QDataStream &in)
{
    int shapeId;
    QRect r;
    in >> shapeId;
    in >> r;
    auto shape = std::make_unique<Rectangle>(r);
    shape->setId(shapeId);
    return shape;
}
