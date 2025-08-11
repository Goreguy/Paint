#include "triangle.h"

Triangle::Triangle(const QPolygon& p)
    : BaseShape(ShapeType::Triangle), polygon{p}
{}

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
    // Убедимся, что полигон имеет три точки
    if (polygon.size() < 3) {
        polygon = QPolygon(3); // Создаем полигон с тремя точками
    }

    QPoint center = newBounds.center();
    int width = newBounds.width();
    int height = newBounds.height();

    // Обновляем вершины
    polygon[0] = QPoint(center.x(), center.y() - height / 2);
    polygon[1] = QPoint(center.x() - width / 2, center.y() + height / 2);
    polygon[2] = QPoint(center.x() + width / 2, center.y() + height / 2);
}


void Triangle::move(const QPoint& delta)
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

QJsonObject Triangle::serialize() const
{
    QJsonObject obj;
    obj["type"] = "Triangle";

    // Заполняем координаты для вершин треугольника
    obj["x1"] = polygon[0].x();
    obj["y1"] = polygon[0].y();
    obj["x2"] = polygon[1].x();
    obj["y2"] = polygon[1].y();
    obj["x3"] = polygon[2].x();
    obj["y3"] = polygon[2].y();

    return obj;
}
