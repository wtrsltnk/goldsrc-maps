#ifndef BSPSCENE_H
#define BSPSCENE_H

#include "bsptypes.h"
#include <vector>

class BspScene
{
public:
    BspScene();

    std::vector<tBspPlane> planes;
    std::vector<tBspModel> models;
    std::vector<tBspVertex> vertices;
    std::vector<tBspEdge> edges;
    std::vector<tBspSurfaceEdge> surfaceEdges;
    std::vector<tBspFace> faces;
    std::vector<tBspNode> nodes;
    std::vector<tBspLeaf> leafs;
    std::vector<tBspClipNode> clipNodes;
    std::vector<tBspTexInfo> tetureInfo;
};

#endif // BSPSCENE_H
