#include "maprenderdata.h"
#include "mapscene.h"
#include "mapentity.h"
#include "mapbrush.h"
#include "mapbrushside.h"
#include "brushops.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glm/gtx/string_cast.hpp>

MapVertex::MapVertex(const glm::vec3& pos)
    : _pos(pos)
{ }

MapFace::MapFace(const std::string& texture, int first, int count)
    : _texture(texture), _firstVertex(first), _vertexCount(count)
{ }

MapRenderData::MapRenderData()
{ }

void MapRenderData::Clear()
{
    this->_data.clear();
    this->_verts.clear();
}

void MapRenderData::Collect(const MapScene* scene)
{
    this->Collect(scene->_entities);
}

void MapRenderData::Collect(const std::vector<MapEntity*>& entities)
{
    for (auto entity : entities)
    {
        this->Collect(entity);
    }
}

void MapRenderData::UpdateTextureData(MapBrushSide& side, const MapFace& face)
{
    // TODO : get the texture width/height from the texture/material manager
    auto textureWidth = 32;
    auto textureHeight = 32;

    float W, H, SX = 1.0f, SY = 1.0f;
    W = 1.0f / float(textureWidth);
    H = 1.0f / float(textureHeight);
    SX = 1.0f / float(side._material._scalex);
    SY = 1.0f / float(side._material._scaley);

    for (int i = 0; i < face._vertexCount; i++)
    {
        glm::vec3 v = this->_verts[face._firstVertex + i]._pos;
        this->_verts[i]._uv.x = (glm::dot(v, side._material._axis1) * W * SX) + (side._material._offset1 * W);
        this->_verts[i]._uv.y = (glm::dot(v, side._material._axis2) * H * SY) + (side._material._offset2 * H);
    }
}

void MapRenderData::Collect(const MapEntity* entity)
{
    for (MapBrush* brush : entity->_brushes)
    {
        winding_t** windings = new winding_t*[brush->_sides.size()];
        for (int i = 0; i < brush->_sides.size(); i++)
        {
            auto side = brush->_sides[i];
            auto planei = Plane(side._distance, side._normal);
            windings[i] = CreateWindingFromPlane(&planei);
            for (int j = 0; j < brush->_sides.size(); j++)
            {
                if (i != j && windings[i] != 0)
                {
                    auto planej = new Plane(brush->_sides[j]._distance, brush->_sides[j]._normal);
                    windings[i] = ClipWinding(windings[i], planej);
                }
            }
            if (windings[i] != 0)
            {
                MapFace face(side._material._texture, this->_verts.size(), windings[i]->numpoints);

                // Change order of vertices, otherwise it will show 'backsides'
                for (int k = 0; k < windings[i]->numpoints; k++)
                {
                    glm::vec3 a = windings[i]->p[k];
//                    std::cout << glm::to_string(a) << "\n";
                    this->_verts.push_back(MapVertex(a));
                }
                this->UpdateTextureData(side, face);
                auto found = this->_data.find(side._material._texture);
                if (found == this->_data.end())
                {
                    this->_data.insert(this->_data.end(), std::make_pair(side._material._texture, std::vector<MapFace>()));
                }
                this->_data[side._material._texture].push_back(face);
            }
        }

        // To prevent memory leak, cleanup the used windings
        for (int i = 0; i < brush->_sides.size(); i++)
        {
            if (windings[i] != 0)
            {
                FreeWinding(windings[i]);
            }
        }
    }
}
