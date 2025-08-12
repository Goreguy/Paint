#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "BaseShape.h"

class Rectangle: public BaseShape
{
public:
    Rectangle(const QRect& r);
    ~Rectangle() = default;

    void draw(QPainter* painter) override;
    bool contains(const QPoint& point) const override;
    void resize(const QRect& newBounds) override;
    void move(const QPoint& delta) override;
    QPoint center() const override;
    QRect boundingRect() const override;

    QJsonObject serialize() const override;


};

#endif // RECTANGLE_H
