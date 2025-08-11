#include "ellipse.h"

Ellipse::Ellipse(const QRect& r)
    : BaseShape(ShapeType::Ellipse), rectangle{r}
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
    rectangle = newBounds;;
}

void Ellipse::move(const QPoint& delta)
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

QJsonObject Ellipse::serialize() const
{
    QJsonObject obj;
    obj["type"] = "Ellipse";
    obj["x"] = rectangle.x();
    obj["y"] = rectangle.y();
    obj["width"] = rectangle.width();
    obj["height"] = rectangle.height();

    return obj;
}
