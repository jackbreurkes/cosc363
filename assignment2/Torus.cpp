//
// Created by Jack Breurkes on 5/05/20.
//

#include "Cone.h"
#include <math.h>

/**
* Cone's intersection method.  The input is a ray.
*/
float Cone::intersect(glm::vec3 p0, glm::vec3 dir)
{
    return -1;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cone.
*/
glm::vec3 Cone::normal(glm::vec3 p)
{
    glm::vec3 n(p.x - center.x, 0, p.z - center.z); // TODO wrong equation
    n = glm::normalize(n);
    return n;
}
