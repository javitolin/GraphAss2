/*
 * Sphere.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: jdorfsman
 */

#include "Sphere.h"
namespace ShapeObjects {
Sphere::Sphere(){
}
Sphere::Sphere(Vector3f c, Vector3f kAv, Vector3f kDv, Vector3f kSv, GLfloat r,
		GLfloat sh) {
	radius = r;
	center = c;
	kA = kAv;
	kD = kDv;
	kS = kSv;
	shine = sh;
}
GLfloat Sphere::intersect(Scene s, Ray ray, Vector3f& v) {
	Vector3f L = center - ray.getOriginO();
	GLfloat tm = L.dotProduct(L,ray.getDirectionV());
	GLfloat dpow = pow(center.distance(center, ray.getOriginO()),2) - pow(tm,2);
	if(dpow > pow(radius,2)){
		return -1;
	}
	GLfloat th = sqrt(pow(radius,2) - dpow);
	GLfloat t1 = tm - th;
	GLfloat t2 = tm + th;
	if(t1 > 0){
		v = ray.getOriginO() + t1*ray.getDirectionV();
		return t1;
	}
	else if (t2 > 0){
		v = ray.getOriginO() + t2*ray.getDirectionV();
		return t2;
	}
	else return -2;
}

Vector3f Sphere::getNormal(Vector3f p){
	Vector3f ans(0,0,0);
	ans = (p - center)/(p.distance(p,center));
	return ans;
}

void Sphere::moveX(int l){
	center.x += l;
}
void Sphere::moveY(int l){
	center.y += l;
}
void Sphere::moveZ(int l){
	center.z += l;
}
}
