//
// Created by Jack Breurkes on 5/05/20.
//

#include "Cylinder.h"
#include <math.h>

/**
* Sphere's intersection method.  The input is a ray.
*/
float Cylinder::intersect(glm::vec3 p0, glm::vec3 dir)
{
    return -1;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Cylinder::normal(glm::vec3 p)
{
    glm::vec3 n(p.x - center.x, 0, p.z - center.z);
    n = glm::normalize(n);
    return n;
}
