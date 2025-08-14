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

QPoint Rectangle::center() const
{
    return rectangle.center();
}

void Rectangle::moveShape(const QPoint& delta)
{
    rectangle.translate(delta);
}

QRect Rectangle::boundingRect() const
{
    return rectangle;
}

void Rectangle::serialize(QDataStream &out) const
{
    out << shape_type;
    out << getId();
    out << rectangle;
}

std::unique_ptr<BaseShape> Rectangle::deserialize(QDataStream &in)
{
    quint32 shapeId;
    QRect r;
    in >> shapeId >> r;
    auto shape = std::make_unique<Rectangle>(r);
    shape->setId(shapeId);
    return shape;
}
