#ifndef BSPRENDERDATA_H
#define BSPRENDERDATA_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct BspVertex
{
    glm::vec3 position;
    glm::vec2 texcoords[2];
    glm::vec3 normal;
};

struct BspFace
{
    int firstVertex;
    int vertexCount;
    unsigned int lightmap;
    unsigned int texture;

    int flags;
    glm::vec4 plane;

};

class BspRenderData
{
public:
    BspRenderData();

    void Clear();
    void Collect(const class BspScene* scene);

    std::vector<BspFace> _data;
    std::vector<BspVertex> _verts;
};

#endif // BSPRENDERDATA_H
