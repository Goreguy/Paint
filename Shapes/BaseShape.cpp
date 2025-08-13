#include "BaseShape.h"
#include "ellipse.h"
#include "rectangle.h"
#include "triangle.h"

#include <QDebug>

BaseShape::BaseShape(ShapeType type, const QRect& r)
    :shape_type(type), shapeId{countAndSetId()}, rectangle{r}
{
}

std::unique_ptr<BaseShape> BaseShape::deserialize(QDataStream &in) {
    ShapeType type;
    in >> type;
    switch (static_cast<ShapeType>(type)) {
    case ShapeType::Rectangle: return std::unique_ptr<BaseShape>(Rectangle::deserialize(in));
    case ShapeType::Ellipse:   return std::unique_ptr<BaseShape>(Ellipse::deserialize(in));
    case ShapeType::Triangle:   return std::unique_ptr<BaseShape>(Triangle::deserialize(in));
    }
    return nullptr;
}

int BaseShape::countAndSetId()
{
    static int counter{0};
    return ++counter;
}

void BaseShape::setId(int id)
{
    shapeId = id;
}

void BaseShape::addConnection(Connection *c)
{
    connections.push_back(c);
}

void BaseShape::removeConnection(Connection *c)
{
    auto it = std::find(connections.begin(), connections.end(), c);
    if (it != connections.end())
    {
        connections.erase(it);
    }
}


