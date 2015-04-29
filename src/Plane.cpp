/*
 * Plane.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: jdorfsman
 */

#include "Plane.h"

namespace ShapeObjects {
Plane::Plane(){

}
Plane::Plane(Vector3f n,Vector3f c,Vector3f kAv,Vector3f kDv,Vector3f kSv, GLfloat w,GLfloat h,GLfloat s) {
	normal = n;
	center = c;
	kA = kAv;
	kA.normalize();
	kD = kDv;
	kD.normalize();
	kS = kSv;
	kS.normalize();
	width = w;
	height = h;
	shine = s;
}
GLfloat Plane::intersect(Scene s,Ray r,Vector3f& poi){
	GLfloat t = normal.dotProduct(normal,((center - r.getOriginO())/normal.dotProduct(normal,r.getDirectionV())));
	poi = r.getOriginO() + t*r.getDirectionV();
	if(t >= 1) return t;
	else return -1;
}
void Plane::moveX(int l){
	center.x += l;
}
void Plane::moveY(int l){
	center.y += l;
}
void Plane::moveZ(int l){
	center.z += l;
}
} /* namespace Main */
