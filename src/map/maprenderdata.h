#ifndef MAPRENDERDATA_H
#define MAPRENDERDATA_H

#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct MapVertex
{
    MapVertex(glm::vec3 const &pos, glm::vec3 const &normal);

    glm::vec3 _pos;
    glm::vec2 _uv;
    glm::vec3 _normal;
};

class MapFace
{
public:
    MapFace(const std::string& texture, int first, int count);

    std::string _texture;
    int _firstVertex;
    int _vertexCount;
    glm::vec4 _color;
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
