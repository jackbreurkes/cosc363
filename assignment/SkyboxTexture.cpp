//
// Created by Jack Breurkes on 9/03/20.
//

#include "SkyboxTexture.h"
#include "GLUT/glut.h"

bool SkyboxTexture::Load() {
    glGenTextures(1, &texture_object);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_object);

}