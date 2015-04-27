/*
 * Sphere.h
 *
 *  Created on: Apr 17, 2015
 *      Author: jdorfsman
 */

#ifndef SPHERE_H_
#define SPHERE_H_
#include "GL/gl.h"
#include "GL/glut.h"
#include "Vector3f.h"
#include "Scene.h"
#include "Ray.h"
namespace ShapeObjects {


class Sphere {
public:
	Sphere();
	Sphere(Vector3f,Vector3f,Vector3f,Vector3f,GLfloat,GLfloat);
	GLfloat intersect(Scene,Ray);

	const Vector3f& getCenter() const {
		return center;
	}

	const Vector3f& getA() const {
		return kA;
	}

	const Vector3f& getD() const {
		return kD;
	}

	const Vector3f& getS() const {
		return kS;
	}

	GLfloat getRadius() const {
		return radius;
	}

	GLfloat getShine() const {
		return shine;
	}

private:
	Vector3f center;
	GLfloat radius;
	Vector3f kA;
	Vector3f kD;
	Vector3f kS;
	GLfloat shine;
};

} /* namespace Main */

#endif /* SPHERE_H_ */
