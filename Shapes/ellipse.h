#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "BaseShape.h"

class Ellipse: public BaseShape
{
public:
    Ellipse(const QRect& r);
    ~Ellipse() = default;

    void draw(QPainter* painter) override;
    bool contains(const QPoint& point) const override;
    void resize(const QRect& newBounds) override;
    void move(const QPoint& delta) override;
    QPoint center() const override;
    QRect boundingRect() const override;

    QJsonObject serialize() const override;


};

#endif // ELLIPSE_H
