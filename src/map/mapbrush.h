#ifndef MAPBRUSH_H
#define MAPBRUSH_H

#include <vector>
#include "mapbrushside.h"

class MapBrush
{
public:
    MapBrush();
    MapBrush(const MapBrush& cloneFrom);

    MapBrush& operator = (const MapBrush& cloneFrom);

    std::vector<MapBrushSide> _sides;
};

#endif // MAPBRUSH_H
