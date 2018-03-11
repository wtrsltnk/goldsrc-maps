#include "mapentity.h"
#include "mapbrush.h"
#include "mapkeyvaluepair.h"

MapEntity::MapEntity()
{
}

void MapEntity::Clear()
{
    for (auto item : _brushes)
    {
        delete item;
    }
    _brushes.clear();

    for (auto item : _keyValuePairs)
    {
        delete item;
    }
    _keyValuePairs.clear();
}
