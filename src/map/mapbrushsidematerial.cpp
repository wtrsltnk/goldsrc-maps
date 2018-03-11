#include "mapbrushsidematerial.h"

MapBrushSideMaterial::MapBrushSideMaterial()
{ }

MapBrushSideMaterial::MapBrushSideMaterial(const MapBrushSideMaterial& cloneFrom)
{
    (*this) = cloneFrom;
}

MapBrushSideMaterial& MapBrushSideMaterial::operator = (const MapBrushSideMaterial& cloneFrom)
{
    this->_axis1 = cloneFrom._axis1;
    this->_axis2 = cloneFrom._axis2;
    this->_offset1 = cloneFrom._offset1;
    this->_offset2 = cloneFrom._offset2;
    this->_rotation = cloneFrom._rotation;
    this->_scalex = cloneFrom._scalex;
    this->_scaley = cloneFrom._scaley;
    this->_texture = cloneFrom._texture;

    return (*this);
}
