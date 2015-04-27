/*
 * Sphere.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: jdorfsman
 */

#include "Sphere.h"
namespace ShapeObjects {
Sphere::Sphere(){
}
Sphere::Sphere(Vector3f c, Vector3f kAv, Vector3f kDv, Vector3f kSv, GLfloat r,
		GLfloat sh) {
	radius = r;
	center = c;
	kA = kAv;
	kD = kDv;
	kS = kSv;
	shine = sh;
}
GLfloat Sphere::intersect(Scene s, Ray ray) {
	Vector3f rayDirection = ray.getDirectionV();
	GLfloat a = pow(rayDirection.x, 2) + pow(rayDirection.y, 2) + pow(rayDirection.z, 2);
	GLfloat b = 2
			* (rayDirection.x * (s.getCenterCoordinates().x - center.x)
					+ rayDirection.y * (s.getCenterCoordinates().y - center.y)
					+ rayDirection.z * (s.getCenterCoordinates().z - center.z)); //z = 0 ?
	GLfloat c = pow(s.getCenterCoordinates().x - center.x, 2)
			+ pow(s.getCenterCoordinates().y - center.y, 2)
			+ pow(s.getCenterCoordinates().z - center.z, 2) - pow(radius, 2);

	GLfloat bsqrac = pow(b, 2) - 4 * a * c;
	if (bsqrac < 0)
		return -1;
	else {
		GLfloat t1 = (-b + sqrt(bsqrac)) / 2 * a;
		GLfloat t2 = (-b - sqrt(bsqrac)) / 2 * a;
		if (t1 > 0 && t2 > 0) {
			return (t1 < t2 ? t1 : t2);
		} else {
			if (t1 > 0)
				return t1;
			else if (t2 > 0)
				return t2;
			else
				return -2;
		}
	}
}
}
