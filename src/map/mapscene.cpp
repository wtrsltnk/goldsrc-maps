#include "mapscene.h"
#include "mapentity.h"

MapScene::MapScene()
    : _flags(0)
{
}

void MapScene::Clear()
{
    for (auto entity : _entities)
    {
        entity->Clear();
        delete entity;
    }
    _entities.clear();
}
