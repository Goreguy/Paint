#include "connection.h"


void Connection::draw(QPainter &p) const
{
    p.save();
    p.setPen(QPen(Qt::black, 1));
    p.drawLine(a->center(), b->center());
    p.restore();
}

void Connection::serialize(QDataStream &out, const std::vector<BaseShape*> &shapes) const
{
    // write indices of shapes in shapes vector
    out << a->getId();
    out << b->getId();
}

// static std::unique_ptr<Connection> Connection::deserialize(QDataStream &in, const std::unordered_map<int, BaseShape*>& shapeMap)
// {
//     int fromId, toId;
//     in >> fromId >> toId;

//     auto itFrom = shapeMap.find(fromId);
//     auto itTo = shapeMap.find(toId);
//     if (itFrom != shapeMap.end() && itTo != shapeMap.end()) {
//         return std::make_unique<Connection>(itFrom->second, itTo->second);
//     }
//     return nullptr;
// }
