/*
 * Intersection.h
 *
 *  Created on: Apr 27, 2015
 *      Author: jdorfsman
 */

#ifndef SRC_INTERSECTION_H_
#define SRC_INTERSECTION_H_
#include "Sphere.h"
#include "Plane.h"
using namespace ShapeObjects;

class Intersection {
public:
	Intersection();
	Intersection(GLfloat, Sphere s);
	Intersection(GLfloat, Plane p);

	const Plane& getPlane() const {
		return plane;
	}

	void setPlane(const Plane& plane) {
		this->plane = plane;
	}

	const Sphere& getSphere() const {
		return sphere;
	}

	void setSphere(const Sphere& sphere) {
		this->sphere = sphere;
	}

	GLfloat getT() const {
		return t;
	}

	void setT(GLfloat t) {
		this->t = t;
	}

	GLfloat t;
	Sphere sphere;
	Plane plane;
};

#endif /* SRC_INTERSECTION_H_ */
