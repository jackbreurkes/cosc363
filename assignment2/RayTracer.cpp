/*==================================================================================
* COSC 363  Computer Graphics (2020)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab07.pdf, Lab08.pdf for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "Cylinder.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Plane.h"
#include "TextureBMP.h"
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/freeglut.h"
#endif
using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 600;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;
TextureBMP texture;

vector<SceneObject*> sceneObjects;

const int CHECKERED_PLANE_INDEX = 0;


// TODO spotlight


//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
	glm::vec3 lightPos(10, 40, -3);					//Light's position
	glm::vec3 color(0);
	SceneObject* obj;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found

    if (ray.index == CHECKERED_PLANE_INDEX) { // draw checkered plane
        int stripeWidth = 5;
        int iz = (ray.hit.z) / stripeWidth;
        int ix = (ray.hit.x + 1000) / stripeWidth; // add 1000 so that ix is positive in this render
        int k = (iz + ix) % 2; // 2 colours
        switch (k) {
            case 0:
                color = glm::vec3(0, 1, 0);
                break;
            default:
                color = glm::vec3(1, 1, 0.5);
                break;
        }
        obj->setColor(color);
    }

	color = obj->lighting(lightPos, -ray.dir, ray.hit);	 // Object's colour
	glm::vec3 lightVec = lightPos - ray.hit;
	Ray shadowRay(ray.hit, lightVec);

	shadowRay.closestPt(sceneObjects);
	if (shadowRay.index != -1 && shadowRay.dist < glm::length(lightVec)) {
	    float shadowScalar = 0.2f;
	    if (sceneObjects[shadowRay.index]->isTransparent()) { // TODO if object is placed at the back, then shadow is brighter??
	        shadowScalar = 0.7f;
	    }
	    color = glm::vec3(shadowScalar) * obj->getColor();
	}

	if (obj->isReflective() && step < MAX_STEPS) {
	    float rho = obj->getReflectionCoeff();
	    glm::vec3 normalVec = obj->normal(ray.hit);
	    glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
	    Ray reflectedRay(ray.hit, reflectedDir);
	    glm::vec3 reflectedColor = trace(reflectedRay, step + 1); // reflection recursive call
	    color = color + (rho * reflectedColor);
	}

	if (obj->isTransparent() && step < MAX_STEPS) { // TODO tutor help - weird spots at bottom of sphere; colouring the sphere; specular transparent objects?
	    Ray transparencyRay(ray.hit, ray.dir);
	    if (glm::dot(obj->normal(ray.hit), ray.dir) < 0) { // ray is going into the object
	        color *= trace(transparencyRay, step + 1);
	    } else { // ray out of the object
	        color = trace(transparencyRay, step + 1);
	    }
	}

	if (obj->isRefractive() && step < MAX_STEPS) {
	    float eta = 1.0f / obj->getRefractiveIndex(); // refractive index of air is 1.0
	    glm::vec3 normal = obj->normal(ray.hit);
	    if (glm::dot(normal, ray.dir) < 0) { // ray is going into object
	        glm::vec3 g = glm::refract(ray.dir, normal, eta);
	        Ray refractionRay(ray.hit, g);
	        color = trace(refractionRay, step + 1); // TODO doesn't care about old colour?
	    } else { // ray is coming out of object
	        glm::vec3 h = glm::refract(ray.dir, -normal, 1.0f/eta);
	        Ray refractionRay(ray.hit, h);
	        color = trace(refractionRay, step + 1);
	    }
	}

	return color;
}


glm::vec3 getColorNoAntiAliasing(float xp, float yp, float cellX, float cellY, glm::vec3 eye) {
    glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);	//direction of the primary ray
    Ray ray = Ray(eye, dir);
    glm::vec3 col = trace (ray, 1); //Trace the primary ray and get the colour value
    return col;
}

glm::vec3 getColorWithAntiAliasing(float xp, float yp, float cellX, float cellY, glm::vec3 eye) {
        glm::vec3 dir1(xp+0.25*cellX, yp+0.25*cellY, -EDIST);
        glm::vec3 dir2(xp+0.25*cellX, yp+0.75*cellY, -EDIST);
        glm::vec3 dir3(xp+0.75*cellX, yp+0.25*cellY, -EDIST);
        glm::vec3 dir4(xp+0.75*cellX, yp+0.75*cellY, -EDIST);

        Ray ray1 = Ray(eye, dir1);
        Ray ray2 = Ray(eye, dir2);
        Ray ray3 = Ray(eye, dir3);
        Ray ray4 = Ray(eye, dir4);

        glm::vec3 col1 = trace (ray1, 1);
        glm::vec3 col2 = trace (ray2, 1);
        glm::vec3 col3 = trace (ray3, 1);
        glm::vec3 col4 = trace (ray4, 1);

        float avgRed = (col1.r + col2.r + col3.r + col4.r) / 4.0f;
        float avgGreen = (col1.g + col2.g + col3.g + col4.g) / 4.0f;
        float avgBlue = (col1.b + col2.b + col3.b + col4.b) / 4.0f;

        return glm::vec3(avgRed, avgGreen, avgBlue);
}


//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
	float cellY = (YMAX-YMIN)/NUMDIV;  //cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a tiny quad.

	for(int i = 0; i < NUMDIV; i++)	//Scan every cell of the image plane
	{
		xp = XMIN + i*cellX;
		for(int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j*cellY;

            glm::vec3 col = getColorNoAntiAliasing(xp, yp, cellX, cellY, eye); // TODO turn on anti-aliasing
//            glm::vec3 col = getColorWithAntiAliasing(xp, yp, cellX, cellY, eye);

            glColor3f(col.r, col.g, col.b);

			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp+cellX, yp);
			glVertex2f(xp+cellX, yp+cellY);
			glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}



void createCube(glm::vec3 center, float size=1, glm::vec3 color = glm::vec3(0.8, 0.2, 0.2)) {
    float halfSize = size / 2.0f;

    glm::vec3 bottomLeftFront(center.x - halfSize, center.y - halfSize, center.z + halfSize);
    glm::vec3 bottomLeftBack(center.x - halfSize, center.y - halfSize, center.z - halfSize);
    glm::vec3 bottomRightFront(center.x + halfSize, center.y - halfSize, center.z + halfSize);
    glm::vec3 bottomRightBack(center.x + halfSize, center.y - halfSize, center.z - halfSize);
    glm::vec3 topLeftFront(center.x - halfSize, center.y + halfSize, center.z + halfSize);
    glm::vec3 topLeftBack(center.x - halfSize, center.y + halfSize, center.z - halfSize);
    glm::vec3 topRightFront(center.x + halfSize, center.y + halfSize, center.z + halfSize);
    glm::vec3 topRightBack(center.x + halfSize, center.y + halfSize, center.z - halfSize);

    Plane *top = new Plane (topLeftFront, topRightFront, topRightBack, topLeftBack);
    Plane *bottom = new Plane (bottomLeftBack, bottomRightBack, bottomRightFront, bottomLeftFront);
    Plane *left = new Plane (bottomLeftBack, bottomLeftFront, topLeftFront, topLeftBack);
    Plane *right = new Plane (topRightBack, topRightFront, bottomRightFront, bottomRightBack);
    Plane *back = new Plane (topLeftBack, topRightBack, bottomRightBack, bottomLeftBack);
    Plane *front = new Plane (bottomLeftFront, bottomRightFront, topRightFront, topLeftFront);


    top->setColor(color);
    bottom->setColor(color);
    left->setColor(color);
    right->setColor(color);
    back->setColor(color);
    front->setColor(color);

//    top->setReflectivity(reflectivity);
//    bottom->setReflectivity(reflectivity);
//    left->setReflectivity(reflectivity);
//    right->setReflectivity(reflectivity);
//    back->setReflectivity(reflectivity);
//    front->setReflectivity(reflectivity);
//
//    top->setSpecularity(specularity);
//    bottom->setSpecularity(specularity);
//    left->setSpecularity(specularity);
//    right->setSpecularity(specularity);
//    back->setSpecularity(specularity);
//    front->setSpecularity(specularity);

    sceneObjects.push_back(top);
    sceneObjects.push_back(bottom);
    sceneObjects.push_back(left);
    sceneObjects.push_back(right);
    sceneObjects.push_back(back);
    sceneObjects.push_back(front);
}



//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographic projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    texture = TextureBMP("Butterfly.bmp");

    glClearColor(0, 0, 0, 1);

    Plane *plane = new Plane (glm::vec3(-20., -15, -40), glm::vec3(20., -15, -40),
                              glm::vec3(20., -15, -200), glm::vec3(-20., -15, -200));
    plane->setColor(glm::vec3(0.8, 0.8, 0));
    plane->setSpecularity(false);
    sceneObjects.push_back(plane); // index 0

    createCube(glm::vec3(-10., -10., -60.), 4.);

    Cylinder *cylinder1 = new Cylinder(glm::vec3(10.0, -15.0, -60.0), 2.5, 5.0); // TODO 1 mark
    cylinder1->setColor(glm::vec3(0, 0, 1));   // Set colour to blue
    cylinder1->setReflectivity(true, 0.8);
    sceneObjects.push_back(cylinder1);

	Sphere *sphere1 = new Sphere(glm::vec3(0.0, 0.0, -120.0), 15.0);
	sphere1->setColor(glm::vec3(1, 1, 0));
	sphere1->setSpecularity(true);
	sphere1->setShininess(10.0f);
	sphere1->setTransparency(true);
	sceneObjects.push_back(sphere1);


    Sphere *sphere2 = new Sphere(glm::vec3(0.0, -5.0, -50), 4.0);
    sphere2->setColor(glm::vec3(1, 1, 0));
    sphere2->setRefractivity(true, 1.0f, 1.01f); // TODO what is diff between refraction coefficient and refractive index?? coefficient not used??
    sceneObjects.push_back(sphere2);

    Sphere *sphere3 = new Sphere(glm::vec3(5.0, -10.0, -60), 5.0);
    sphere3->setColor(glm::vec3(1,1,1));
    sphere3->setSpecularity(false);
//    sceneObjects.push_back(sphere3);

    Sphere *sphere4 = new Sphere(glm::vec3(10.0, 10.0, -60.0), 3.0);
    sphere4->setColor(glm::vec3(1, 0, 0));
//    sceneObjects.push_back(sphere4);

}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
