#include "mapbrush.h"
#include "mapbrushside.h"

static unsigned long randomSeed = 0;

unsigned long nextNumbr()
{
    randomSeed = randomSeed * 214013L + 20171010L;

    return randomSeed;
}

MapBrush::MapBrush()
{
    _color = glm::vec4(0,
                       (100 + (nextNumbr() % 156)) / 255.0f,
                       (100 + (nextNumbr() % 156)) / 255.0f,
                       1.0f);
}

MapBrush::MapBrush(const MapBrush& cloneFrom)
{
    (*this) = cloneFrom;
}

MapBrush& MapBrush::operator = (const MapBrush& cloneFrom)
{
    _color = cloneFrom._color;
    this->_sides.clear();

    for (auto side : cloneFrom._sides) this->_sides.push_back(side);

    return (*this);
}
