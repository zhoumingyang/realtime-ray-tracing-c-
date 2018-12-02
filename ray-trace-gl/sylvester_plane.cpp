#include "sylvester_plane.h"
#include "sylvester_vector.h"
#include "sylvester_matrix.h"
#include "sylvester_line.h"

Plane::Plane() :SylPrimitive("plane") {
	vector<float> _a = { 0.0, 0.0, 0.0 };
	vector<float> _n = { 0.0, 0.0, 0.0 };
	anchor = new Vector(_a);
	normal = new Vector(_n);
}

Plane::Plane(const Vector& _anchor, const Vector& _normal) :SylPrimitive("plane") {
	vector<float> _achEle = _anchor.allElements();
	vector<float> _norEle = _normal.allElements();
	if (_achEle.size() == 2) {
		_achEle.push_back(0.0);
		anchor = new Vector(_achEle);
	}
	if (_norEle.size() == 2) {
		_norEle.push_back(0.0);
		normal = new Vector(_norEle);
	}
	if (_achEle.size() > 3 || _norEle.size() > 3) {
		vector<float> _a = { 0, 0, 0 };
		vector<float> _n = { 0, 0, 0 };
		anchor = new Vector(_a);
		normal = new Vector(_n);
	}
}

Plane::Plane(const Vector* _anchor, const Vector* _normal) :SylPrimitive("plane") {
	Plane(*_anchor, *_normal);
}

Plane::Plane(const vector<float>& _anchor, const vector<float>& _normal) : SylPrimitive("plane") {
	anchor = new Vector(_anchor);
	normal = new Vector(_normal);
	vector<float> _a = { 0, 0, 0 };
	vector<float> _n = { 0, 0, 0 };
	if (_anchor.size() == 2) {
		_a = { _anchor[0], _anchor[1], 0.0 };
		anchor = new Vector(_a);
	}
	if (_normal.size() == 2) {
		_n = { _normal[0], _normal[1], 0.0 };
		normal = new Vector(_n);
	}
	if (_anchor.size() > 3 || _normal.size() > 3) {
		anchor = new Vector(_a);
		normal = new Vector(_n);
	}
}

Plane::~Plane() {
	if (anchor != NULL) {
		delete anchor;
	}
	if (normal != NULL) {
		delete normal;
	}
}

Vector* Plane::getAnchor() const {
	return anchor;
}

Vector* Plane::getNormal() const {
	return normal;
}

Plane Plane::dup() const {
	return Plane(anchor, normal);
}

Plane Plane::translate(const Vector& trans) const {
	vector<float> a = anchor->allElements();
	vector<float> t = trans.allElements();
	if (t.size() == 2) {
		t.push_back(0.0);
	}
	if (t.size() != 3) {
		return Plane();
	}
	vector<float> na = {
		a[0] + t[0],
		a[1] + t[1],
		a[2] + t[2]
	};
	return Plane(na, *normal);
}
bool Plane::isParalleTo(const SylPrimitive* obj) const {
	float theta;
	if (obj->instanceof("plane")) {
		theta = normal->angleFrom(obj->getNormal());
		return (abs(theta) <= SYLVESTER_RRECISION || abs(M_PI - theta) <= SYLVESTER_RRECISION);
	}
	else if (obj->instanceof("line")) {
		return normal->isPerpendicularTo(*obj->getDirection());
	}
	return false;
}

bool Plane::isPerpendicularTo(const Plane& plane) const {
	float theta = normal->angleFrom(plane.getNormal());
	return (abs(M_PI / 2 - theta) <= SYLVESTER_RRECISION);
}

float Plane::distanceFrom(const SylPrimitive* obj) const {
	if (intersects(obj) || contains(obj)) { return 0; }
	if (obj->instanceof("plane") || obj->instanceof("line")) {
		vector<float> A = anchor->allElements();
		vector<float> B = obj->getAnchor()->allElements();
		vector<float> N = normal->allElements();
		return abs((A[0] - B[0]) * N[0] + (A[1] - B[1]) * N[1] + (A[2] - B[2]) * N[2]);
	}
	return FLT_MAX;
}

float Plane::distanceFrom(const Vector& point) const {
	vector<float> P = point.allElements();
	vector<float> A = anchor->allElements();
	vector<float> N = normal->allElements();
	return abs((A[0] - P[0]) * N[0] + (A[1] - P[1]) * N[1] + (A[2] - (P[2] || 0)) * N[2]);
}

bool Plane::contains(const SylPrimitive* obj) const {
	if (obj->instanceof("plane")) {
		return false;
	}
	if (obj->instanceof("line")) {
		Vector another = obj->getAnchor()->add(*obj->getDirection());
		return (contains(*obj->getAnchor()) && contains(another));
	}
	return false;
}

bool Plane::contains(const Vector& point) const {
	vector<float> P = point.allElements();
	vector<float> A = anchor->allElements();
	vector<float> N = normal->allElements();
	float diff = abs(N[0] * (A[0] - P[0]) + N[1] * (A[1] - P[1]) + N[2] * (A[2] - (P[2] || 0)));
	return diff <= SYLVESTER_RRECISION;
}

