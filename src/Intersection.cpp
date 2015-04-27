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
Intersection::Intersection(GLfloat t1, Sphere s){
	t = t1;
	sphere = s;
}
Intersection::Intersection(GLfloat t1, Plane p){
	t = t1;
	plane = p;
}

