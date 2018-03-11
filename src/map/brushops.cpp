//========= Copyright � 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "brushops.h"
#include "worldsize.h"



#define	SIDE_FRONT		0
#define	SIDE_BACK		1
#define	SIDE_ON			2

#define	BOGUS_RANGE	( MAX_COORD_INTEGER * 4 )

float		lightaxis[3] = {1, 0.6f, 0.75f};

const int MAX_POINTS_ON_WINDING	= 128;


void Error(const char* fmt, ...)
{
	printf(fmt, (&fmt)+1);
}


/*
=============================================================================

			TURN PLANES INTO GROUPS OF FACES

=============================================================================
*/


/*
==================
NewWinding
==================
*/
winding_t *NewWinding (int points)
{
	winding_t	*w;

	if (points > MAX_POINTS_ON_WINDING)
		Error ("NewWinding: %i points", points);

	w = (winding_t *)malloc(sizeof(*w));
	w->numpoints = 0; // None are occupied yet even though allocated.
    w->p = (glm::vec3 *)calloc( points, sizeof(glm::vec3) );

	return w;
}

void FreeWinding (winding_t *w)
{
	if (*(unsigned *)w == 0xdeaddead)
		Error ("FreeWinding: freed a freed winding");
	*(unsigned *)w = 0xdeaddead;

	if (w->p)
	{
	    free (w->p);
		w->p = NULL;
	}
	free (w);
}

size_t WindingSize(int points)
{
	return (size_t)(&((winding_t *)0)->p[points]);
}


//-----------------------------------------------------------------------------
// Purpose: Removes points that are withing a given distance from each other
//			from the winding.
// Input  : pWinding - The winding to remove duplicates from.
//			fMinDist - The minimum distance two points must be from one another
//				to be considered different. If this is zero, the points must be
//				identical to be considered duplicates.
//-----------------------------------------------------------------------------
void RemoveDuplicateWindingPoints(winding_t *pWinding, float fMinDist)
{
	for (int i = 0; i < pWinding->numpoints; i++)
	{
		for (int j = i + 1; j < pWinding->numpoints; j++)
		{
            glm::vec3 edge = pWinding->p[i] - pWinding->p[j];

			if (edge.length() < fMinDist)
			{
				if (j + 1 < pWinding->numpoints)
				{
					memmove(&pWinding->p[j], &pWinding->p[j + 1], (pWinding->numpoints - (j + 1)) * sizeof(pWinding->p[0]));
				}

				pWinding->numpoints--;
			}
		}		
	}
}


/*
==================
CopyWinding
==================
*/
winding_t	*CopyWinding (winding_t *w)
{
	int			size;
	winding_t	*c;

	c = NewWinding (w->numpoints);
	c->numpoints = w->numpoints;
	size = w->numpoints*sizeof(w->p[0]);
	memcpy (c->p, w->p, size);
	return c;
}


