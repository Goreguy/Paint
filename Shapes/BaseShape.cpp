#include "BaseShape.h"

BaseShape::BaseShape(ShapeType type)
    :shape_type(type)
{
}

std::unique_ptr<BaseShape> BaseShape::deserialize(const QJsonObject& obj)
{

}
