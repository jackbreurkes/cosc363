//
// Created by Jack Breurkes on 8/03/20.
//

#include <cmath>
#include "loadBMP.h"
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/freeglut.h"
#endif

#define CAM_HEIGHT 5

float cam_pos_x = 0;
float cam_pos_z = 10;
float cam_front_x = 0;
float cam_front_z = -1;
float cam_rot = 0;

GLuint texId[6];


void loadTexture() {
    glGenTextures(7, texId); 	// Create texture ids

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
    loadBMP("brick.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
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


//--Draws a grid of lines on the floor plane -------------------------------
void drawFloor()
{
    glColor3f(0., 0.5,  0.);			//Floor colour

    for(int i = -50; i <= 50; i ++)
    {
        glBegin(GL_LINES);			//A set of grid lines on the xz-plane
        glVertex3f(-50, 0, i);
        glVertex3f(50, 0, i);
        glVertex3f(i, 0, -50);
        glVertex3f(i, 0, 50);
        glEnd();
    }
}


void drawPlane()
{
    glColor3f(1, 0.7, 1);   //replace with a texture

    ////////////////////// TOP ///////////////////////
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0); // begin point
    glVertex3f(-5, 0, 2);
    glVertex3f(-5, 0, -2);
    glVertex3f(-5, -0.5, -0.5);
    glVertex3f(-5, -1, 0);
    glVertex3f(-5, -0.5, 0.5);
    glVertex3f(-5, 0, 2);
    glEnd();

}

//--Function to compute the normal vector of a triangle with index indx ----------
void normal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    float nx, ny, nz;
    nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
    ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
    nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
    glNormal3f(nx, ny, nz);
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

    glBindTexture(GL_TEXTURE_2D, texId[6]);
    for (int i = 0; i < 5; i++) {
        ////////////////////// WALLS ///////////////////////
        glBegin(GL_QUADS);
        normal(-hex_half_side_len, 7, hex_wall_dist,
               -hex_half_side_len, 0, hex_wall_dist,
               hex_half_side_len, 0, hex_wall_dist);
        glTexCoord2f(0, 4);         glVertex3f(-hex_half_side_len, 7, hex_wall_dist);
        glTexCoord2f(0, 0);         glVertex3f(-hex_half_side_len, 0, hex_wall_dist);
        glTexCoord2f(4, 0);         glVertex3f(hex_half_side_len, 0, hex_wall_dist);
        glTexCoord2f(4, 4);         glVertex3f(hex_half_side_len, 7, hex_wall_dist);
        glEnd();
        glRotatef(60, 0, 1, 0);
    }

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
    glutTimerFunc(TEAPOT_ANIM_STEP, tipTeapot, frame + 1);
}

float plane_rot = 0;
float plane_height = 4;
void flyPlane(int value) {
    plane_rot += 1.5;
    plane_height = 4 + sin(plane_rot * 0.01745329252 * 4.0) * 0.5;
//    glutPostRedisplay();
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

    glutPostRedisplay();
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
    float lpos[4] = {0., 10., 10., 1.0};  //light's position

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    int deltaTime = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;
    calculateJumpY(deltaTime);

    gluLookAt(cam_pos_x, CAM_HEIGHT + player_y, cam_pos_z, cam_pos_x + cam_front_x, CAM_HEIGHT + player_y, cam_pos_z + cam_front_z, 0, 1, 0);  //Camera position and orientation

    glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position

    glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
    drawFloor();
    glEnable(GL_LIGHTING);

    drawMuseum();

    glPushMatrix();
    glRotatef(plane_rot, 0, 1, 0);
    glTranslatef(0, plane_height, hex_wall_dist - 0.5);
    drawPlane();
    glPopMatrix();
    glColor3f(0.0, 1.0, 1.0);


    glPushMatrix();
    glTranslatef(2.0, 1.0, -3.0);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 1.5, 0); // height of teapot
    glRotatef(teapot_yaw, 0, 1, 0);
    glRotatef(teapot_pitch, 0, 0, 1);
    glTranslatef(0, 0.8, 0); // sets axis of rotation
    glutSolidTeapot(0.8);
    glPopMatrix();

    drawSkybox();

    glFlush();
}

//----------------------------------------------------------------------
void initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_LIGHTING);		//Enable OpenGL states
    glEnable(GL_LIGHT0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
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

