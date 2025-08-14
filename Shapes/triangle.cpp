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
    if (polygon.size() < 3) polygon = QPolygon(3);

    QPoint center = newBounds.center();
    int width = newBounds.width();
    int height = newBounds.height();

    polygon[0] = QPoint(center.x(), center.y() - height / 2);
    polygon[1] = QPoint(center.x() - width / 2, center.y() + height / 2);
    polygon[2] = QPoint(center.x() + width / 2, center.y() + height / 2);

    rectangle = newBounds;
}

void Triangle::moveShape(const QPoint& delta)
{
    polygon.translate(delta);
}

QPoint Triangle::center() const
{
    return polygon.boundingRect().center();
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
