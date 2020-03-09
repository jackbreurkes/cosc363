//
// Created by Jack Breurkes on 9/03/20.
//

#ifndef ASSIGNMENT_SKYBOXTEXTURE_H
#define ASSIGNMENT_SKYBOXTEXTURE_H

#include <string>
#include <OpenGL/OpenGL.h>

class SkyboxTexture {
public:
    SkyboxTexture(
            const std::string& directory,
            const std::string& pos_x_filename,
            const std::string& neg_x_filename,
            const std::string& pos_y_filename,
            const std::string& neg_y_filename,
            const std::string& pos_z_filename,
            const std::string& neg_z_filename);
    ~SkyboxTexture();

    bool Load();

    void Bind(GLenum texture_unit);

private:
    std::string filenames[6];
    GLuint texture_object;
};


#endif //ASSIGNMENT_SKYBOXTEXTURE_H
