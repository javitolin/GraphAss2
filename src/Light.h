/*
 * Light.h
 *
 *  Created on: Apr 17, 2015
 *      Author: jdorfsman
 */

#ifndef LIGHT_H_
#define LIGHT_H_
#include "GL/gl.h"
#include "GL/glut.h"
#include "Vector3f.h"

namespace ShapeObjects {

class Light {
public:
	Light(Vector3f,Vector3f,Vector3f,bool,GLfloat);

	GLfloat getCutoff() const {
		return cutoff;
	}

	const Vector3f& getLightDirection() const {
		return lightDirection;
	}

	const Vector3f& getLightIntensity() const {
		return lightIntensity;
	}

	const Vector3f& getLightPosition() const {
		return lightPosition;
	}

	bool isSpotlight() const {
		return spotlight;
	}

private:
	Vector3f lightDirection;
	Vector3f lightIntensity;
	bool spotlight;
	Vector3f lightPosition;
	GLfloat cutoff;
};

} /* namespace Main */

#endif /* LIGHT_H_ */
