//
// Created by Jack Breurkes on 23/05/20.
//

#ifndef H_TORUS
#define H_TORUS
#include <glm/glm.hpp>
#include "SceneObject.h"


/**
 * defines a torus located at a point center
 */
class Torus : public SceneObject {

private:
    glm::vec3 center = glm::vec3(0);
    float majorRadius = 5; // distance from center of hole to center of tube
    float minorRadius = 1; // radius of tube

public:
    Torus() {};  //Default constructor creates a unit cone at the origin

    Torus(glm::vec3 c, float R, float r) : center(c), majorRadius(R), minorRadius(r) {}

    float intersect(glm::vec3 p0, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

};

#endif //!H_TORUS
