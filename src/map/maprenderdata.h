#ifndef MAPRENDERDATA_H
#define MAPRENDERDATA_H

#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct MapVertex
{
    MapVertex(const glm::vec3& pos);

    glm::vec3 _pos;
    glm::vec2 _uv;
};

class MapFace
{
public:
    MapFace(const std::string& texture, int first, int count);

    std::string _texture;
    int _firstVertex;
    int _vertexCount;
};

class MapRenderData
{
public:
    MapRenderData();

    void Clear();
    void Collect(const class MapScene* scene);
    void Collect(const std::vector<class MapEntity*>& entities);
    void Collect(const class MapEntity* entity);
    void UpdateTextureData(class MapBrushSide& side, const class MapFace& face);

    std::map<std::string, std::vector<MapFace>> _data;
    std::vector<MapVertex> _verts;
};

#endif // MAPRENDERDATA_H
