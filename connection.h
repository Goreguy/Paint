#ifndef CONNECTION_H
#define CONNECTION_H

#include <QPoint>
#include <QPainter>
#include <QDataStream>

#include "Shapes/BaseShape.h"

class Connection
{
public:
    Connection(BaseShape* a, BaseShape* b) : a(a), b(b) {}

    void draw(QPainter &p) const;

    BaseShape* getFirstShape() const { return a; }
    BaseShape* getSecondShape() const { return b; }

    void translate(const QPoint &delta)
    {
        // connections don't have their own coords; they follow connected shapes
        Q_UNUSED(delta);
    }

    void serialize(QDataStream &out, const std::vector<BaseShape*> &shapes) const;

    // static std::unique_ptr<Connection> deserialize(QDataStream &in, const std::unordered_map<int, BaseShape*>& shapeMap);


private:
    BaseShape *a;
    BaseShape *b;
};

#endif // CONNECTION_H
