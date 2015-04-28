/*
 * Plane.h
 *
 *  Created on: Apr 17, 2015
 *      Author: jdorfsman
 */

#ifndef PLANE_H_
#define PLANE_H_
#include "GL/gl.h"
#include "GL/glut.h"
#include "Vector3f.h"
#include "Scene.h"
#include "Ray.h"
namespace ShapeObjects {


class Plane {
public:
	Plane();
	Plane(Vector3f,Vector3f,Vector3f,Vector3f,Vector3f, GLfloat,GLfloat,GLfloat);
	GLfloat intersect(Scene,Ray,Vector3f&);
	void moveX(int);
	void moveY(int);
	void moveZ(int);
	//Vector3f getNormal(Vector3f);
	const Vector3f& getCenter() const {
		return center;
	}

	GLfloat getHeight() const {
		return height;
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

	const Vector3f& getNormal() const {
		return normal;
	}

	GLfloat getShine() const {
		return shine;
	}

	GLfloat getWidth() const {
		return width;
	}

private:
	Vector3f normal;
	Vector3f center;
	GLfloat width;
	GLfloat height;
	Vector3f kA;
	Vector3f kD;
	Vector3f kS;
	GLfloat shine;
};

} /* namespace Main */

#endif /* PLANE_H_ */
