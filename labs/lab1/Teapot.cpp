//  ========================================================================
//  COSC363: Computer Graphics (2020);  University of Canterbury.
//
//  FILE NAME: Teapot.cpp
//  See Lab01.pdf for details
//  ========================================================================

#include <cmath>
#include <GL/freeglut.h>
//#include <GLUT/glut.h>

#define CAM_PAN_UNITS 12

int cam_height = 10;
float theta = 0;


//--Draws a grid of lines on the floor plane -------------------------------
void drawFloor()
{
	glColor3f(0., 0.5,  0.);			//Floor colour

    for(int i = -50; i <= 50; i ++)
	{
        glBegin(GL_LINES);			//A set of grid lines on the xz-plane
            glVertex3f(-50, -0.75, i);
            glVertex3f(50, -0.75, i);
            glVertex3f(i, -0.75, -50);
			glVertex3f(i, -0.75, 50);
		glEnd();
	}
}

void myTimer(int value) {
    theta += 0.05;
    glutPostRedisplay();
    glutTimerFunc(50, myTimer, 0); // 50 ms
}

//--Special keyboard event callback function ---------
void special(int key, int x, int y) {
    if (key == GLUT_KEY_UP) cam_height++;
    else if (key == GLUT_KEY_DOWN) cam_height--;

//    if (key == GLUT_KEY_LEFT) cam_pan_pos--;
//    else if (key == GLUT_KEY_RIGHT) cam_pan_pos++;

    if (cam_height > 20) cam_height = 20;
    else if (cam_height < 2) cam_height = 2;

//    cam_pan_pos = cam_pan_pos % 360;

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

    gluLookAt(12*sin(theta), cam_height, 12*cos(theta),  0, 0, 0,  0, 1, 0);  //Camera position and orientation

	glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position

	glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
    drawFloor();

	glEnable(GL_LIGHTING);			//Enable lighting when drawing the teapot
    glColor3f(0.0, 1.0, 1.0);

    if (cam_height % 2 == 0) {
        glutSolidTeapot(1.0);
    } else {
        glutSolidSphere(1, 40, 40);
    }

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
    glutTimerFunc(50, myTimer, 0);
	glutMainLoop();
	return 0; 
}

