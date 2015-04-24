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
	lightIntensity = li;
	lightPosition = lp;
	spotlight = spot;
	cutoff = cut;
}

} /* namespace Main */
