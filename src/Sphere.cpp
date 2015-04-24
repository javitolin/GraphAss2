/*
 * Sphere.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: jdorfsman
 */

#include "Sphere.h"

namespace ShapeObjects {

Sphere::Sphere(Vector3f c,Vector3f kAv,Vector3f kDv,Vector3f kSv,GLfloat r,GLfloat sh) {
	radius = r;
	center = c;
	kA = kAv;
	kD = kDv;
	kS = kSv;
	shine = sh;
}

} /* namespace Main */
