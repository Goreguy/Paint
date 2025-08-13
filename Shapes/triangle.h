#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "BaseShape.h"

class Triangle: public BaseShape
{
public:
    Triangle(const QRect& r);
    ~Triangle() = default;

    void draw(QPainter* painter) override;
    bool contains(const QPoint& point) const override;
    void resize(const QRect& newBounds) override;
    void moveShape(const QPoint& delta) override;
    QPoint center() const override;
    QRect boundingRect() const override;

    void serialize(QDataStream &out) const override;
    static std::unique_ptr<BaseShape> deserialize(QDataStream &in);

private:
    QPolygon polygon;

};

#endif // TRIANGLE_H
