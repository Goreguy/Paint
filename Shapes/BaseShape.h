#ifndef BASESHAPE_H
#define BASESHAPE_H

#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QPolygon>
#include <QString>
#include <QJsonObject>

enum class ShapeType
{
    Rectangle,
    Triangle,
    Ellipse,
    Link

};

class Connection;

class BaseShape
{
public:
    BaseShape(ShapeType type, const QRect& r);
    virtual ~BaseShape() = default;

    virtual void draw(QPainter* painter) = 0;
    virtual bool contains(const QPoint& point) const = 0;
    virtual void resize(const QRect& newBounds) = 0;
    virtual void move(const QPoint& delta) = 0;
    virtual QPoint center() const = 0;
    virtual QRect boundingRect() const = 0;

    void addConnection(Connection *c) { connections.push_back(c); }

    virtual QJsonObject serialize() const = 0;
    static std::unique_ptr<BaseShape> deserialize(const QJsonObject& obj);//реализовать

protected:
    std::vector<Connection*> connections;
    ShapeType shape_type;
    QRect rectangle;

};

#endif // BASESHAPE_H
