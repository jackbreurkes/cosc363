//
// Created by Jack Breurkes on 5/05/20.
//

#ifndef H_CYLINDER
#define H_CYLINDER
#include <glm/glm.hpp>
#include "SceneObject.h"


/**
 * defines a cylinder located at center
 */
class Cylinder : public SceneObject {

private:
    glm::vec3 center = glm::vec3(0);
    float radius = 1;
    float height = 1;

public:
    Cylinder() {};  //Default constructor creates a unit cylinder at the origin

    Cylinder(glm::vec3 c, float r, float h) : center(c), radius(r), height(h) {}

    float intersect(glm::vec3 p0, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

};

#endif //!H_CYLINDER
