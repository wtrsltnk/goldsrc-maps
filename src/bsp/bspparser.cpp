#include "bspparser.h"
#include "bspscene.h"
#include "bsptypes.h"

BspParser::BspParser(unsigned char *data, int size)
    : _data(data), _size(size)
{ }

template <class T>
std::vector<T> loadLump(tBspLump const &lump, unsigned char *data)
{
    auto faceData = reinterpret_cast<T const*>(data + lump.offset);

    return std::vector<T>(faceData, faceData + (lump.size / sizeof(T)));
}

bool BspParser::LoadScene(class BspScene* scene)
{
    auto header = reinterpret_cast<tBspHeader const*>(_data);

    if (header->signature != HL1_BSP_SIGNATURE)
    {
        return false;
    }

    scene->faces = loadLump<tBspFace>(header->lumps[HL1_BSP_FACELUMP], _data);
    scene->models = loadLump<tBspModel>(header->lumps[HL1_BSP_MODELLUMP], _data);
}
