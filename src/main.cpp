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

using namespace std;
using namespace ShapeObjects;

Scene s;
vector<Light> lights;
vector<Plane> planes;
vector<Sphere> sphere;

vector<float> extractNumbers(char *line){
	vector<float> a;
	string s = line;
	string forNow = "";
	for(unsigned int i = 0; i < s.size(); i++){
		if(s[i] == ','){
			stringstream ss;
			ss << forNow;
			float f;
			ss >> f;
			a.push_back(f);
			forNow = "";
			//cout << f << endl;
		}
		else{
			if(s[i] != ' '){
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

int main() {
	ifstream sceneFile("scene.txt");
	char output[512];
	int objectsFound = 0;
	while (sceneFile.is_open()) {
		while (!sceneFile.eof()) {
			sceneFile >> output;
			if(strcmp(output,"scene") == 0){
				objectsFound++;
				sceneFile >> output;
				cout << "scene!" << endl;
				vector<float> params = extractNumbers(output);
				Vector3f cc(params[0],params[1],params[2]);
				Vector3f up(params[3],params[4],params[5]);
				float width = params[6];
				float rX = params[7];
				float rY = params[8];
				Vector3f amb(params[9],params[10],params[11]);
				s = Scene(cc,up,amb,width,rX,rY);
			}
			else if(strcmp(output,"light") == 0){
				objectsFound++;
				sceneFile >> output;
				cout << "light!" << endl;
				extractNumbers(output);
				vector<float> params = extractNumbers(output);
				Vector3f ld(params[0],params[1],params[2]);
				Vector3f li(params[3],params[4],params[5]);
				if(params.size() == 10){
					Vector3f lp(params[6],params[7],params[8]);
					GLfloat cut = params[9];
					bool spot = true;
					lights.push_back(Light(ld,li, lp,spot, cut));
				}
				else{
					Vector3f lp(0,0,0);
					lights.push_back(Light(ld,li, lp,false, 0));
				}
			}
			else if(strcmp(output,"spher") == 0){
				objectsFound++;
				sceneFile >> output;
				cout << "spher!" << endl;
				extractNumbers(output);
				vector<float> params = extractNumbers(output);
				Vector3f c(params[0],params[1],params[2]);
				Vector3f kAv(params[3],params[4],params[5]);
				Vector3f kDv(params[6],params[7],params[8]);
				Vector3f kSv(params[9],params[10],params[11]);
				GLfloat r = params[12];
				GLfloat sh = params[13];
				sphere.push_back(Sphere(c,kAv,kDv,kSv,r,sh));
			}
			else if(strcmp(output,"plane") == 0){
				objectsFound++;
				sceneFile >> output;
				cout << "plane!" << endl;
				extractNumbers(output);
				vector<float> params = extractNumbers(output);
				Vector3f n(params[0],params[1],params[2]);
				Vector3f c(params[3],params[4],params[5]);
				Vector3f kAv(params[6],params[7],params[8]);
				Vector3f kDv(params[9],params[10],params[11]);
				Vector3f kSv(params[12],params[13],params[14]);
				GLfloat w = params[15];
				GLfloat h = params[16];
				GLfloat s = params[17];
				planes.push_back(Plane(n,c,kAv,kDv,kSv,w,h,s));
			}
		}
		sceneFile.close();
	}
	cout << "Reading file completed. Found: " << objectsFound << " objects" << endl;
	return 0;
}
