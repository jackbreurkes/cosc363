//
// Created by Jack Breurkes on 8/03/20.
//

#include <cmath>
#include "loadBMP.h"
#include "loadTGA.h"
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/freeglut.h"
#endif
using namespace std;

#define CAM_HEIGHT 5
#define RAD_TO_DEG (180.0 / 3.14159)

float cam_pos_x = 0;
float cam_pos_z = 60;
float cam_front_x = 0;
float cam_front_z = -1;
float cam_rot = 0;

GLuint texId[10];


void loadTexture() {
    glGenTextures(10, texId); 	// Create texture ids

    glBindTexture(GL_TEXTURE_2D, texId[0]);  //Use this texture
    loadBMP("Daylight Box_Back.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[1]);  //Use this texture
    loadBMP("Daylight Box_Right.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[2]);  //Use this texture
    loadBMP("Daylight Box_Front.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[3]);  //Use this texture
    loadBMP("Daylight Box_Left.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[4]);  //Use this texture
    loadBMP("Daylight Box_Top.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[5]);  //Use this texture
    loadBMP("Daylight Box_Bottom.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[6]);  //Use this texture
    loadTGA("tree.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[7]);  //Use this texture
    loadBMP("brick.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[8]);  //Use this texture
    loadBMP("water.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[9]);  //Use this texture
    loadTGA("tree.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}



/**
 * adapted from explanation in section 8 of http://www.lighthouse3d.com/opengl/billboarding/billboardingtut.pdf
 * @param obj_pos array to load with object position information in format [x, y, z]
 */
void getObjectPosition(float* obj_pos) {
    float transMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, transMatrix);

    float M1T[9];  // transpose of first 3 rows/cols of transMatrix
    M1T[0] = transMatrix[0];
    M1T[1] = transMatrix[4];
    M1T[2] = transMatrix[8];
    M1T[3] = transMatrix[1];
    M1T[4] = transMatrix[5];
    M1T[5] = transMatrix[9];
    M1T[6] = transMatrix[2];
    M1T[7] = transMatrix[6];
    M1T[8] = transMatrix[10];

    float V[3];  // vertical matrix
    V[0] = transMatrix[12];
    V[1] = transMatrix[13];
    V[2] = transMatrix[14];

    // object pos in world coordinates = camera position + M1T * V
    float M1TmultV[3];
    M1TmultV[0] = M1T[0] * V[0] + M1T[3] * V[1] + M1T[6] * V[2];
    M1TmultV[1] = M1T[1] * V[0] + M1T[4] * V[1] + M1T[7] * V[2];
    M1TmultV[2] = M1T[2] * V[0] + M1T[5] * V[1] + M1T[8] * V[2];

    // object position in [x, y, z] format
    obj_pos[0] = cam_pos_x + M1TmultV[0];
    obj_pos[1] = CAM_HEIGHT + M1TmultV[1];
    obj_pos[2] = cam_pos_z + M1TmultV[2];
}



/**
 * sets up cylindrical billboarding by applying a rotation pointing the current object at the camera
 */
void setupCylindricalBillboarding() {
    float obj_pos[3];
    getObjectPosition(obj_pos);
    glPushMatrix();
    glRotatef(atan2(obj_pos[0] - cam_pos_x, obj_pos[2] - cam_pos_z) * RAD_TO_DEG,
            0, 1, 0);
}


void teardownBillboard() {
    glPopMatrix();
}

typedef struct {
    float speed;
    float height;
    float x, z;
} Particle;

#define NUM_FLAMES 50
Particle flames[NUM_FLAMES];
void drawParticles() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[6]);

    glPushMatrix();
    // code for billboarding
    for (int i = 0; i < NUM_FLAMES; i++) {
        setupCylindricalBillboarding();

        glBegin(GL_QUADS);
        glTexCoord2f(1, 1);     glVertex3f(flames[i].x-0.5, flames[i].height-0.5, flames[i].z);
        glTexCoord2f(1, 0);     glVertex3f(flames[i].x-0.5, flames[i].height+0.5, flames[i].z);
        glTexCoord2f(0, 0);     glVertex3f(flames[i].x+0.5, flames[i].height+0.5, flames[i].z);
        glTexCoord2f(0, 1);     glVertex3f(flames[i].x+0.5, flames[i].height-0.5, flames[i].z);
        glEnd();
    teardownBillboard();
    }
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void createParticles() {
    for (int i = 0; i < NUM_FLAMES; i++) {
        flames[i].height = drand48() * 5.;
        flames[i].x = -2. + drand48() * 4.;
        flames[i].z = -2. + drand48() * 4.;
        flames[i].speed = 0.05 + drand48() * 0.05;
    }
}


void updateParticles() {
    for (int i = 0; i < NUM_FLAMES; i++) {
        flames[i].height += flames[i].speed;
        if (flames[i].height > 5.) {
            flames[i].height -= 5.0;
        }
    }
}


float skybox_dist = 400.;
float skybox_half_width = 400.;
float skybox_half_height = 200.;

void drawSkybox()
{
    glColor3f(0.8, 0.7, 0.3);   //replace with a texture

    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(cam_pos_x, CAM_HEIGHT, cam_pos_z);

    ////////////////////// FRONT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1);         glVertex3f(-skybox_half_width, skybox_half_height, skybox_dist);
    glTexCoord2f(1, 0);         glVertex3f(-skybox_half_width, -skybox_half_height, skybox_dist);
    glTexCoord2f(0, 0);         glVertex3f(skybox_half_width, -skybox_half_height, skybox_dist);
    glTexCoord2f(0, 1);         glVertex3f(skybox_half_width, skybox_half_height, skybox_dist);
    glEnd();

    ////////////////////// RIGHT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);         glVertex3f(skybox_half_width, skybox_half_height, -skybox_dist);
    glTexCoord2f(0, 0);         glVertex3f(skybox_half_width, -skybox_half_height, -skybox_dist);
    glTexCoord2f(1, 0);         glVertex3f(skybox_half_width, -skybox_half_height, skybox_dist);
    glTexCoord2f(1, 1);         glVertex3f(skybox_half_width, skybox_half_height, skybox_dist);
    glEnd();

    ////////////////////// BACK WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);         glVertex3f(-skybox_half_width, skybox_half_height, -skybox_dist);
    glTexCoord2f(0, 0);         glVertex3f(-skybox_half_width, -skybox_half_height, -skybox_dist);
    glTexCoord2f(1, 0);         glVertex3f(skybox_half_width, -skybox_half_height, -skybox_dist);
    glTexCoord2f(1, 1);         glVertex3f(skybox_half_width, skybox_half_height, -skybox_dist);
    glEnd();

    ////////////////////// LEFT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1);         glVertex3f(-skybox_half_width, skybox_half_height, -skybox_dist);
    glTexCoord2f(1, 0);         glVertex3f(-skybox_half_width, -skybox_half_height, -skybox_dist);
    glTexCoord2f(0, 0);         glVertex3f(-skybox_half_width, -skybox_half_height, skybox_dist);
    glTexCoord2f(0, 1);         glVertex3f(-skybox_half_width, skybox_half_height, skybox_dist);
    glEnd();

    ////////////////////// CEILING ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);         glVertex3f(-skybox_half_width, skybox_half_height, skybox_dist);
    glTexCoord2f(1, 1);         glVertex3f(skybox_half_width, skybox_half_height, skybox_dist);
    glTexCoord2f(1, 0);         glVertex3f(skybox_half_width, skybox_half_height, -skybox_dist);
    glTexCoord2f(0, 0);         glVertex3f(-skybox_half_width, skybox_half_height, -skybox_dist);
    glEnd();

    ////////////////////// FLOOR ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 0);         glVertex3f(-skybox_half_width, -skybox_half_height, skybox_dist);
    glTexCoord2f(1, 1);         glVertex3f(skybox_half_width, -skybox_half_height, skybox_dist);
    glTexCoord2f(0, 1);         glVertex3f(skybox_half_width, -skybox_half_height, -skybox_dist);
    glTexCoord2f(0, 0);         glVertex3f(-skybox_half_width, -skybox_half_height, -skybox_dist);
    glEnd();


    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}


void drawTree() {
    glPushMatrix();
    glScalef(10, 10, 10);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);
    glBindTexture(GL_TEXTURE_2D, texId[6]);

    setupCylindricalBillboarding();
    glBegin(GL_QUADS);
    glTexCoord2f(1, 0);     glVertex3f(0.5, 1, 0);
    glTexCoord2f(0, 0);     glVertex3f(-0.5, 1, 0);
    glTexCoord2f(0, 1);     glVertex3f(-0.5, 0, 0);
    glTexCoord2f(1, 1);     glVertex3f(0.5, 0, 0);
    glEnd();
    teardownBillboard();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);


    glPopMatrix();
}



void drawTrees() {
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        float x = i < 2 ? -20 : 20;
        float z = i % 2 == 0 ? -20 : 20;
        glTranslatef(x, 0, z);
        drawTree();
        glPopMatrix();
    }
}


void drawFloor()
{
    int square_size = 5;
    glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);

    for (int i = -100; i < 100; i += square_size) {
        for (int j = -100; j < 100; j += square_size) {
            if ((j + i) % 2 == 0) {
                glColor3f(0.95, 0.95,  0.95);			//Floor colour
            } else {
                glColor3f(0.1, 0.1,  0.1);
            }
            glVertex3f(i, 0, j);
            glVertex3f(i+square_size, 0, j);
            glVertex3f(i+square_size, 0, j+square_size);
            glVertex3f(i, 0, j+square_size);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);
}


//--Function to compute the normal vector of a triangle given three points on it (ccw) ----------
void normal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    float nx, ny, nz;
    nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
    ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
    nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
    glNormal3f(nx, ny, nz);
}


void drawPlane()
{
    glColor3f(1, 0.7, 1);

    glBegin(GL_TRIANGLE_FAN);
        normal(-5, 0, 2, 0, 0, 0, -5, 0, -2); // top surface normal
        glVertex3f(0, 0, 0); // fan point
        glVertex3f(-5, 0, 2);
        glVertex3f(-5, 0, 1);
        glVertex3f(-5, 0, 0);
        glVertex3f(-5, 0, -1);
        glVertex3f(-5, 0, -2);
        // bottom surfaces
        normal(-5, 0, -2, 0, 0, 0, -5, -0.5, -0.5);
        glVertex3f(-5, -0.5, -0.5);
        normal(-5, 0, -2, -5, -0.5, -0.5, -5, -1, 0);
        glVertex3f(-5, -1, 0);
        normal(-5, -0.5, -0.5, -5, -1, 0, -5, -0.5, 0.5);
        glVertex3f(-5, -0.5, 0.5);
        normal(-5, -1, 0, -5, -0.5, 0.5, -5, 0, 2);
        glVertex3f(-5, 0, 2);
    glEnd();

    glBegin(GL_TRIANGLE_FAN); // back surface
        normal(-5, 0, 2, -5, 0, -2, -5, -1, 0);
        glVertex3f(-5, 0, 0); // fan point
        glVertex3f(-5, 0, 2);
        glVertex3f(-5, -0.5, 0.5);
        glVertex3f(-5, -1, 0);
        glVertex3f(-5, -0.5, -0.5);
        glVertex3f(-5, 0, -2);
        glVertex3f(-5, 0, 2);
    glEnd();
}


void drawBowl() {
    glColor3f(0.5, 0.5, 0.5);

    float prev_radius = 0;
    float radius;// = prev_radius * prev_radius;
    float vx, vy, vz;
    float wx, wy, wz;
    float prev_vx, prev_vy, prev_vz;
    float prev_wx, prev_wy, prev_wz;

    for (int level = 0; level < 30; level++) {

        vy = level * 0.1;
        radius = sqrt(vy) * 2;
        wy = level * 0.1 + 0.1;

        glBegin(GL_TRIANGLE_STRIP);
        for (int rev = 0; rev <= 36; rev++) {
            vx = sin(3.14159 * 2 * (float)rev / 36) * prev_radius;
            vz = cos(3.14159 * 2 * (float)rev / 36) * prev_radius;
            wx = sin(3.14159 * 2 * (float)rev / 36) * radius;
            wz = cos(3.14159 * 2 * (float)rev / 36) * radius;
            if (rev > 0) {
                normal(vx, vy, vz, prev_wx, prev_wy, prev_wz, prev_vx, prev_vy, prev_vz);
            }
            glVertex3f(vx, vy, vz);
            if (rev > 0) {
                normal(prev_wx, prev_wy, prev_wz, prev_vx, prev_vy, prev_vz, wx, wy, wz);
            }
            glVertex3f(wx, wy, wz);
            prev_vx = vx;
            prev_vy = vy;
            prev_vz = vz;
            prev_wx = wx;
            prev_wy = wy;
            prev_wz = wz;
        }
        glEnd();

        prev_radius = radius;
    }

    //// TOP
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[8]);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0, wy, 0);
    for (int rev = 0; rev <= 36; rev++) {
        float x = sin(3.14159 * 2 * (float)rev / 36);
        float z = cos(3.14159 * 2 * (float)rev / 36);
        glTexCoord2f(x * 0.5 + 0.5, z * 0.5 + 0.5);
        glVertex3f(
                x * radius,
                wy,
                z * radius
        );
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


void drawMuseum()
{
    glColor3f(0.8, 0.7, 0.3);   //replace with a texture
    float hex_half_side_len = 10.0;
    float hex_wall_dist = hex_half_side_len * tan(1.0472);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[0]);

    glPushMatrix();

    for (int i = 0; i < 6; i++) {
        ////////////////////// SPIRE ///////////////////////
        glBegin(GL_TRIANGLES);
        normal(-hex_half_side_len, 7, hex_wall_dist,
               hex_half_side_len, 7, hex_wall_dist,
               0, 14, 0);
        glTexCoord2f(0, 0);         glVertex3f(-hex_half_side_len, 7, hex_wall_dist);
        glTexCoord2f(2, 0);         glVertex3f(hex_half_side_len, 7, hex_wall_dist);
        glTexCoord2f(1, 2);         glVertex3f(0, 14, 0);
        glEnd();
        glRotatef(60, 0, 1, 0);
    }

    glBindTexture(GL_TEXTURE_2D, texId[7]);
    for (int i = 0; i < 5; i++) {
        glRotatef(60, 0, 1, 0);
        ////////////////////// WALLS ///////////////////////
        glBegin(GL_QUADS);
//        normal(-hex_half_side_len, 7, hex_wall_dist,
//               -hex_half_side_len, 0, hex_wall_dist,
//               hex_half_side_len, 0, hex_wall_dist);
        glTexCoord2f(0, 0);         glVertex3f(hex_half_side_len, 7, hex_wall_dist);
        glTexCoord2f(0, 4);         glVertex3f(hex_half_side_len, 0, hex_wall_dist);
        glTexCoord2f(4, 4);         glVertex3f(-hex_half_side_len, 0, hex_wall_dist);
        glTexCoord2f(4, 0);         glVertex3f(-hex_half_side_len, 7, hex_wall_dist);
        glEnd();
    }

    glRotatef(60, 0, 1, 0);
    ////////////////////// DOOR SIDE 1 ///////////////////////
    glBegin(GL_QUADS);
    glTexCoord2f(0, 2);         glVertex3f(-hex_half_side_len, 7, hex_wall_dist);
    glTexCoord2f(0, 0);         glVertex3f(-hex_half_side_len, 0, hex_wall_dist);
    glTexCoord2f(2, 0);         glVertex3f(-hex_half_side_len / 2.5, 0, hex_wall_dist);
    glTexCoord2f(2, 2);         glVertex3f(-hex_half_side_len / 2.5, 7, hex_wall_dist);
    glEnd();

    ////////////////////// DOOR SIDE 2 ///////////////////////
    glBegin(GL_QUADS);
    glTexCoord2f(0, 2);         glVertex3f(hex_half_side_len, 7, hex_wall_dist);
    glTexCoord2f(0, 0);         glVertex3f(hex_half_side_len, 0, hex_wall_dist);
    glTexCoord2f(2, 0);         glVertex3f(hex_half_side_len / 2.5, 0, hex_wall_dist);
    glTexCoord2f(2, 2);         glVertex3f(hex_half_side_len / 2.5, 7, hex_wall_dist);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}


void drawTable(float height, float length, float depth, float wood_width) {
    glColor3f(0.5, 0.4, 0.2);

    glPushMatrix();
    glTranslatef(0, height, 0);
    glScalef(length, wood_width, depth);
    glutSolidCube(1);
    glPopMatrix();

    float leg_x = length / 2.0 - wood_width / 2.;
    float leg_z = depth / 2.0 - wood_width / 2.;
    for (int i = 0; i < 4; i++) {
        if (i % 2 == 0) {
            leg_x *= -1;
        } else {
            leg_z *= -1;
        }
        glPushMatrix();
        glTranslatef(leg_x, height / 2.0, leg_z);
        glScalef(wood_width, height, wood_width);
        glutSolidCube(1);
        glPopMatrix();
    }
}


void drawGlass() {
    glColor3f(0.0, 1.0, 1.0);

    glPushMatrix();
    glTranslatef(0, 2, 0);
    glRotatef(90, 1, 0, 0);
    glutSolidCone(1,1, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0.8, 0);
    glScalef(1, 6, 1);
    glutSolidCube(0.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(0.7,0.3, 20, 10);
    glPopMatrix();
}


#define TEAPOT_ANIM_STEP 0.05
#define TEAPOT_ANIM_TOTAL_FRAMES 90
float teapot_pitch = 0;
float teapot_yaw = 0;
void tipTeapot(int frame) {
    frame = frame % TEAPOT_ANIM_TOTAL_FRAMES;
    int blockNum = frame / (TEAPOT_ANIM_TOTAL_FRAMES / 5);
    if (blockNum < 1) {
        teapot_pitch += 1;
    } else if (blockNum < 3) {
        teapot_pitch -= 2;
    } else if (blockNum < 4) {
        teapot_pitch += 3;
    } else {
        teapot_yaw += 180.0 / (float)(TEAPOT_ANIM_TOTAL_FRAMES / 5);
    }
    glutPostRedisplay();
    updateParticles();
    glutTimerFunc(TEAPOT_ANIM_STEP, tipTeapot, frame + 1);
}

float plane_rot = 0;
float plane_height = 4;
bool crashing = false;
void flyPlane(int value) {
    if (!crashing) {
        plane_height = 4 + sin(plane_rot * 0.01745329252 * 4.0) * 0.5;
    } else {
        plane_height -= 0.025;
    }
    if (plane_height < 0.2) {  // plane crashed
        plane_height = 0.2;
        return; // stop callbacks
    }
    plane_rot += 1.5;
    glutTimerFunc(0.05, flyPlane, 0);
}


//--Special keyboard event callback function ---------
void special(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        cam_pos_x += cam_front_x;
        cam_pos_z += cam_front_z;
    } else if (key == GLUT_KEY_DOWN) {
        cam_pos_x -= cam_front_x;
        cam_pos_z -= cam_front_z;
    } else if (key == GLUT_KEY_LEFT) {
        cam_rot -= 0.05f;
        cam_front_x = sin(cam_rot);
        cam_front_z = -cos(cam_rot);
    } else if (key == GLUT_KEY_RIGHT) {
        cam_rot += 0.05f;
        cam_front_x = sin(cam_rot);
        cam_front_z = -cos(cam_rot);
    }
//    if (cam_height > 20) cam_height = 20;
//    else if (cam_height < 2) cam_height = 2;

//    glutPostRedisplay();
}

float player_v_vert = 0;
float player_y = 0;
void calculateJumpY(int delta_t_ms) {
    double delta_t_s = (delta_t_ms / 1000.);
    if (player_y <= 0) {
        player_y = 0;
        player_v_vert = 0;
        return;
    }

    // stops player_y from getting too high if the last frame was a long time ago
    delta_t_s = std::min(delta_t_s, 0.05);
    player_y += player_v_vert * delta_t_s;
    player_v_vert = player_v_vert - 9.8f * delta_t_s;
//    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {
    if (key == ' ' && player_y <= 0) {
        player_v_vert = 10;
        player_y = 0.1;
        calculateJumpY(0);
//        glutPostRedisplay();
    } else if (key == 'x') {
        crashing = true;
    }
}


float oldTimeSinceStart = 0;
//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display(void)
{
    float hex_half_side_len = 10.0;
    float hex_wall_dist = hex_half_side_len * tan(1.0472);
    float lpos[4] = {10., 5., 0., 1.0};  //light's position

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    int deltaTime = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;
    calculateJumpY(deltaTime);

    gluLookAt(cam_pos_x, CAM_HEIGHT + player_y, cam_pos_z, cam_pos_x + cam_front_x, CAM_HEIGHT + player_y, cam_pos_z + cam_front_z, 0, 1, 0);  //Camera position and orientation

    glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //Set light position

    //// SCENE
    drawSkybox();
    drawFloor();
    drawMuseum();
    drawTrees();

    //// PARABOLIC POOL
    glPushMatrix();
    glTranslatef(0, 0, 3);
    glScalef(0.5, 0.5, 0.5);
    drawBowl();
    glPopMatrix();

    //// PAPER PLANE
    glPushMatrix();
    glRotatef(plane_rot, 0, 1, 0);
    glTranslatef(0, plane_height, hex_wall_dist - 1.6);
    glScalef(0.7, 0.7, 0.7);
    drawPlane();
    if (crashing) {
        glTranslatef(-2.5, 0., 0.);
        glScalef(0.5, 0.5, 0.5);
        drawParticles();
    }
    glPopMatrix();

    glPushMatrix();
    //// TABLE AND TEAPOT START
    glColor3f(1.0, 0.5, 0.5);
    glTranslatef(0, 0, -8);

    glPushMatrix();
    glTranslatef(0, 4, 0); // height of teapot
    glRotatef(teapot_yaw, 0, 1, 0);
    glRotatef(teapot_pitch, 0, 0, 1);
    glTranslatef(0, 0.8, 0); // sets axis of rotation
    glutSolidTeapot(0.7);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2, 2.25, 0);
    glScalef(0.5, 0.5, 0.5);
    drawGlass();
    glTranslatef(-8, 0, 0);
    drawGlass();
    glPopMatrix();
    drawTable(2, 5, 3, 0.3);
    //// TABLE AND TEAPOT END
    glPopMatrix();

    glFlush();
}


//----------------------------------------------------------------------
void initialize(void)
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    createParticles();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour


    glEnable(GL_LIGHTING);		//Enable OpenGL states
    glEnable(GL_LIGHT0);

    //	Define light's ambient, diffuse, specular properties
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    loadTexture();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 1000.0);   //Camera Frustum
}


//  ------- Main: Initialize glut window and register call backs -------
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("jsv22 Assignment 1");
    initialize();
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(TEAPOT_ANIM_STEP, tipTeapot, 0);
    glutTimerFunc(0.05, flyPlane, 0);

    glutMainLoop();
    return 0;
}

