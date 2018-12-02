#include "sylvester_line.h"
#include "sylvester_vector.h"
#include "sylvester_matrix.h"
#include <float.h>
#include <math.h>

Line::Line() :SylPrimitive("line") {
	vector<float> _a = { 0.0, 0.0, 0.0 };
	vector<float> _d = { 0.0, 0.0, 0.0 };
	anchor = new Vector(_a);
	direction = new Vector(_d);
}

Line::Line(const vector<float>& _anchor, const vector<float>& _direction) :SylPrimitive("line") {
	anchor = new Vector(_anchor);
	direction = new Vector(_direction);
	vector<float> _a = { 0, 0, 0 };
	vector<float> _d = { 0, 0, 0 };
	if (_anchor.size() == 2) {
		_a = { _anchor[0], _anchor[1], 0.0 };
		anchor = new Vector(_a);
	}
	if (_direction.size() == 2) {
		_d = { _direction[0], _direction[1], 0.0 };
		direction = new Vector(_d);
	}
	if (_anchor.size() > 3 || _direction.size() > 3) {
		anchor = new Vector(_a);
		direction = new Vector(_d);
	}
}

Line::Line(const Vector& _anchor, const Vector& _direction) :SylPrimitive("line") {
	vector<float> _achEle = _anchor.allElements();
	vector<float> _dirEle = _direction.allElements();
	anchor = new Vector(_achEle);
	direction = new Vector(_dirEle);
	if (_achEle.size() == 2) {
		_achEle = { _achEle[0], _achEle[1], 0.0 };
		anchor = new Vector(_achEle);
	}
	if (_dirEle.size() == 2) {
		_dirEle = { _dirEle[0], _dirEle[1], 0.0 };
		direction = new Vector(_dirEle);
	}
	if (_achEle.size() > 3 || _dirEle.size() > 3) {
		vector<float> _a = { 0, 0, 0 };
		vector<float> _d = { 0, 0, 0 };
		anchor = new Vector(_a);
		direction = new Vector(_d);
	}
}

Line::Line(const Vector* _anchor, const Vector* _direction) :SylPrimitive("line") {
	Line(*_anchor, *_direction);
}

Line::~Line() {
	if (anchor != NULL) {
		delete anchor;
	}
	if (direction != NULL) {
		delete direction;
	}
}

Vector* Line::getAnchor() const {
	return anchor;
}

Vector* Line::getDirection() const {
	return direction;
}

bool Line::eql(const Line* line) const {
	return this->isParallelTo(line) && this->contains(line->getAnchor());
}

Line Line::dup() const {
	return Line(anchor, direction);
}

Line Line::tranlate(const Vector& vec) const {
	vector<float> a = anchor->allElements();
	vector<float> v = vec.allElements();
	if (v.size() == 2) {
		v.push_back(0.0);
	}
	if (v.size() != 3) {
		return Line();
	}
	vector<float> t = {
		a[0] + v[0],
		a[1] + v[1],
		a[2] + v[2]
	};
	return Line(t, *direction);
}

bool Line::isParallelTo(const SylPrimitive* obj) const {
	if (obj->instanceof("plane")) {

	}
	else if (obj->instanceof("line")) {
		Vector* dir = obj->getDirection();
		float theta = direction->angleFrom(dir);
		return (abs(theta) <= SYLVESTER_RRECISION || abs(theta - M_PI) <= SYLVESTER_RRECISION);
	}
	return false;
}

float Line::distanceFrom(const SylPrimitive* obj) const {
	if (obj->instanceof("plane")) {

	}
	else if (obj->instanceof("line")) {
		if (this->isParallelTo(obj)) {
			Vector* _anchor = obj->getAnchor();
			return this->distanceFrom(_anchor);
		}
		Vector crossMat = direction->cross(obj->getAnchor());
		crossMat.normalize();
		vector<float> N = crossMat.allElements();
		vector<float> A = anchor->allElements();
		vector<float> B = obj->getAnchor()->allElements();
		return abs((A[0] - B[0]) * N[0] + (A[1] - B[1]) * N[1] + (A[2] - B[2]) * N[2]);
	}
	return FLT_MAX;
}

float Line::distanceFrom(const Vector* point) const {
	if (point == NULL) {
		return NULL;
	}
	return distanceFrom(*point);
}

float Line::distanceFrom(const Vector& point) const {
	vector<float> p = point.allElements();
	if (p.size() == 2) {
		p.push_back(0.0);
	}
	if (p.size() != 3) {
		return FLT_MAX;
	}
	vector<float> a = anchor->allElements();
	float pa1 = p[0] - a[0];
	float pa2 = p[1] - a[1];
	float pa3 = p[2] - a[2];
	float modPa = sqrt(pa1 * pa1 + pa2 * pa2 + pa3 * pa3);
	if (modPa <= 0.0000001) return 0;
	Vector dupDir = direction->dup();
	dupDir.normalize();
	vector<float> dir = dupDir.allElements();
	float cosTheta = (pa1 * dir[0] + pa2 * dir[1] + pa3 * dir[2]) / modPa;
	float sin2 = 1.0 - cosTheta * cosTheta;
	return abs(modPa * sqrt(sin2 < 0 ? 0.0 : sin2));
}

bool Line::contains(const Vector& point) const {
	float dist = this->distanceFrom(point);
	return (dist >= 0 && dist <= SYLVESTER_RRECISION);
}

