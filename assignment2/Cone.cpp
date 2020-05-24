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
    glm::vec3 diff = p0 - center;
    float yDelta = height - p0.y + center.y;

    glm::vec3 v(0, -1, 0);

    float rOverHSquared = (radius / height) * (radius / height);
    float a = (dir.x * dir.x) + (dir.z * dir.z) - (rOverHSquared * (dir.y * dir.y));
    float b = 2*(diff.x * dir.x + diff.z * dir.z + rOverHSquared * yDelta * dir.y);
    float c = (diff.x * diff.x) + (diff.z * diff.z) - (rOverHSquared * (yDelta * yDelta));

    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0 || fabs(discriminant) < 0.001) { // negative or zero discriminant
        return -1.0;
    }

    float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
    float t2 = (-b + sqrt(discriminant)) / (2.0f * a);

    if(fabs(t1)<0.01){
        t1=-1;
    }
    if(fabs(t2)<0.01){
        t2=-1;
    }

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
        return t2;
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
* Assumption: The input point p lies on the cone.
*/
glm::vec3 Cone::normal(glm::vec3 p)
{
    float alpha = std::atan2(p.x - center.x, p.z - center.z);
    float theta = std::atan2(radius, height);
    glm::vec3 n(std::sin(alpha) * std::cos(theta), std::sin(theta), std::cos(alpha) * std::cos(theta));
    n = glm::normalize(n);
    return n;
}
