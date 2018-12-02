#pragma once
#ifndef SYLLINE_H
#define SYLLINE_H
#include "sylvester_primitive.h"
#include <vector>
using namespace std;
class Vector;
class Matrix;
class Plane;
class Line :public SylPrimitive {
private:
	Vector* anchor;
	Vector* direction;
public:
	Line();
	Line(const vector<float>& _anchor, const vector<float>& _direction);
	Line(const Vector& _anchor, const Vector& _direction);
	Line(const Vector* _anchor, const Vector* _direction);
	virtual ~Line();
	virtual Vector* getAnchor() const;
	virtual Vector* getDirection() const;
	bool eql(const Line* obj) const;
	Line dup() const;
	Line tranlate(const Vector& vec) const;
	bool isParallelTo(const SylPrimitive* obj) const;
	float distanceFrom(const SylPrimitive* obj) const;
	virtual float distanceFrom(const Vector& point) const;
	float distanceFrom(const Vector* point) const;
	bool contains(const Vector& point) const;
	bool contains(const Vector* point) const;
	bool liesIn(Plane* plane) const;
	bool intersects(const SylPrimitive* obj) const;
	Vector intersectionWith(const SylPrimitive* obj) const;
	Vector* pointClosestTo(const SylPrimitive* obj) const;
	virtual Vector* pointClosestTo(const Vector& point) const;
	Line rotate(const float& t, const Line& line) const;
	Line reflectionIn(SylPrimitive* obj) const;
	Line reflectionIn(const Vector& point) const;
	Line reflectionIn(const Vector* point) const;
};
#endif // !SYLLINE_H

