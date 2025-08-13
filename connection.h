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

    int getIdFirstShape() const { return a->getId(); }
    int getIdSecondShape() const { return b->getId(); }

    void translate(const QPoint &delta)
    {
        // connections don't have their own coords; they follow connected shapes
        Q_UNUSED(delta);
    }

    void serialize(QDataStream &out) const;

    static std::pair<quint32, quint32> deserialize(QDataStream &in);


private:
    BaseShape *a;
    BaseShape *b;
};

#endif // CONNECTION_H
