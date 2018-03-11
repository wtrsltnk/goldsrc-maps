#ifndef MAPBRUSHSIDEMATERIAL_H
#define MAPBRUSHSIDEMATERIAL_H

#include <glm/glm.hpp>
#include <string>

class MapBrushSideMaterial
{
public:
    MapBrushSideMaterial();
    MapBrushSideMaterial(const MapBrushSideMaterial& cloneFrom);

    MapBrushSideMaterial& operator = (const MapBrushSideMaterial& cloneFrom);

    std::string _texture;
    glm::vec3 _axis1, _axis2;
    float _offset1, _offset2;
    float _rotation, _scalex, _scaley;
};

#endif // MAPBRUSHSIDEMATERIAL_H
