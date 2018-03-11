#ifndef MAPBRUSHSIDE_H
#define MAPBRUSHSIDE_H

#include <glm/glm.hpp>
#include "mapbrushsidematerial.h"

class MapBrushSide
{
public:
    MapBrushSide();
    MapBrushSide(const MapBrushSide& cloneFrom);

    MapBrushSide& operator = (const MapBrushSide& cloneFrom);

    glm::vec3 _normal;
    float _distance;
    MapBrushSideMaterial _material;

public:
    static MapBrushSide FromVertices(const glm::vec3 a, const glm::vec3& b, const glm::vec3& c);
};

#endif // MAPBRUSHSIDE_H
