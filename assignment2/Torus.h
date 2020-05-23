//
// Created by Jack Breurkes on 5/05/20.
//

#ifndef H_CONE
#define H_CONE
#include <glm/glm.hpp>
#include "SceneObject.h"


/**
 * defines a cone located at center
 */
class Cone : public SceneObject {

private:
    glm::vec3 center = glm::vec3(0);
    float radius = 1;
    float height = 1;

public:
    Cone() {};  //Default constructor creates a unit cone at the origin

    Cone(glm::vec3 c, float r, float h) : center(c), radius(r), height(h) {}

    float intersect(glm::vec3 p0, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

};

#endif //!H_CONE
