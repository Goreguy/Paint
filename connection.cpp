#include "connection.h"


void Connection::draw(QPainter &p) const
{
    p.save();
    p.setPen(QPen(Qt::black, 1));
    p.drawLine(a->center(), b->center());
    p.restore();
}

void Connection::serialize(QDataStream &out) const
{
    // write indices of shapes in shapes vector
    out << a->getId();
    out << b->getId();
}

std::pair<quint32, quint32> Connection::deserialize(QDataStream &in)
{
    quint32 fromId, toId;
    in >> fromId >> toId;
    return { fromId, toId };
}
