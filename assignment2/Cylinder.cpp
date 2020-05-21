//
// Created by Jack Breurkes on 5/05/20.
//

#include "Cylinder.h"
#include <math.h>

/**
* Cylinder's intersection method.  The input is a ray.
*/
float Cylinder::intersect(glm::vec3 p0, glm::vec3 dir)
{
    float a = dir.x * dir.x + dir.z * dir.z;
    float b = 2.0f * ( dir.x * ( p0.x - center.x ) + dir.z * ( p0.z - center.z ) );
    float c = (p0.x - center.x) * (p0.x - center.x) + (p0.z - center.z) * (p0.z - center.z) - radius * radius;
    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0 || fabs(discriminant) < 0.001) { // negative or zero discriminant
        return -1.0;
    }

    float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
    float t2 = (-b + sqrt(discriminant)) / (2.0f * a);
    if (t2 < t1) {
        float temp = t1;
        t1 = t2;
        t2 = temp;
    }

    glm::vec3 p1 = p0 + t1 * dir;
    glm::vec3 p2 = p0 + t2 * dir;
    float worldHeight = center.y + height;
    if (p1.y > worldHeight || p1.y < center.y) {
        if (p2.y > worldHeight || p2.y < center.y) {
            return -1.0; // both points miss
        }
        if (t2 > 0.001) { // return a point on the 'cap'
            float tCap = (center.y + height - p0.y) / dir.y;
            return tCap;
        }
    }

    if(fabs(t1) < 0.01 ) // hit point is at p0, i.e. this is a secondary ray starting on the cylinder
        // will not happen if p1 is outside of the cylinder y bounds
    {
        if (t2 > 0) return t2;
        else t1 = -1.0;
    }
    if(fabs(t2) < 0.01 ) t2 = -1.0;

    return (t1 < t2)? t1: t2;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cylinder.
*/
glm::vec3 Cylinder::normal(glm::vec3 p)
{
    if (fabs((center.y + height) - p.y) < 0.01) return glm::vec3(0, 1, 0);
    glm::vec3 n(p.x - center.x, 0, p.z - center.z);
    n = glm::normalize(n);
    return n;
}
