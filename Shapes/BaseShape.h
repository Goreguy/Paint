#ifndef BASESHAPE_H
#define BASESHAPE_H

#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QPolygon>
#include <QString>
#include <QDataStream>

enum class ShapeType
{
    Rectangle,
    Triangle,
    Ellipse

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
    virtual void moveShape(const QPoint& delta) = 0;
    virtual QPoint center() const = 0;
    virtual QRect boundingRect() const = 0;

    void addConnection(Connection *c);
    void removeConnection(Connection *c);

    quint32 getId() const { return shapeId;}
    void setId(quint32 id){shapeId = id;}

    virtual void serialize(QDataStream &out) const = 0;
    static std::unique_ptr<BaseShape> deserialize(QDataStream &in);

protected:
    quint32 shapeId;
    std::vector<Connection*> connections;
    ShapeType shape_type;
    QRect rectangle;


private:
    static quint32 countAndSetId();


};

#endif // BASESHAPE_H
