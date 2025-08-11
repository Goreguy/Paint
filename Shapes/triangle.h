#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "BaseShape.h"

class Triangle: public BaseShape
{
public:
    Triangle(const QPolygon& p);
    ~Triangle() = default;

    void draw(QPainter* painter) override;
    bool contains(const QPoint& point) const override;
    void resize(const QRect& newBounds) override;
    void move(const QPoint& delta) override;
    QPoint center() const override;
    QRect boundingRect() const override;

    QJsonObject serialize() const override;

private:
    QPolygon polygon;

};

#endif // TRIANGLE_H
