#include "bsprenderdata.h"
#include "bspscene.h"

BspRenderData::BspRenderData()
{}

void BspRenderData::Clear()
{
    this->_data.clear();
    this->_verts.clear();
}

void BspRenderData::Collect(const BspScene *scene)
{
    if (scene->vertices.empty())
    {
        return;
    }

    for (auto face : scene->faces)
    {
        BspFace outFace;

        outFace.firstVertex = _verts.size();
        outFace.vertexCount = face.edgeCount;
        outFace.plane = glm::vec4(
            scene->planes[face.planeIndex].normal[0],
            scene->planes[face.planeIndex].normal[1],
            scene->planes[face.planeIndex].normal[2],
            scene->planes[face.planeIndex].distance);

        if (face.side == 1)
        {
            outFace.plane[0] = -outFace.plane[0];
            outFace.plane[1] = -outFace.plane[1];
            outFace.plane[2] = -outFace.plane[2];
            outFace.plane[3] = -outFace.plane[3];
        }

        for (int e = 0; e < face.edgeCount; e++)
        {
            BspVertex v;

            int ei = scene->surfaceEdges[face.firstEdge + e];
            // Determine the vertex based on the edge index
            v.position = scene->vertices[scene->edges[ei < 0 ? -ei : ei].vertex[ei < 0 ? 1 : 0]].point;

            // Copy the normal from the plane
            v.normal = glm::vec3(outFace.plane);

            _verts.push_back(v);
        }
        _data.push_back(outFace);
    }
}
