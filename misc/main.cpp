//
// Created by Jack Breurkes on 8/03/20.
//

#include <cmath>
//#include <GL/freeglut.h>
#include <GLUT/glut.h>

#define CAM_PAN_UNITS 12

int cam_height = 10;
float theta = 0;


//--Draws a grid of lines on the floor plane -------------------------------
void drawFloor()
{
    glColor3f(0., 0.5,  0.);			//Floor colour

    for(int i = 0; i <= 0; i ++)
    {
        glBegin(GL_LINES);			//A set of grid lines on the xz-plane
        glVertex3f(-50, 0, i);
        glVertex3f(50, 0, i);
        glVertex3f(i, 0, -50);
        glVertex3f(i, 0, 50);
        glEnd();
    }
}

//--Special keyboard event callback function ---------
void special(int key, int x, int y) {
    if (key == GLUT_KEY_UP) cam_height++;
    else if (key == GLUT_KEY_DOWN) cam_height--;

    if (cam_height > 20) cam_height = 20;
    else if (cam_height < 2) cam_height = 2;

    glutPostRedisplay();
}

//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display(void)
{
    float lpos[4] = {0., 10., 10., 1.0};  //light's position

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, cam_height, 10,  0, 0, 0,  0, 1, 0);  //Camera position and orientation

    glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position

    glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
    drawFloor();

    glEnable(GL_LIGHTING);			//Enable lighting when drawing the teapot
    glColor3f(0.0, 1.0, 1.0);


    glPushMatrix();
    glTranslatef(2.0, 1.0, -3.0);
    glutSolidCube(1.0);
    glPopMatrix();
    glRotatef(30.0, 0.0, 1.0, 0.0);
    glutSolidTeapot(0.8);

    glFlush();
}

//----------------------------------------------------------------------
void initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_LIGHTING);		//Enable OpenGL states
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);   //Camera Frustum
}


//  ------- Main: Initialize glut window and register call backs -------
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Teapot");
    initialize();
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutMainLoop();
    return 0;
}

