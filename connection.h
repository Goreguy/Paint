#ifndef CONNECTION_H
#define CONNECTION_H

#include <QPoint>
#include <QPainter>

#include "Shapes/BaseShape.h"

class Connection
{
public:
    Connection(BaseShape* a, BaseShape* b) : a(a), b(b) {}

    void draw(QPainter &p) const
    {
        p.save();
        p.setPen(QPen(Qt::black, 1));
        p.drawLine(a->center(), b->center());
        p.restore();
    }

    BaseShape* getFirstShape() const { return a; }
    BaseShape* getSecondShape() const { return b; }

    void translate(const QPoint &delta)
    {
        // connections don't have their own coords; they follow connected shapes
        Q_UNUSED(delta);
    }

    void serialize(QDataStream &out, const std::vector<BaseShape*> &shapes) const
    {
        // write indices of shapes in shapes vector
        int ia = -1, ib = -1;
        for(int i=0;i<(int)shapes.size();++i){ if(shapes[i]==a) ia=i; if(shapes[i]==b) ib=i; }
        out << ia << ib;
    }
    static std::pair<int,int> deserializePair(QDataStream &in){ int ia,ib; in>>ia>>ib; return {ia,ib}; }

private:
    BaseShape *a;
    BaseShape *b;
};

#endif // CONNECTION_H
