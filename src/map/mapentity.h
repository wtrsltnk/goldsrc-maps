#ifndef MAPENTITY_H
#define MAPENTITY_H

#include <vector>

class MapEntity
{
public:
    MapEntity();

    void Clear();

    std::vector<class MapBrush*> _brushes;
    std::vector<class MapKeyValuePair*> _keyValuePairs;
};

#endif // MAPENTITY_H
