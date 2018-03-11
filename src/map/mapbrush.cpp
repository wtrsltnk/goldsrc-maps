#include "mapbrush.h"
#include "mapbrushside.h"

MapBrush::MapBrush() { }

MapBrush::MapBrush(const MapBrush& cloneFrom)
{
    (*this) = cloneFrom;
}

MapBrush& MapBrush::operator = (const MapBrush& cloneFrom)
{
    this->_sides.clear();

    for (auto side : cloneFrom._sides) this->_sides.push_back(side);

    return (*this);
}
