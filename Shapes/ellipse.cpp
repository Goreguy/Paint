#include "ellipse.h"

Ellipse::Ellipse(const QRect& r)
    : BaseShape(ShapeType::Ellipse, r)
{}

void Ellipse::draw(QPainter* painter)
{
    painter->drawEllipse(rectangle);
}

bool Ellipse::contains(const QPoint& point) const
{
    return rectangle.contains(point);
}

void Ellipse::resize(const QRect& newBounds)
{
    rectangle = newBounds;
}

void Ellipse::moveShape(const QPoint& delta)
{
    rectangle.translate(delta);
}

QPoint Ellipse::center() const
{
    return rectangle.center();
}

QRect Ellipse::boundingRect() const
{
    return rectangle;
}

void Ellipse::serialize(QDataStream &out) const
{
    out << shape_type;
    out << shapeId;
    out << rectangle;
}

std::unique_ptr<BaseShape> Ellipse::deserialize(QDataStream &in)
{
    int shapeId;
    QRect r;
    in >> shapeId;
    in >> r;
    auto shape = std::make_unique<Ellipse>(r);
    shape->setId(shapeId);
    return shape;
}



