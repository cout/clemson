#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "gl/aw.h"
#include "math/matrix.h"

int ray_intersect_plane(pVector p1, pVector p2, pVector face_normal,
        pVector face_center, float d, pVector p);

int point_inside_face(pVector p, Face* face, VertList* verts);

#endif
