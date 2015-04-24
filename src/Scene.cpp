/*
 * Scene.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: jdorfsman
 */

#include "Scene.h"

namespace ShapeObjects {
Scene::Scene(){

}
Scene::Scene(Vector3f cc,Vector3f uv,Vector3f al,GLfloat sW,GLfloat xR,GLfloat yR) {
	centerCoordinates = cc;
	upVector = uv;
	ambientLight = al;
	screenWidth = sW;
	xResolution = xR;
	yResolution = yR;
}


} /* namespace Main */
