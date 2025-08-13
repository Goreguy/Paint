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
    void moveShape(const QPoint& delta) override;
    QPoint center() const override;
    QRect boundingRect() const override;

    void serialize(QDataStream &out) const override;
    static std::unique_ptr<BaseShape> deserialize(QDataStream &in);

};

#endif // RECTANGLE_H
