/*
 * Scene.h
 *
 *  Created on: Apr 17, 2015
 *      Author: jdorfsman
 */

#ifndef SCENE_H_
#define SCENE_H_
#include "GL/gl.h"
#include "GL/glut.h"
#include "Vector3f.h"
#include <ostream>
namespace ShapeObjects {


class Scene {
public:
	Scene();
	Scene(Vector3f,Vector3f,Vector3f,GLfloat,GLfloat,GLfloat);

	const Vector3f& getAmbientLight() const {
		return ambientLight;
	}

	const Vector3f& getCenterCoordinates() const {
		return centerCoordinates;
	}

	GLint getScreenWidth() const {
		return screenWidth;
	}

	const Vector3f& getUpVector() const {
		return upVector;
	}

	GLint getResolutionX() const {
		return xResolution;
	}

	GLint getResolutionY() const {
		return yResolution;
	}


private:
	Vector3f centerCoordinates;
	Vector3f upVector;
	Vector3f ambientLight;
	GLfloat screenWidth;
	GLfloat xResolution;
	GLfloat yResolution;

};

} /* namespace Main */

#endif /* SCENE_H_ */
