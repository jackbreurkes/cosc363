//
// Created by Jack Breurkes on 9/03/20.
//

#include "SkyboxTexture.h"
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/freeglut.h"
#endif

bool SkyboxTexture::Load() {
    glGenTextures(1, &texture_object);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_object);


}