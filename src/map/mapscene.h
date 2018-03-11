#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <vector>

class MapScene
{
public:
    MapScene();

    void Clear();
    std::vector<class MapEntity*> _entities;
};

#endif // MAPSCENE_H
