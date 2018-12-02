#pragma once
#ifndef SYLPLANE_H
#define SYLPLANE_H
#include <vector>
#include <string>
#include "sylvester_primitive.h"
using namespace std;
class Vector;
class Matrix;
class Line;
class Plane :public SylPrimitive {
private:
	Vector* anchor;
	Vector* normal;
public:
	Plane();
	Plane(const Vector* _anchor, const Vector* _normal);
	Plane(const Vector& _anchor, const Vector& _normal);
	Plane(const vector<float>& _a, const vector<float>& _n);
	virtual ~Plane();
	virtual Vector* getAnchor() const;
	virtual Vector* getNormal() const;
	bool eql(const Plane* obj) const;
	Plane dup() const;
	Plane translate(const Vector& trans) const;
	bool isParalleTo(const SylPrimitive* obj) const;
	bool isPerpendicularTo(const Plane& plane) const;
	float distanceFrom(const SylPrimitive* obj) const;
	float distanceFrom(const Vector& point) const;
	bool contains(const SylPrimitive* obj) const;
	bool contains(const Vector& point) const;
	bool intersects(const SylPrimitive* obj) const;
	void intersectionWith(const SylPrimitive* obj, void* result) const;
	virtual Vector* pointClosestTo(const Vector& point) const;
	Plane rotate(const float& theta, const Line& line) const;
	Plane reflectionIn(SylPrimitive* obj) const;
	Plane reflectionIn(const Vector& point) const;
	Plane reflectionIn(const Vector* point) const;
};
#endif // !SYLPLANE_H

