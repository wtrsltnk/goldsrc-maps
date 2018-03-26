#ifndef BSPTYPES_H
#define BSPTYPES_H

#include <glm/glm.hpp>
#include <string>
#include <map>

typedef std::map<std::string,std::string> KeyValueList;

#define HL1_BSP_SIGNATURE 30
#define HL1_BSP_LUMPCOUNT 15

#define HL1_BSP_ENTITYLUMP 0
#define HL1_BSP_PLANELUMP 1
#define HL1_BSP_TEXTURELUMP 2
#define HL1_BSP_VERTEXLUMP 3
#define HL1_BSP_VISIBILITYLUMP 4
#define HL1_BSP_NODELUMP 5
#define HL1_BSP_TEXINFOLUMP 6
#define HL1_BSP_FACELUMP 7
#define HL1_BSP_LIGHTINGLUMP 8
#define HL1_BSP_CLIPNODELUMP 9
#define HL1_BSP_LEAFLUMP 10
#define HL1_BSP_MARKSURFACELUMP 11
#define HL1_BSP_EDGELUMP 12
#define HL1_BSP_SURFEDGELUMP 13
#define HL1_BSP_MODELLUMP 14

#define HL1_BSP_MAX_MAP_HULLS 4
#define HL1_BSP_MAX_LIGHT_MAPS 4
#define HL1_BSP_MAX_AMBIENTS 4

#define HL1_WAD_SIGNATURE "WAD3"

#define CONTENTS_EMPTY  -1
#define CONTENTS_SOLID  -2
#define CONTENTS_WATER  -3
#define CONTENTS_SLIME  -4
#define CONTENTS_LAVA   -5
#define CONTENTS_SKY    -6
#define CONTENTS_ORIGIN -7 // removed at csg time
#define CONTENTS_CLIP   -8 // changed to contents_solid

#define CONTENTS_CURRENT_0    -9
#define CONTENTS_CURRENT_90   -10
#define CONTENTS_CURRENT_180  -11
#define CONTENTS_CURRENT_270  -12
#define CONTENTS_CURRENT_UP   -13
#define CONTENTS_CURRENT_DOWN -14

#define CONTENTS_TRANSLUCENT  -15

#pragma pack(push, 4)

/* BSP */
typedef struct sBspLump
{
    int offset;
    int size;

} tBspLump;

typedef struct sBspHeader
{
    int signature;
    tBspLump lumps[HL1_BSP_LUMPCOUNT];

} tBspHeader;

typedef struct sBspMipTexOffsetTable
{
    int miptexCount;
    int offsets[1];             /* an array with "miptexcount" number of offsets */

} tBspMipTexOffsetTable;

typedef struct sBspMipTexHeader
{
    char name[16];
    unsigned int width;
    unsigned int height;
    unsigned int offsets[4];

} tBspMipTexHeader;


typedef struct sBspModel
{
    glm::vec3 mins, maxs;
    glm::vec3 origin;
    int headnode[HL1_BSP_MAX_MAP_HULLS];
    int visLeafs;                       // not including the solid leaf 0
    int firstFace;
    int faceCount;

} tBspModel;

typedef struct sBspVertex
{
    glm::vec3 point;

} tBspVertex;

typedef struct sBspEdge
{
    unsigned short vertex[2];

} tBspEdge;

typedef int tBspSurfaceEdge;

typedef struct sBspFace
{
    short planeIndex;
    short side;
    int firstEdge;
    short edgeCount;
    short texinfo;

    // lighting info
    unsigned char styles[HL1_BSP_MAX_LIGHT_MAPS];
    int lightOffset;                    // start of [numstyles*surfsize] samples

} tBspFace;

typedef struct sBspPlane
{
    glm::vec3 normal;
    float distance;
    int type;

} tBspPlane;

typedef struct sBspNode
{
    int planeIndex;
    short children[2];                  // negative numbers are -(leafs+1), not nodes
    short mins[3];                      // for sphere culling
    short maxs[3];
    unsigned short firstFace;
    unsigned short faceCount;            // counting both sides

} tBspNode;

typedef struct sBspClipNode
{
    int planeIndex;
    short children[2];                  // negative numbers are contents

} tBspClipNode;

typedef struct sBspTexInfo
{
    glm::vec4 vecs[2];                // [s/t][xyz offset]
    int miptexIndex;
    int flags;

} tBspTexInfo;

typedef struct sBspLeaf
{
    int contents;
    int visofs;                         // -1 = no visibility info

    short mins[3];                      // for frustum culling
    short maxs[3];

    unsigned short firstMarkSurface;
    unsigned short markSurfacesCount;

    unsigned char ambientLevel[HL1_BSP_MAX_AMBIENTS];

} tBspLeaf;

typedef struct sBspEntity
{
    std::string classname;
    KeyValueList keyvalues;

} tBspEntity;

typedef struct sBspVisLeaf
{
    int leafCount;
    int* leafs;

} tBspVisLeaf;


/* WAD */
typedef struct sWadHeader
{
    char signature[4];
    int lumpsCount;
    int lumpsOffset;

} tWadHeader;

typedef struct sWadLump
{
    int offset;
    int sizeOnDisk;
    int size;
    char type;
    char compression;
    char empty0;
    char empty1;
    char name[16];

} tWadLump;

#pragma pack(pop)

#endif // BSPTYPES_H
