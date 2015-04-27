/*
 * Ray.h
 *
 *  Created on: Apr 27, 2015
 *      Author: jdorfsman
 */

#ifndef SRC_RAY_H_
#define SRC_RAY_H_
#include "Vector3f.h"
class Ray {
public:
	Ray(Vector3f,Vector3f);

	const Vector3f& getDirectionV() const {
		return directionV;
	}

	void setDirectionV(const Vector3f& directionV) {
		this->directionV = directionV;
	}

	const Vector3f& getOriginO() const {
		return originO;
	}

	void setOriginO(const Vector3f& originO) {
		this->originO = originO;
	}

	Vector3f originO;
	Vector3f directionV;
};

#endif /* SRC_RAY_H_ */
