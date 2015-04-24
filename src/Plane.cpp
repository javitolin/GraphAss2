/*
 * Plane.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: jdorfsman
 */

#include "Plane.h"

namespace ShapeObjects {

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

} /* namespace Main */
