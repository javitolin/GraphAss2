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

	Vector3f& getUpVector() {
		return upVector;
	}

	GLint getResolutionX() const {
		return xResolution;
	}

	GLint getResolutionY() const {
		return yResolution;
	}

	const Vector3f& getCamera() const {
		return camera;
	}

	void setCamera(const Vector3f& camera) {
		this->camera = camera;
	}

private:
	Vector3f centerCoordinates;
	Vector3f upVector;
	Vector3f ambientLight;
	Vector3f camera;
	GLfloat screenWidth;
	GLfloat xResolution;
	GLfloat yResolution;

};

} /* namespace Main */

#endif /* SCENE_H_ */
