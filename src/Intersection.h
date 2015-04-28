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
	Intersection(GLfloat, Sphere s, Vector3f v);
	Intersection(GLfloat, Plane p,Vector3f v);

	const Plane& getPlane() const {
		return plane;
	}

	void setPlane(const Plane& plane) {
		this->plane = plane;
	}

	Sphere& getSphere() {
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

	const Vector3f& getPoi() const {
		return poi;
	}

	void setPoi(const Vector3f& poi) {
		this->poi = poi;
	}

	GLfloat t;
	Sphere sphere;
	Plane plane;
	Vector3f poi;
	bool isSphere;
};

#endif /* SRC_INTERSECTION_H_ */
