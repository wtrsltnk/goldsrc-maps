#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <vector>

enum SceneFlags
{
    Modified = 1 << 1,
};

class MapScene
{
public:
    MapScene();

    void Clear();
    std::vector<class MapEntity*> _entities;
    int _flags;
};

#endif // MAPSCENE_H
