#pragma once
#ifndef SYLPRIMITIVE_H
#define SYLPRIMITIVE_H
#include <string>
#include <vector>
class Vector;
class SylPrimitive {
private:
	std::string geometryPrototye;
public:
	SylPrimitive() {
		geometryPrototye = "";
	}
	SylPrimitive(const std::string& name) {
		geometryPrototye = name;
	}
	virtual ~SylPrimitive() {

	}
	virtual bool instanceof(std::string geometryname) const {
		if (geometryPrototye == geometryname) {
			return true;
		}
		return false;
	}
	virtual Vector* getAnchor() const { return NULL; }
	virtual Vector* getDirection() const { return NULL; }
	virtual Vector* getNormal() const { return NULL; }
	virtual Vector* pointClosestTo(const Vector& obj) const { return NULL; }
	virtual float distanceFrom(const Vector& point) const { return NULL; }
};
#endif // !SYLPRIMITIVE_H
