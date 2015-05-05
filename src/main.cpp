#include "GL/gl.h"
#include "GL/glut.h"
#include "Vector3f.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include "Scene.h";
#include "Plane.h";
#include "Light.h";
#include "Sphere.h";
#include "Intersection.h"
#include "Ray.h"
#include <limits>

using namespace std;
using namespace ShapeObjects;
#define M_PI 3.14159265358979323846
void startRayTracing();
void drawImage();
Scene scene;
vector<Light> lights;
vector<Plane> planeArray;
vector<Sphere> sphereArray;
GLubyte *image;
Vector3f camdir;
Vector3f camright;
Vector3f campos;
Vector3f X(1, 0, 0);
Vector3f Y(0, 1, 0);
Vector3f Z(0, 0, 1);
/*
 * Help function to extract the numbers from the text file.
 * Works like "Split" in Java.
 */
vector<float> extractNumbers(char *line) {
	vector<float> a;
	string s = line;
	string forNow = "";
	for (unsigned int i = 0; i < s.size(); i++) {
		if (s[i] == ',') {
			stringstream ss;
			ss << forNow;
			float f;
			ss >> f;
			a.push_back(f);
			forNow = "";
		} else {
			if (s[i] != ' ') {
				forNow += s[i];
			}
		}
	}
	std::stringstream ss;
	ss << forNow;
	float f;
	ss >> f;
	a.push_back(f);
	forNow = "";
	return a;
}
/*
 * Main controler of the functions. Starts the ray tracing algorithm
 * and the image drawer algorithm.
 */
void startRay() {
	image = new GLubyte[scene.getResolutionX() * scene.getResolutionY() * 3];
	startRayTracing();
	drawImage();
}

/*
 * Main function.
 * - Reads from file
 * - Creates all objects and adds them to vectors
 * - Starts OpenGL engine
 * - Print Pro Tips and usage
 */
int main(int argc, char* argv[]) {
	ifstream sceneFile((argc > 1) ? argv[1] : "scene.txt");
	char output[512];
	int objectsFound = 0;
	while (sceneFile.is_open()) {
		while (!sceneFile.eof()) {
			sceneFile >> output;
			if (strcmp(output, "scene") == 0) {
				objectsFound++;
				sceneFile >> output;
				vector<float> params = extractNumbers(output);
				Vector3f cc(params[0], params[1], params[2]);
				Vector3f up(params[3], params[4], params[5]);
				float width = params[6];
				float rX = params[7];
				float rY = params[8];
				Vector3f amb(params[9], params[10], params[11]);
				scene = Scene(cc, up, amb, width, rX, rY);
			} else if (strcmp(output, "light") == 0) {
				objectsFound++;
				sceneFile >> output;
				extractNumbers(output);
				vector<float> params = extractNumbers(output);
				Vector3f ld(params[0], params[1], params[2]);
				Vector3f li(params[3], params[4], params[5]);
				if (params.size() == 10) {
					Vector3f lp(params[6], params[7], params[8]);
					GLfloat cut = params[9];
					bool spot = true;
					lights.push_back(Light(ld, li, lp, spot, cut));
				} else {
					Vector3f lp(0, 0, 0);
					lights.push_back(Light(ld, li, lp, false, 0));
				}
			} else if (strcmp(output, "spher") == 0) {
				objectsFound++;
				sceneFile >> output;
				extractNumbers(output);
				vector<float> params = extractNumbers(output);
				Vector3f c(params[0], params[1], params[2]);
				Vector3f kAv(params[3], params[4], params[5]);
				Vector3f kDv(params[6], params[7], params[8]);
				Vector3f kSv(params[9], params[10], params[11]);
				GLfloat r = params[12];
				GLfloat sh = params[13];
				sphereArray.push_back(Sphere(c, kAv, kDv, kSv, r, sh));
			} else if (strcmp(output, "plane") == 0) {
				objectsFound++;
				sceneFile >> output;
				extractNumbers(output);
				vector<float> params = extractNumbers(output);
				Vector3f n(params[0], params[1], params[2]);
				Vector3f c(params[3], params[4], params[5]);
				Vector3f kAv(params[6], params[7], params[8]);
				Vector3f kDv(params[9], params[10], params[11]);
				Vector3f kSv(params[12], params[13], params[14]);
				GLfloat w = params[15];
				GLfloat h = params[16];
				GLfloat s = params[17];
				planeArray.push_back(Plane(n, c, kAv, kDv, kSv, w, h, s));
			}
		}
		sceneFile.close();
	}
	cout
			<< "PRO HINT: You can change the file to read by calling this program with the file name as a parameter"
			<< endl;
	cout << "YOU CAN MOVE THE OBJECTS!" << endl;
	cout << "Enter the number of the object to move" << endl;
	cout << "Enter 'c' if you want to move a Sphere" << endl;
	cout << "Use 'w' and 's' to move the object up and down" << endl;
	cout << "Use 'a' and 'd' to move the object left and right" << endl;
	cout << "Use 'q' and 'e' to move the object nearer and farer" << endl;
	glutInit(&argc, argv);
	startRay();
	delete image;
	return 0;
}
/*
 * Constructing the ray through pixel. We receive the source of the ray and
 * the pixel (i,j) where the ray must go through it.
 * Returns an object Ray according to the algorithm shown in class
 */
