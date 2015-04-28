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
#include "Image.h"
#include <limits>

using namespace std;
using namespace ShapeObjects;

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
			//cout << f << endl;
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
	//cout << f << endl;
	return a;
}

int main(int argc, char* argv[]) {
	ifstream sceneFile("scene.txt");
	char output[512];
	int objectsFound = 0;
	while (sceneFile.is_open()) {
		while (!sceneFile.eof()) {
			sceneFile >> output;
			if (strcmp(output, "scene") == 0) {
				objectsFound++;
				sceneFile >> output;
				cout << "scene!" << endl;
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
				cout << "light!" << endl;
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
				cout << "spher!" << endl;
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
				cout << "plane!" << endl;
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
	image = new GLubyte[scene.getResolutionX() * scene.getResolutionY() * 3];
	startRayTracing();
	glutInit(&argc, argv);
	drawImage();
	cout << "Reading file completed. Found: " << objectsFound << " objects"
			<< endl;
	return 0;
}
Ray ConstructRayThroughPixel(Vector3f source, int i, int j) {
	//scene.getCenterCoordinates() +
	GLfloat d = scene.getCenterCoordinates().distance(
			scene.getCenterCoordinates(), source);
	Vector3f Pc = source + scene.getCenterCoordinates() * d;
	Vector3f vRight = scene.getCenterCoordinates().crossProduct(
			scene.getUpVector(), scene.getCenterCoordinates());
	vRight.normalize();
	Vector3f UP = scene.getUpVector();
	UP.normalize();
	Pc.normalize();
	GLfloat R = (float) scene.getScreenWidth() / (float) scene.getResolutionX();

	Vector3f P = Pc
			+ (float) (i - floor((float) scene.getResolutionX() / 2)) * R
					* vRight
			- (float) (j - floor((float) scene.getResolutionY() / 2)) * R * UP;
	//P = P - Pc;

	//cout << P.x << " " << P.y << " " << P.z << endl;
	P.normalize();
	Ray ans(source, P);
	return ans;
}
Intersection FindIntersection(Ray ray) {
	GLint min_t = numeric_limits<int>::max();
	Sphere min_sphere;
	Plane min_plane;
	Vector3f poi;
	bool fromPlane = false;
	bool found = false;
	for (unsigned int i = 0; i < sphereArray.size(); i++) {
		Sphere sp = sphereArray[i];
		GLfloat t = sp.intersect(scene, ray, poi);
		if (t > 0) {
			if (t < min_t) {
				min_sphere = sp;
				min_t = t;
				found = true;
			}
		}
	}
	for (unsigned int i = 0; i < planeArray.size(); i++) {
		Plane p = planeArray[i];
		GLfloat t = p.intersect(scene, ray, poi);
		if (t > 0) {
			if (t < min_t) {
				fromPlane = true;
				min_plane = p;
				min_t = t;
				found = true;
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

Vector3f GetColor(Ray ray, Intersection hit) {
	Vector3f ans(0,0,0);
	if(hit.isSphere){
		//SPHERE
		ans += hit.getSphere().getA() * scene.getAmbientLight();
		Vector3f sum(0,0,0);
		for(int i = 0; i < lights.size(); i++){
			Vector3f L = lights[i].getLightPosition() - hit.getPoi();
			L.normalize();
			sum += hit.getSphere().getD() * (hit.getSphere().getNormal(hit.getPoi()) * L);
			sum +=
		}
	}
	else{
		//SPHERE
		ans += hit.getPlane().getA() * scene.getAmbientLight();
	}
	return ans;
}

void startRayTracing() {
	GLint width = scene.getResolutionX();
	GLint height = scene.getResolutionY();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Ray ray = ConstructRayThroughPixel(scene.getCamera(), i, j);
			Intersection hit = FindIntersection(ray);
			if (hit.t > 0) {
				Vector3f color = GetColor(ray,hit);
				image[3 * (i + j * width) + 0] = color.x*255;
				image[3 * (i + j * width) + 1] = color.y*255;
				image[3 * (i + j * width) + 2] = color.z*255;
			} else {
				image[3 * (i + j * width) + 0] = 0;
				image[3 * (i + j * width) + 1] = 0;
				image[3 * (i + j * width) + 2] = 0;
			}/*
			 for (int k = 0; k < 3; k++) {
			 image[i + j * width] = 255;
			 }*/
			//image[i][j] = GetColor( ray, hit);
		}
	}
	/*
	 for(int i = 0; i < scene.getResolutionX()*scene.getResolutionY()*3; i++){
	 image[i] = 120;
	 }*/
}

/* OPENGL GRAPHICS STUFF*/
GLuint loadTexture();
void mydisplay(void);
GLuint texture;
void drawImage() {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(scene.getResolutionX(), scene.getResolutionY());
	glutCreateWindow("3DMOFO");
	glOrtho(-1.0, 1.0, -1.0, 1.0, 2.0, -2.0);
	texture = loadTexture();
	glutDisplayFunc(mydisplay);
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