/*
==================
ClipWinding

Clips the winding to the plane, returning the new winding on the positive side
Frees the input winding.
==================
*/
// YWB ADDED SPLIT EPS to match qcsg splitting
#define	SPLIT_EPSILON	0.01
winding_t *ClipWinding (winding_t *in, Plane *split)
{
	float	dists[MAX_POINTS_ON_WINDING];
	int		sides[MAX_POINTS_ON_WINDING];
	int		counts[3];
	float	dot;
	int		i, j;
    glm::vec3	*p1, *p2, *mid;
	winding_t	*neww;
	int		maxpts;

	counts[0] = counts[1] = counts[2] = 0;

	// determine sides for each point
	for (i=0 ; i<in->numpoints ; i++)
	{
        dot = glm::dot(in->p[i], split->_normal);
        dot -= split->_distance;
		dists[i] = dot;
		if (dot > SPLIT_EPSILON)
			sides[i] = SIDE_FRONT;
		else if (dot < -SPLIT_EPSILON)
			sides[i] = SIDE_BACK;
		else
		{
			sides[i] = SIDE_ON;
		}
		counts[sides[i]]++;
	}
	sides[i] = sides[0];
	dists[i] = dists[0];

	if (!counts[0] && !counts[1])
		return in;

	if (!counts[0])
	{
		free (in);
		return NULL;
	}
	if (!counts[1])
		return in;

	maxpts = in->numpoints+4;	// can't use counts[0]+2 because
								// of fp grouping errors
	neww = NewWinding (maxpts);

	for (i=0 ; i<in->numpoints ; i++)
	{
		p1 = &in->p[i];

		mid = &neww->p[neww->numpoints];

		if (sides[i] == SIDE_FRONT || sides[i] == SIDE_ON)
		{
			*mid = *p1;
			neww->numpoints++;
			if (sides[i] == SIDE_ON)
				continue;
			mid = &neww->p[neww->numpoints];
		}

		if (sides[i+1] == SIDE_ON || sides[i+1] == sides[i])
			continue;

	// generate a split point
		if (i == in->numpoints - 1)
			p2 = &in->p[0];
		else
			p2 = p1 + 1;

		neww->numpoints++;

		dot = dists[i] / (dists[i]-dists[i+1]);
		for (j=0 ; j<3 ; j++)
		{	// avoid round off error when possible
            if (split->_normal[j] == 1)
                mid[0][j] = split->_distance;
            else if (split->_normal[j] == -1)
                mid[0][j] = -split->_distance;
			mid[0][j] = p1[0][j] + dot*(p2[0][j]-p1[0][j]);
		}
//		mid[3] = p1[3] + dot*(p2[3]-p1[3]);
//		mid[4] = p1[4] + dot*(p2[4]-p1[4]);
	}

	if (neww->numpoints > maxpts)
		Error ("ClipWinding: points exceeded estimate");

// free the original winding
	FreeWinding (in);

	return neww;
}



void VectorMA (const glm::vec3& va, double scale, const glm::vec3& vb, glm::vec3& vc)
{
    vc.x = va.x + scale*vb.x;
    vc.y = va.y + scale*vb.y;
    vc.z = va.z + scale*vb.z;
}

//-----------------------------------------------------------------------------
// Purpose: Creates a huge quadrilateral winding given a plane.
// Input  : pPlane - Plane normal and distance to use when creating the winding.
// Output : Returns a winding with 4 points.
//-----------------------------------------------------------------------------
// dvs: read through this and clean it up
winding_t *CreateWindingFromPlane(Plane *pPlane)
{
	int		i, x;
	float	max, v;
    glm::vec3	org, vright, vup;
	winding_t	*w;
	
	// find the major axis
	max = -BOGUS_RANGE;
	x = -1;
	for (i=0 ; i<3; i++)
	{
        v = fabs(pPlane->_normal[i]);
		if (v > max)
		{
			x = i;
			max = v;
		}
	}
	if (x==-1)
		Error ("BasePolyForPlane: no axis found");

    vup = glm::vec3();
	switch (x)
	{
		case 0:
		case 1:
			vup[2] = 1;
			break;
		case 2:
			vup[0] = 1;
			break;
	}

    v = glm::dot(vup, pPlane->_normal);
    VectorMA (vup, -v, pPlane->_normal, vup);
    vup = glm::normalize(vup);

    org = glm::vec3(pPlane->_normal) * pPlane->_distance;

    vright = glm::cross(vup, pPlane->_normal);

    vup = glm::vec3(vup.x * MAX_TRACE_LENGTH, vup.y * MAX_TRACE_LENGTH, vup.z * MAX_TRACE_LENGTH);
    vright = glm::vec3(vright.x * MAX_TRACE_LENGTH, vright.y * MAX_TRACE_LENGTH, vright.z * MAX_TRACE_LENGTH);

	// project a really big	axis aligned box onto the plane
	w = NewWinding (4);
	w->numpoints = 4;

	w->p[0] = (org - vright);
	w->p[0] = (w->p[0] + vup);

	w->p[1] = (org + vright);
	w->p[1] = (w->p[1] + vup);

	w->p[2] = (org + vright);
	w->p[2] = (w->p[2] - vup);

	w->p[3] = (org - vright);
	w->p[3] = (w->p[3]-  vup);

	return w;
}
