#ifndef MAPBRUSH_H
#define MAPBRUSH_H

#include <vector>
#include <glm/glm.hpp>
#include "mapbrushside.h"

class MapBrush
{
public:
    MapBrush();
    MapBrush(const MapBrush& cloneFrom);

    MapBrush& operator = (const MapBrush& cloneFrom);

    std::vector<MapBrushSide> _sides;
    glm::vec4 _color;
};

#endif // MAPBRUSH_H
