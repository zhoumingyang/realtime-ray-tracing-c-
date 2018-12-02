#pragma once
#ifndef HITABLE_H
#define HITABLE_H
#include "sylvester_vector.h"
#include "sylvester_matrix.h"
#include <string>
#include <vector>
#include <float.h>

//base class
class PathTracer;
class Hitable {
public:
	virtual string getGlobalCode() const {
		return "";
	}

	virtual string getIntersectCode() const {
		return "";
	}

	virtual string getShadowTestCode() const {
		return "";
	}

	virtual string getMinimumIntersectCode() const {
		return "";
	}

	virtual string getNormalCalculationCode() const {
		return "";
	}

	virtual void temporaryTranslate(const Vector& translation) {

	}

	virtual void translate(const Vector& translation) {

	}

	virtual Vector getMinCorner() const {
		return Vector();
	}

	virtual Vector getMaxCorner() const {
		return Vector();
	}

	virtual float intersect(const Vector& origin, const Vector& ray) const {
		return FLT_MAX;
	}

	virtual void setUniforms(PathTracer* pt) {

	}
};
#endif // !HITABLE_H
