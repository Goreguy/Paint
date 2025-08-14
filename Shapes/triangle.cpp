#include "triangle.h"

Triangle::Triangle(const QRect& r)
    : BaseShape(ShapeType::Triangle, r)
{

}

void Triangle::draw(QPainter* painter)
{
    painter->drawPolygon(polygon);
}

bool Triangle::contains(const QPoint& point) const
{
    return polygon.containsPoint(point, Qt::OddEvenFill);
}

void Triangle::resize(const QRect& newBounds)
{
    polygon.clear();

    QPoint top(newBounds.left() + newBounds.width()/2, newBounds.top());
    QPoint left(newBounds.left(), newBounds.bottom());
    QPoint right(newBounds.right(), newBounds.bottom());

    polygon << top << right << left;

    rectangle = newBounds;
}

QPoint Triangle::center() const
{
    return polygon.boundingRect().center();
}

void Triangle::moveShape(const QPoint& delta)
{
    polygon.translate(delta);
}

QRect Triangle::boundingRect() const
{
    return polygon.boundingRect();
}

void Triangle::serialize(QDataStream &out) const
{
    out << shape_type;
    out << getId();
    out << rectangle;
}

std::unique_ptr<BaseShape> Triangle::deserialize(QDataStream &in)
{
    quint32 shapeId;
    QRect r;
    in >> shapeId >> r;
    auto shape = std::make_unique<Triangle>(r);
    shape->Triangle::resize(r);
    shape->setId(shapeId);
    return shape;
}
