//
// Created by Jack Breurkes on 23/05/20.
//

#include "Torus.h"
#include "roots3and4.c"
#include <iostream>
#include <math.h>
#include <algorithm>

/**
* Torus's intersection method.  The input is a ray.
*/
float Torus::intersect(glm::vec3 p0, glm::vec3 dir) // TODO only renders correctly at center = (0, 0, 0)
{
    float torusDist = glm::length(center);
    p0 -= center; // moves the ray to avoid complicating the torus equation TODO help

    float sum_d_sqrd = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
    float e = p0.x * p0.x + p0.y * p0.y + p0.z * p0.z - majorRadius * majorRadius - minorRadius * minorRadius;
    float f = p0.x * dir.x + p0.y * dir.y + p0.z * dir.z;
    float four_a_sqrd = 4.0f * majorRadius * minorRadius;

    double coefficients[5] = { // gives us the coefficients of the quartic equation that needs to be solved to give torus intersects
            e * e - four_a_sqrd * (minorRadius * minorRadius - p0.y * p0.y), // constant term
            4.0f * f * e + 2.0f * four_a_sqrd * p0.y * dir.y, // t term
            2.0f * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * dir.y * dir.y, // t^2 term
            4.0f * sum_d_sqrd * f, // t^3 term
            sum_d_sqrd * sum_d_sqrd}; // t^4 term

    double solutions[4] = {-1.0, -1.0, -1.0, -1.0};
    int nonComplexCount = SolveQuartic(coefficients, solutions); // fills the solutions array with the non-complex solutions

    for (int i = 0; i < nonComplexCount; i++) {
        if ((solutions[i] - torusDist) > 0.01) { // check that solution is not for intersection at p0 (secondary rays) TODO fix
//            std::cout << "z coord: " << p0.z + solutions[i] * dir.z << std::endl;
            return solutions[i];
        }
    }
    return -1.;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the torus.
*/
glm::vec3 Torus::normal(glm::vec3 p)
{
//    std::cout << p.x << " " << p.y << " " << p.z << std::endl;
    p -= center; // TODO fix with moving the torus
    float paramSquared = majorRadius * majorRadius + minorRadius * minorRadius;
    float sumSquared = p.x * p.x + p.y * p.y + p.z * p.z;
    glm::vec3 n(4.0 * p.x * (sumSquared - paramSquared),
            4.0 * p.y * (sumSquared - paramSquared + 2.0 * majorRadius * majorRadius),
            4.0 * p.z * (sumSquared - paramSquared));
    n = glm::normalize(n);
    return n;
}
