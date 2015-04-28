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
	kD = kDv;
	kS = kSv;
	width = w;
	height = h;
	shine = s;
}
GLfloat Plane::intersect(Scene s,Ray r,Vector3f& poi){
	GLfloat t = normal.dotProduct(normal,((center - r.getOriginO())/normal.dotProduct(normal,r.getDirectionV())));
	if(t > 0) return t;
	else return -1;

}
} /* namespace Main */