bool Plane::intersects(const SylPrimitive* obj) const {
	if (!obj->instanceof("plane") && !obj->instanceof("line")) {
		return false;
	}
	return !isParalleTo(obj);
}

void Plane::intersectionWith(const SylPrimitive* obj, void* result) const {
	if (intersects(obj)) {
		return;
	}
	if (obj->instanceof("line")) {
		vector<float> A = obj->getAnchor()->allElements();
		vector<float> D = obj->getDirection()->allElements();
		vector<float> P = anchor->allElements();
		vector<float> N = normal->allElements();
		float multiplier = (N[0] * (P[0] - A[0]) + N[1] * (P[1] - A[1]) + N[2] * (P[2] - A[2])) / (N[0] * D[0] + N[1] * D[1] + N[2] * D[2]);
		vector<float> newV = { A[0] + D[0] * multiplier, A[1] + D[1] * multiplier, A[2] + D[2] * multiplier };
		result = new Vector(newV);
	}
	else if (obj->instanceof("plane")) {
		Vector _direction = normal->cross(obj->getNormal()).toUnitVector();
		vector<float> N = normal->allElements();
		vector<float> A = anchor->allElements();
		vector<float> O = obj->getNormal()->allElements();
		vector<float> B = obj->getAnchor()->allElements();
		Matrix solver = Matrix().zero(2);
		int i = 0;
		while (solver.isSingular()) {
			i++;
			vector<vector<float>> se = { {N[i % 3], N[(i + 1) % 3]},{ O[i % 3], O[(i + 1) % 3] } };
			solver = Matrix(se);
		}

		vector<vector<float>> inverse = solver.inverse().allElements();
		float x = N[0] * A[0] + N[1] * A[1] + N[2] * A[2];
		float y = O[0] * B[0] + O[1] * B[1] + O[2] * B[2];
		vector<float> intersection = { inverse[0][0] * x + inverse[0][1] * y , inverse[1][0] * x + inverse[1][1] * y };
		vector<float> _anchor;
		for (int j = 1; j <= 3; j++) {
			_anchor.push_back((i == j) ? 0 : intersection[(j + (5 - i) % 3) % 3]);
		}
		result = new Line(_anchor, _direction);
	}
}

Vector* Plane::pointClosestTo(const Vector& point) const {
	vector<float> P = point.allElements();
	vector<float> A = anchor->allElements();
	vector<float> N = normal->allElements();
	float dot = (A[0] - P[0]) * N[0] + (A[1] - P[1]) * N[1] + (A[2] - (P[2] || 0)) * N[2];
	vector<float> newV = { P[0] + N[0] * dot, P[1] + N[1] * dot, (P[2] || 0) + N[2] * dot };
	return new Vector(newV);
}

Plane Plane::rotate(const float& t, const Line& line) const {
	vector<vector<float>> R = Matrix().rotation(t, *line.getDirection()).allElements();
	vector<float> C = line.pointClosestTo(*anchor)->allElements();
	vector<float> A = anchor->allElements();
	vector<float> N = normal->allElements();
	float C1 = C[0], C2 = C[1], C3 = C[2], A1 = A[0], A2 = A[1], A3 = A[2];
	float x = A1 - C1, y = A2 - C2, z = A3 - C3;
	vector<float> _anchor = { 
		C1 + R[0][0] * x + R[0][1] * y + R[0][2] * z,
		C2 + R[1][0] * x + R[1][1] * y + R[1][2] * z,
		C3 + R[2][0] * x + R[2][1] * y + R[2][2] * z };
	vector<float> _normal = {
		R[0][0] * N[0] + R[0][1] * N[1] + R[0][2] * N[2],
		R[1][0] * N[0] + R[1][1] * N[1] + R[1][2] * N[2],
		R[2][0] * N[0] + R[2][1] * N[1] + R[2][2] * N[2]};
	return Plane(_anchor, _normal);
}

Plane Plane::reflectionIn(SylPrimitive* obj) const {
	if (obj->instanceof("plane")) {
		vector<float> A = anchor->allElements();
		vector<float> N = anchor->allElements();
		float A1 = A[0], A2 = A[1], A3 = A[2], N1 = N[0], N2 = N[1], N3 = N[2];
		vector<float> newA = anchor->reflectionIn(obj).allElements();
		float AN1 = A1 + N1, AN2 = A2 + N2, AN3 = A3 + N3;
		vector<float> AN = { AN1, AN2, AN3 };
		vector<float> Q = obj->pointClosestTo(Vector(AN))->allElements();
		vector<float> newN = { Q[0] + (Q[0] - AN1) - newA[0], Q[1] + (Q[1] - AN2) - newA[1], Q[2] + (Q[2] - AN3) - newA[2] };
		return Plane(newA, newN);
	}
	else if (obj->instanceof("line")) {
		Line *l = dynamic_cast<Line*>(obj);
		return rotate(M_PI, *l);
	}
	return Plane();
}

Plane Plane::reflectionIn(const Vector& point) const {
	vector<float> P = point.allElements();
	Vector p(P);
	Vector _anchor = anchor->reflectionIn(p);
	return Plane(&_anchor, normal);
}

Plane Plane::reflectionIn(const Vector* point) const {
	return reflectionIn(*point);
}