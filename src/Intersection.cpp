/*
 * Intersection.cpp
 *
 *  Created on: Apr 27, 2015
 *      Author: jdorfsman
 */

#include "Intersection.h"

Intersection::Intersection() {
	t = 0;
}
Intersection::Intersection(GLfloat t1, Sphere s, Vector3f v){
	t = t1;
	sphere = s;
	poi = v;
	isSphere = true;
}
Intersection::Intersection(GLfloat t1, Plane p, Vector3f v){
	t = t1;
	plane = p;
	poi = v;
	isSphere = false;
}