bool Line::contains(const Vector* point) const {
	if (point == NULL) {
		return false;
	}
	return contains(*point);
}

bool Line::intersects(const SylPrimitive* obj) const {
	if (obj->instanceof("plane")) {

	}
	else if (obj->instanceof("line")) {
		return (!this->isParallelTo(obj) && this->distanceFrom(obj) <= SYLVESTER_RRECISION);
	}
	return false;
}

Vector Line::intersectionWith(const SylPrimitive* obj) const {
	if (obj->instanceof("plane")) {

	}
	else if (obj->instanceof("line")) {
		if (!this->intersects(obj)) {
			return Vector();
		}
		vector<float> P = anchor->allElements();
		vector<float> X = direction->allElements();
		vector<float> Q = obj->getAnchor()->allElements();
		vector<float> Y = obj->getDirection()->allElements();
		float X1 = X[0], X2 = X[1], X3 = X[2];
		float Y1 = Y[0], Y2 = Y[1], Y3 = Y[2];
		float PsubQ1 = P[0] - Q[0];
		float PsubQ2 = P[1] - Q[1];
		float PsubQ3 = P[2] - Q[2];
		float XdotQsubP = -X1 * PsubQ1 - X2 * PsubQ2 - X3 * PsubQ3;
		float YdotPsubQ = Y1 * PsubQ1 + Y2 * PsubQ2 + Y3 * PsubQ3;
		float XdotX = X1 * X1 + X2 * X2 + X3 * X3;
		float YdotY = Y1 * Y1 + Y2 * Y2 + Y3 * Y3;
		float XdotY = X1 * Y1 + X2 * Y2 + X3 * Y3;
		float k = (XdotQsubP * YdotY / XdotX + XdotY * YdotPsubQ) / (YdotY - XdotY * XdotY);
		vector<float> v = { P[0] + k * X1, P[1] + k * X2, P[2] + k * X3 };
		return Vector(v);
	}
	return Vector();
}

Vector* Line::pointClosestTo(const SylPrimitive* obj) const {
	if (obj->instanceof("line")) {
		if (this->intersects(obj)) {
			Vector pt = this->intersectionWith(obj);
			return new Vector(pt);
		}
		if (this->isParallelTo(obj)) {
			return new Vector();
		}
	}
	return new Vector();
}

Vector* Line::pointClosestTo(const Vector& point) const {
	vector<float> P = point.allElements();
	if (P.size() == 2) {
		P.push_back(0.0);
	}
	if (P.size() != 3) {
		return new Vector();
	}
	if (this->contains(point)) {
		return new Vector(P);
	}
	vector<float> A = anchor->allElements();
	vector<float> D = direction->allElements();
	float D1 = D[0], D2 = D[1], D3 = D[2];
	float A1 = A[0], A2 = A[1], A3 = A[2];
	float x = D1 * (P[1] - A2) - D2 * (P[0] - A1);
	float y = D2 * ((P[2] || 0) - A3) - D3 * (P[1] - A2);
	float z = D3 * (P[0] - A1) - D1 * ((P[2] || 0) - A3);
	vector<float> v = { D2 * x - D3 * z, D3 * y - D1 * x, D1 * z - D2 * y };
	Vector V(v);
	float k = this->distanceFrom(P) / V.moduluse();
	vector<float> newv = { P[0] + v[0] * k, P[1] + v[1] * k, P[2] + v[2] * k };
	return new Vector(newv);
}

Line Line::rotate(const float& t, const Line& line) const {
	Matrix rotMat = Matrix().rotation(t, *line.getDirection());
	vector<vector<float>> R = rotMat.allElements();
	Vector* C = line.pointClosestTo(*anchor);
	vector<float> A = anchor->allElements();
	vector<float> D = direction->allElements();
	float C1 = C->x(), C2 = C->y(), C3 = C->z();
	float A1 = A[0], A2 = A[1], A3 = A[2];
	float x = A1 - C1, y = A2 - C2, z = A3 - C3;
	vector<float> _anchor = {
		C1 + R[0][0] * x + R[0][1] * y + R[0][2] * z,
		C2 + R[1][0] * x + R[1][1] * y + R[1][2] * z,
		C3 + R[2][0] * x + R[2][1] * y + R[2][2] * z
	};
	vector<float> _direction = {
		R[0][0] * D[0] + R[0][1] * D[1] + R[0][2] * D[2],
		R[1][0] * D[0] + R[1][1] * D[1] + R[1][2] * D[2],
		R[2][0] * D[0] + R[2][1] * D[1] + R[2][2] * D[2]
	};
	return Line(_anchor, _direction);
}

Line Line::reflectionIn(SylPrimitive* obj) const {
	if (obj->instanceof("plane")) {

	}
	else if (obj->instanceof("line")) {
		Line *l = dynamic_cast<Line*>(obj);
		if (l == NULL) {
			return Line();
		}
		return rotate(M_PI, *l);
	}
	return Line();
}

Line Line::reflectionIn(const Vector& point) const {
	vector<float> P = point.allElements();
	if (P.size() == 2) {
		P.push_back(0.0);
	}
	if (P.size() != 3) {
		return Line();
	}
	Vector re = anchor->reflectionIn(P);
	return Line(re, *direction);
}

Line Line::reflectionIn(const Vector* point) const {
	if (point == NULL) {
		return Line();
	}
	return reflectionIn(*point);
}