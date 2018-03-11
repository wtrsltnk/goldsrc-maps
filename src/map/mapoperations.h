#ifndef MAPOPERATIONS_H
#define MAPOPERATIONS_H

#include <vector>
#include <glm/glm.hpp>

class MapOperations
{
public:
    MapOperations();

    void Clip(const std::vector<class MapBrush*>& brushes, const glm::vec4& plane);
};

#endif // MAPOPERATIONS_H
