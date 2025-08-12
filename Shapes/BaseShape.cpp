#include "BaseShape.h"

BaseShape::BaseShape(ShapeType type, const QRect& r)
    :shape_type(type), rectangle{r}
{
}

std::unique_ptr<BaseShape> BaseShape::deserialize(const QJsonObject& obj)
{

}
