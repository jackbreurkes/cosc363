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
#include "Cone.h"
#include "Torus.h"
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
const int ZOOM = 1;
const bool AA = false; // TODO turn on
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;
TextureBMP texture;

vector<SceneObject*> sceneObjects;

const int FLOOR_INDEX = 0;
const int BACK_INDEX = 1;
const int CYLINDER_INDEX = 2;
const float CYLINDER_HEIGHT = 7.0;
const float CYLINDER_CENTER_Y = -15.0;





//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0, 0, 0.7);
    glm::vec3 mainLightPos(40, 100, -80);
    glm::vec3 spotlightPos(-30, 40, 100);
	glm::vec3 spotlightDirection = glm::vec3(0, -15, -100) - spotlightPos;
	float spotlightCutoff = 5.0 * (3.14 / 180.0);
	spotlightDirection = glm::normalize(spotlightDirection);
	glm::vec3 fullColor(0);
	SceneObject* obj;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found

    if (ray.index == FLOOR_INDEX) { // draw checkered plane
        int squareSize = 5;
        int iz = (ray.hit.z) / squareSize;
        int ix = (ray.hit.x + 1000) / squareSize; // add 1000 so that ix is positive in this render
        int k = (iz + ix) % 2; // 2 colours
        glm::vec3 floorColor(0);
        switch (k) {
            case 0:
                floorColor = glm::vec3(0, 1, 0);
                break;
            default:
                floorColor = glm::vec3(1, 1, 0.5);
                break;
        }
        obj->setColor(floorColor);
    }

    if (ray.index == BACK_INDEX) { // draw checkered plane
        int squareSize = 2;
        int ix = (ray.hit.x + 1000.) / squareSize; // add 1000 so that ix is positive in this render
        int iy = (ray.hit.y + 1000.) / squareSize; // add 1000 so that iy is positive in this render
        int k = (ix + iy) % 2; // 2 colours
        glm::vec3 backColor(0);
        switch (k) {
            case 0:
                backColor = glm::vec3(1, 0.2, 0.2);
                break;
            default:
                backColor = glm::vec3(0.2, 0.2, 1);
                break;
        }
        obj->setColor(backColor);
    }

    if (ray.index == CYLINDER_INDEX) { // texture the cylinder
        float texcoords = std::acos(glm::dot(glm::vec3(0, 0, 1), obj->normal(ray.hit))) / (2.0 * 3.14);
        if (ray.hit.x < 0) {
            texcoords = 1.0f - texcoords;
        }
        float rows = 2.0;
        float texcoordt = fmod((ray.hit.y - CYLINDER_CENTER_Y) / (CYLINDER_HEIGHT / rows), 1);
        obj->setColor(texture.getColorAt(texcoords, texcoordt));
    }

    std::vector<glm::vec3> lights;
    lights.push_back(mainLightPos);
    lights.push_back(spotlightPos);
    std::vector<float> lightScalars;

    float lightDistanceSum = glm::distance(ray.hit, mainLightPos) + glm::distance(ray.hit, spotlightPos);
    lightScalars.push_back( 1.0 - glm::distance(ray.hit, mainLightPos) / lightDistanceSum );
    lightScalars.push_back( 1.0 - glm::distance(ray.hit, spotlightPos) / lightDistanceSum );


    for (int i = 0; i < 2; i++) {


        glm::vec3 lightPos = lights[i];
        glm::vec3 color = glm::vec3(0);

        color = obj->lighting(lightPos, -ray.dir, ray.hit);     // Object's colour
        glm::vec3 lightVec = lightPos - ray.hit;
        Ray shadowRay(ray.hit, lightVec);



        shadowRay.closestPt(sceneObjects);
        if (shadowRay.index != -1 && shadowRay.dist < glm::length(lightVec)) { // in shadow
            float shadowScalar = 0.2f;
            if (sceneObjects[shadowRay.index]->isTransparent() || sceneObjects[shadowRay.index]->isRefractive()) {
                shadowScalar = 0.7f;
            }
            color = glm::vec3(shadowScalar) * color;
        } else {
            if (i == 1) { // spotlight light index
                if (glm::dot(spotlightDirection, glm::normalize(-lightVec)) < glm::cos(spotlightCutoff)) {
                    color = glm::vec3(0.2f) * color;
                }
            }
        }

        if (obj->isReflective() && step < MAX_STEPS) {
            float rho = obj->getReflectionCoeff();
            glm::vec3 normalVec = obj->normal(ray.hit);
            glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
            Ray reflectedRay(ray.hit, reflectedDir);
            glm::vec3 reflectedColor = trace(reflectedRay, step + 1); // reflection recursive call
            color = color + (rho * reflectedColor);
        }

        if (obj->isTransparent() && step < MAX_STEPS) {
            Ray incidentRay(ray.hit, ray.dir);
            incidentRay.closestPt(sceneObjects);
            Ray exitingRay(incidentRay.hit, incidentRay.dir);
            color += trace(exitingRay, step + 1); // TODO spotty ring around object?
        }

        if (obj->isRefractive() && step < MAX_STEPS) {
            float eta = 1.0f / obj->getRefractiveIndex(); // refractive index of air is 1.0

            // interior ray
            glm::vec3 normal = obj->normal(ray.hit);
            glm::vec3 g = glm::refract(ray.dir, normal, eta);
            Ray interiorRay(ray.hit, g);
            interiorRay.closestPt(sceneObjects);

            // exiting ray
            normal = obj->normal(interiorRay.hit);
            glm::vec3 h = glm::refract(interiorRay.dir, -normal, 1.0f / eta);
            Ray exitingRay(interiorRay.hit, h);
            color += trace(exitingRay, step + 1);
        }

        fullColor += color * lightScalars[i];

    }

	return fullColor;
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

			glm::vec3 col;
            if (AA) {
                col = getColorWithAntiAliasing(xp, yp, cellX, cellY, eye);
            } else {
                col = getColorNoAntiAliasing(xp, yp, cellX, cellY, eye);
            }

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

    texture = TextureBMP("Wood.bmp");

    glClearColor(0, 0, 0, 1);

    Plane *floor = new Plane (glm::vec3(-20., -15, -40), glm::vec3(20., -15, -40),
                              glm::vec3(20., -15, -200), glm::vec3(-20., -15, -200));
    floor->setColor(glm::vec3(0.8, 0.8, 0));
    floor->setSpecularity(false);
    sceneObjects.push_back(floor); // index 0

    Plane *back = new Plane (glm::vec3(-20., -15, -200), glm::vec3(20., -15, -200),
            glm::vec3(20., 15, -200), glm::vec3(-20., 15, -200));
    back->setColor(glm::vec3(1, 0.2, 0));
    back->setSpecularity(false);
    sceneObjects.push_back(back); // index 1

    Cylinder *cylinder1 = new Cylinder(glm::vec3(0, CYLINDER_CENTER_Y, -80.0), 2, CYLINDER_HEIGHT);
    cylinder1->setColor(glm::vec3(0, 0, 1));   // Set colour to blue
