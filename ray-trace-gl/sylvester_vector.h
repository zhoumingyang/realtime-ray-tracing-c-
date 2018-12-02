#pragma once
#ifndef SYLVECTOR_H
#define SYLVECTOR_H
#include "sylvester_common.h"
#include <vector>
#include <iterator>
#include <math.h>
#include <algorithm>

using namespace std;
class SylPrimitive;
class Matrix;
class Plane;
class Line;
class Vector {
public:
	Vector();
	Vector(vector<float> v);
	Vector(float v[], int n);
	Vector(float x, float y, float z);
	~Vector();
	static Vector I;
	static Vector J;
	static Vector K;
	void addElement(const float& x);
	void popElement();
	float x() const;
	float y() const;
	float z() const;
	float e(int i) const;
	int dimensions() const;
	vector <float> allElements() const;
	float dot(const Vector* vec) const;
	float dot(const Vector& vec) const;
	float moduluse() const;
	void normalize();
	float length();
	bool eql(const Vector& vec) const;
	Vector dup() const;
	void each(void(*f)(float e, int idx));
	Vector map(float(*f)(float e, int idx));
	Vector toUnitVector() const;
	float angleFrom(const Vector& vec) const;
	float angleFrom(const Vector* vec) const;
	bool isParallelTo(const Vector& vec) const;
	bool isAntiparallelTo(const Vector& vec) const;
	bool isPerpendicularTo(const Vector& vec) const;
	Vector add(const Vector& vec) const;
	Vector substract(const Vector& vec) const;
	Vector multiply(float num) const;
	Vector cross(const Vector& vec) const;
	Vector cross(const Vector* vec) const;
	float max() const;
	Vector max(const Vector& a, const Vector& b) const;
	Vector min(const Vector& a, const Vector& b) const;
	float min() const;
	int indexOf(const float& x) const;
	Matrix toDiagonalMatrix() const;
	Vector roundAll() const;
	Vector snapTo(const float& x) const;
	float distanceFrom(const SylPrimitive* obj) const;
	float distanceFrom(const Vector& obj) const;
	bool liesOn(const Line& line) const ;
	bool liesIn(const Plane& plane) const;
	Vector rotate(const float& t, const SylPrimitive* obj) const;
	Vector rotate(const float& t, const Line& line) const;
	Vector rotate(const float& t, const Vector& point) const;
	Vector reflectionIn(const SylPrimitive* obj) const;
	Vector reflectionIn(const Vector& point) const;
	Vector slice(const int& first, const int& last) const;
	Vector to3D() const;
	void setElements(Vector& vec);
	void setElements(vector<float>& vec);
	Vector zero(int n) const;
	Vector rand(int n) const;
	Vector ensure3() const;
	Vector ensure4(const float& w) const;
	Vector divideByW() const;
	Vector componentDivide(const Vector& v) const;
private:
	vector<float> elements;
};

#endif // !SYLVECTOR_H