Ray ConstructRayThroughPixel(Vector3f source, int i, int j) {
	GLfloat d = scene.getCenterCoordinates().distance(
			scene.getCenterCoordinates(), source);
	Vector3f Pc = source + scene.getCenterCoordinates() * d;
	Vector3f vRight = scene.getCenterCoordinates().crossProduct(
			scene.getUpVector(), scene.getCenterCoordinates());
	vRight.normalize();
	Vector3f UP = scene.getUpVector();
	Pc.normalize();
	GLfloat R = (float) scene.getScreenWidth() / (float) scene.getResolutionX();

	Vector3f P = Pc
			+ (float) (i - floor((float) scene.getResolutionX() / 2)) * R
					* vRight
			- (float) (j - floor((float) scene.getResolutionY() / 2)) * R * UP;

	P.normalize();
	Ray ans(source, P);
	return ans;
}
/*
 * This function receives a Ray and tries to find an intersection between that ray
 * and an object in the screen (Spheres or Planes)
 * This function returns an Intersection object
 */
Intersection FindIntersection(Ray ray) {
	GLfloat min_t = numeric_limits<int>::max();
	Sphere min_sphere;
	Plane min_plane;
	Vector3f poi;
	Vector3f poiTemp;
	bool fromPlane = false;
	bool found = false;
	for (unsigned int i = 0; i < sphereArray.size(); i++) {
		Sphere sp = sphereArray[i];
		GLfloat t = sp.intersect(scene, ray, poiTemp);
		if (t > 0) {
			if (t < min_t) {
				min_sphere = sp;
				min_t = t;
				found = true;
				poi = poiTemp;
			}
		}
	}
	for (unsigned int i = 0; i < planeArray.size(); i++) {
		Plane p = planeArray[i];
		GLfloat t = p.intersect(scene, ray, poiTemp);
		if (t > 0) {
			if (t < min_t) {
				fromPlane = true;
				min_plane = p;
				min_t = t;
				found = true;
				poi = poiTemp;
			}
		}
	}
	if (!found) {
		return Intersection(-1, min_sphere, poi);
	} else {
		if (fromPlane) {
			return Intersection(min_t, min_plane, poi);
		} else {
			return Intersection(min_t, min_sphere, poi);
		}
	}
}
/*
 * This function calculates the color for a Sphere in the scene.
 * It receives the Intersection hit, where the ray hit the Sphere and returns the
 * answer in the ans Vector3f.
 */
void sphereGetColor(Intersection hit, Vector3f& ans) {
	ans += hit.getSphere().getA() * scene.getAmbientLight();
	Vector3f hitPoint = hit.getPoi();
	Vector3f N = hit.getSphere().getNormal(hitPoint);
	N.normalize();
	Vector3f V = hitPoint - scene.getCamera();
	V.normalize();
	Vector3f L;
	for (unsigned int i = 0; i < lights.size(); i++) {
		Vector3f sum(0, 0, 0);
		Light toUse = lights[i];
		if (toUse.isSpotlight()) {
			L = toUse.getLightPosition() - hitPoint;
			L.makeNegative();
			L.normalize();
			Vector3f R = L - 2 * (L.dotProduct(L, N)) * N;
			R.normalize();
			sum += hit.getSphere().getD() * (N.dotProduct(N, L))
					+ hit.getSphere().getS()
							* (pow(V.dotProduct(V, R),
									hit.getSphere().getShine()));

			Ray r(toUse.getLightPosition(), L); //From spotlight to hitPoint
			Intersection in = FindIntersection(r);
			GLfloat distance = toUse.getLightPosition().distance(
					toUse.getLightPosition(), hitPoint);
			if (in.getT() < distance && in.getT() > 0) {
				sum *= 0;
			} else {
				Vector3f lightDir = toUse.getLightDirection();
				lightDir.normalize();
				GLfloat opening = lightDir.dotProduct(lightDir, L);
				if (opening > cos(toUse.getCutoff())) {
					//Inside the angle of spotligh
					sum *= toUse.getLightIntensity() * 2;
				} else {
					//Outside
					sum *= 0;
				}
			}
		} else {
			//DIRECTIONAL
			L = toUse.getLightDirection();
			L.makeNegative();
			L.normalize();
			Vector3f R = L - 2 * (L.dotProduct(L, N)) * N;
			R.normalize();
			sum += hit.getSphere().getD() * (N.dotProduct(N, L))
					+ hit.getSphere().getS()
							* (pow(V.dotProduct(V, R),
									hit.getSphere().getShine()));
			Ray r(hitPoint, L);
			Intersection in = FindIntersection(r);
			if (in.getT() > hit.getSphere().getRadius()) {
				//Hit something
				sum *= 0;
			} else {
				sum *= toUse.getLightIntensity();
			}
		}
		ans += sum;
	}
}
/*
 * This function calculates the color for a Plane in the scene.
 * It receives the Intersection hit, where the ray hit the Plane and returns the
 * answer in the ans Vector3f.
 */