//    cylinder1->setReflectivity(true, 0.3);
//    cylinder1->setSpecularity(true);
//    cylinder1->setShininess(10.0f);
    sceneObjects.push_back(cylinder1); // index 2

    //    Plane *roof = new Plane (glm::vec3(-50., 40, -200), glm::vec3(50., 40, -200),
//            glm::vec3(50., 40, -40), glm::vec3(-50., 40, -40));
//    roof->setColor(glm::vec3(0.8, 0.8, 0));
//    roof->setSpecularity(false);
//    sceneObjects.push_back(roof); // index 3


    Cone *cone1 = new Cone(glm::vec3(-5, -15, -70), 1, 4);
    cone1->setColor(glm::vec3(0.9, 0.6, 0.6));
    sceneObjects.push_back(cone1);
//    createCube(glm::vec3(-5., -10., -70.), 2.);

    Cone *cone2 = new Cone(glm::vec3(5, -15, -70), 1, 4);
    cone2->setColor(glm::vec3(0.9, 0.6, 0.6));
    sceneObjects.push_back(cone2);
    createCube(glm::vec3(5., -10., -70.), 2.);

    Cone *cone3 = new Cone(glm::vec3(10, -15, -90), 1.5, 7);
    cone3->setColor(glm::vec3(0.9, 0.6, 0.6));
    sceneObjects.push_back(cone3);
    createCube(glm::vec3(10., -6., -90.), 4.);

    Cone *cone4 = new Cone(glm::vec3(-10, -15, -90), 1.5, 7);
    cone4->setColor(glm::vec3(0.9, 0.6, 0.6));
    sceneObjects.push_back(cone4);
    createCube(glm::vec3(-10., -6., -90.), 4.);


	Sphere *sphere1 = new Sphere(glm::vec3(-5.0, -9.5, -70.0), 1.5); // transparent sphere
	sphere1->setColor(glm::vec3(0.2, 0.2, 0.2));
	sphere1->setSpecularity(true);
	sphere1->setShininess(10.0f);
	sphere1->setTransparency(true);
    sphere1->setReflectivity(true, 0.1f); // transparent objects should be reflective
	sceneObjects.push_back(sphere1);

    Plane *backLeft = new Plane (glm::vec3(-40., -15, -197), glm::vec3(-20., -15, -200),
                                 glm::vec3(-20., 15, -200), glm::vec3(-40., 15, -197));
    backLeft->setColor(glm::vec3(0.1));
    backLeft->setSpecularity(false);
    backLeft->setReflectivity(true, 1.0);
    sceneObjects.push_back(backLeft);

    Plane *backRight = new Plane (glm::vec3(40., 15, -197), glm::vec3(20., 15, -200),
    glm::vec3(20., -15, -200), glm::vec3(40., -15, -197));
    backRight->setColor(glm::vec3(0.1));
    backRight->setSpecularity(false);
    backRight->setReflectivity(true, 1.0);
    sceneObjects.push_back(backRight);

	Torus *torus1 = new Torus(glm::vec3(0, -0, -1), 2, 0.1);
	torus1->setColor(glm::vec3(1));
	torus1->setSpecularity(false);
	torus1->setReflectivity(false);
//	sceneObjects.push_back(torus1);

    Sphere *sphere2 = new Sphere(glm::vec3(0, -5.0, -80), 3.0); // refractive sphere
    sphere2->setColor(glm::vec3(0, 0, 0));
    sphere2->setRefractivity(true, 1.0, 1.01);
    sphere2->setReflectivity(true, 0.1); // refractive objects should be reflective
    sceneObjects.push_back(sphere2);


    Sphere *sphere3 = new Sphere(glm::vec3(-5.0, -5.0, -70), 5.0);
    sphere3->setColor(glm::vec3(1,1,1));
    sphere3->setSpecularity(false);
    sphere3->setReflectivity(true);
//    sceneObjects.push_back(sphere3);

    Sphere *sphere4 = new Sphere(glm::vec3(10.0, 10.0, -60.0), 3.0);
    sphere4->setColor(glm::vec3(1, 0, 0));
//    sceneObjects.push_back(sphere4);

}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(NUMDIV * ZOOM, NUMDIV * ZOOM);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
