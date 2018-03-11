#include "mapbrushside.h"
#include <glm/glm.hpp>

MapBrushSide::MapBrushSide()
{ }

MapBrushSide::MapBrushSide(const MapBrushSide& cloneFrom)
{
    (*this) = cloneFrom;
}

MapBrushSide& MapBrushSide::operator = (const MapBrushSide& cloneFrom)
{
    this->_distance = cloneFrom._distance;
    this->_normal = cloneFrom._normal;
    this->_material = cloneFrom._material;

    return (*this);
}

MapBrushSide MapBrushSide::FromVertices(const glm::vec3 a, const glm::vec3& b, const glm::vec3& c)
{
    auto normal = glm::normalize(glm::cross(c-b, a-b));

    auto side = MapBrushSide();
    side._normal = normal;
    side._distance = glm::dot(normal, b);

    return side;
}