void planeGetColor(Intersection hit, Vector3f& ans) {
	//PLANE
	ans += hit.getPlane().getA() * scene.getAmbientLight();
	Vector3f sum(0, 0, 0);
	Vector3f hitPoint = hit.getPoi();
	Vector3f N = hit.getPlane().getNormal();
	N.normalize();
	Vector3f V = hitPoint - scene.getCamera();
	V.normalize();
	Vector3f L;
	for (unsigned int i = 0; i < lights.size(); i++) {
		Light toUse = lights[i];
		if (toUse.isSpotlight()) {
			L = toUse.getLightPosition() - hitPoint;
			L.makeNegative();
			L.normalize();
			Vector3f R = L - 2 * (L.dotProduct(L, N)) * N;
			R.normalize();
			sum += hit.getPlane().getD() * (N.dotProduct(N, L))
					+ hit.getPlane().getS()
							* (pow(V.dotProduct(V, R),
									hit.getPlane().getShine()));

			Ray r(toUse.getLightPosition(), L); //From spotlight to hitPoint
			Intersection in = FindIntersection(r);
			GLfloat distance = toUse.getLightPosition().distance(
					toUse.getLightPosition(), hitPoint);
			if (in.getT() < distance && in.getT() > 0) {
				//Hit something
				sum *= 0;
			} else {
				Vector3f lightDir = toUse.getLightDirection();
				lightDir.normalize();
				GLfloat opening = lightDir.dotProduct(lightDir, L);
				if (opening > cos(toUse.getCutoff())) {
					//Inside the angle of the spotlight
					sum *= toUse.getLightIntensity() * 2;
				} else {
					//Outside
					sum *= 0;
				}
			}
		} else {
			//DIRECTIONAL
			L = toUse.getLightDirection();
			L.makeNegative();
			L.normalize();
			Vector3f R = L - 2 * (L.dotProduct(L, N)) * N;
			R.normalize();
			sum += hit.getPlane().getD() * (N.dotProduct(N, L))
					+ hit.getPlane().getS()
							* (pow(V.dotProduct(V, R),
									hit.getPlane().getShine()));
			Ray r(hitPoint, L);
			Intersection in = FindIntersection(r);
			if (in.getT() > 0) {
				//Hit something
				sum *= 0;
			} else {
				//Didn't hit anything
				sum *= toUse.getLightIntensity() * 2;
			}
		}
		ans += sum;
	}
}
/*
 * This function controls the color of the objects.
 * Also, it makes sure the color intensity is between 0-1.
 */
Vector3f GetColor(Ray ray, Intersection hit) {
	Vector3f ans(0, 0, 0);
	if (hit.isSphere) {
		//SPHERE
		sphereGetColor(hit, ans);
	} else {
		//PLANE
		planeGetColor(hit, ans);
	}
	//Normalizing values
	if (ans.x > 1)
		ans.x = 1;
	if (ans.y > 1)
		ans.y = 1;
	if (ans.z > 1)
		ans.z = 1;

	if (ans.x < 0)
		ans.x = 0;
	if (ans.y < 0)
		ans.y = 0;
	if (ans.z < 0)
		ans.z = 0;
	return ans;
}
/*
 * Main loop for ray tracing.
 * - Shoots ray from pixel (i,j)
 * - Finds intersection with object
 * - Calculates correct color to put in that pixel
 */
