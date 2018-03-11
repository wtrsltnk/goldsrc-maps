//========= Copyright � 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef BRUSHOPS_H
#define BRUSHOPS_H

#include <glm/glm.hpp>

class Plane
{
public:
    Plane() : _distance(0.0f) { }
    Plane(float distance, const glm::vec3& normal) : _normal(normal), _distance(distance) { }
    virtual ~Plane() { }

    glm::vec3 _normal;
    float _distance;
};

typedef struct
{
	int		numpoints;
    glm::vec3	*p;			// variable sized
	
} winding_t;

#define	ON_EPSILON				0.5f
#define	ROUND_VERTEX_EPSILON	0.01f		// Vertices within this many units of an integer value will be rounded to an integer value.
#define	VECTOR_EPSILON			0.0001f


winding_t *ClipWinding(winding_t *in, Plane *split);
winding_t *CopyWinding(winding_t *w);
winding_t *NewWinding(int points);
void FreeWinding (winding_t *w);
winding_t *CreateWindingFromPlane(Plane *pPlane);
size_t WindingSize(int points);
void RemoveDuplicateWindingPoints(winding_t *pWinding, float fMinDist = 0);


#endif // BRUSHOPS_H
