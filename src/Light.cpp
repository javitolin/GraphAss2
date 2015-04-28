/*
 * Light.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: jdorfsman
 */

#include "Light.h"

namespace ShapeObjects {

Light::Light(Vector3f ld,Vector3f li,Vector3f lp,bool spot,GLfloat cut) {
	lightDirection = ld;
	lightDirection.normalize();
	lightIntensity = li;
	lightIntensity.normalize();
	lightPosition = lp;
	lightPosition.normalize();
	spotlight = spot;
	cutoff = cut;
}

} /* namespace Main */