void startRayTracing() {
	GLint width = scene.getResolutionX();
	GLint height = scene.getResolutionY();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			//Construct Ray
			Ray ray = ConstructRayThroughPixel(scene.getCamera(), i, j);
			//Try to hit something
			Intersection hit = FindIntersection(ray);
			if (hit.t > 0) {
				//We hit!
				Vector3f color = GetColor(ray, hit);
				image[3 * (i + j * width) + 0] = color.z * 255;
				image[3 * (i + j * width) + 1] = color.y * 255;
				image[3 * (i + j * width) + 2] = color.x * 255;
			} else {
				//Didn't hit anything
				image[3 * (i + j * width) + 0] = 0;
				image[3 * (i + j * width) + 1] = 0;
				image[3 * (i + j * width) + 2] = 0;
			}
		}
	}
}

/* OPENGL GRAPHICS STUFF*/

void keyPressed(unsigned char key, int x, int y);

GLuint loadTexture();
void mydisplay(void);
GLuint texture;
void drawImage() {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(scene.getResolutionX(), scene.getResolutionY());
	glutCreateWindow("AMAZING RAY TRACING 3D");
	glOrtho(-1.0, 1.0, -1.0, 1.0, 2.0, -2.0);
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(keyPressed);
	glutMainLoop();
}

GLuint loadTexture() {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scene.getResolutionX(),
			scene.getResolutionY(), 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);
	glEnable(GL_TEXTURE_2D);
	return textureID;
}
void mydisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	texture = loadTexture();
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(1, 1);
	glTexCoord2f(1, 0);
	glVertex2f(-1, 1);
	glTexCoord2f(1, 1);
	glVertex2f(-1, -1);
	glTexCoord2f(0, 1);
	glVertex2f(1, -1);
	glEnd();
	glFlush();
}
int lastNumberPressed = -1;
bool spherePressed;
/*
 * Key pressed function.
 * Does all the motion cool stuff added to this project.
 */
void keyPressed(unsigned char key, int x, int y) {
	if (key >= 48 && key <= 57) {
		lastNumberPressed = (int) (key - '0');
	} else if (lastNumberPressed == -1 && key != 'f') {
		cout << "Insert a number and then c for Spheres" << endl;
	} else {
		switch (key) {
		case 'a':
			if (spherePressed && lastNumberPressed < (int) sphereArray.size())
				sphereArray[lastNumberPressed].moveX(-1);
			else if (!spherePressed
					&& lastNumberPressed < (int) planeArray.size())
				planeArray[lastNumberPressed].moveX(-1);
			else
				cout << "Errors where found, try again you will" << endl;
			break;
		case 'd':
			if (spherePressed && lastNumberPressed < (int) sphereArray.size())
				sphereArray[lastNumberPressed].moveX(1);
			else if (!spherePressed
					&& lastNumberPressed < (int) planeArray.size())
				planeArray[lastNumberPressed].moveX(1);
			else
				cout << "Errors where found, try again you will" << endl;
			break;
		case 's':
			if (spherePressed && lastNumberPressed < (int) sphereArray.size())
				sphereArray[lastNumberPressed].moveY(-1);
			else if (!spherePressed
					&& lastNumberPressed < (int) planeArray.size())
				planeArray[lastNumberPressed].moveY(-1);
			else
				cout << "Errors where found, try again you will" << endl;
			break;
		case 'w':
			if (spherePressed && lastNumberPressed < (int) sphereArray.size())
				sphereArray[lastNumberPressed].moveY(1);
			else if (!spherePressed
					&& lastNumberPressed < (int) planeArray.size())
				planeArray[lastNumberPressed].moveY(1);
			else
				cout << "Errors where found, try again you will" << endl;
			break;
		case 'q':
			if (spherePressed && lastNumberPressed < (int) sphereArray.size())
				sphereArray[lastNumberPressed].moveZ(1);
			else if (!spherePressed
					&& lastNumberPressed < (int) planeArray.size())
				planeArray[lastNumberPressed].moveZ(1);
			else
				cout << "Errors where found, try again you will" << endl;
			break;
		case 'e':
			if (spherePressed && lastNumberPressed < (int) sphereArray.size())
				sphereArray[lastNumberPressed].moveZ(-1);
			else if (!spherePressed
					&& lastNumberPressed < (int) planeArray.size())
				planeArray[lastNumberPressed].moveZ(-1);
			else
				cout << "Errors where found, try again you will" << endl;
			break;
		case 'c':
			spherePressed = true;
			cout << "Sphere number " << lastNumberPressed << " chosen" << endl;
			break;
		case 'f':
			cout << "Exiting..." << endl;
			exit(0);
		}
	}
	startRayTracing();
	glutPostRedisplay();
}

