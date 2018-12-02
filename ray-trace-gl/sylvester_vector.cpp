#include "sylvester_primitive.h"
#include "sylvester_line.h"
#include "sylvester_vector.h"
#include "sylvester_matrix.h"
#include "drand.h"

inline vector<float> doSlice(vector<float>::iterator first, vector<float>::iterator last) {
	vector<float> newV;
	for (vector<float>::iterator cur = first; cur != last; cur++) {
		newV.push_back((*cur));
	}
	return newV;
}

Vector Vector::I = Vector(vector<float>{ 1, 0, 0 });
Vector Vector::J = Vector(vector<float>{ 0, 1, 0 });
Vector Vector::K = Vector(vector<float>{ 0, 0, 1 });

Vector::Vector() {

}

Vector::Vector(vector<float> v) {
	elements = v;
}

Vector::Vector(float v[], int n) {
	for (int i = 0; i < n; i++) {
		elements.push_back(v[i]);
	}
}

Vector::Vector(float x, float y, float z) {
	vector<float> v = { x, y, z };
	elements = v;
}

Vector::~Vector() {

}

void Vector::addElement(const float& x) {
	elements.push_back(x);
}

void Vector::popElement() {
	elements.pop_back();
}

float Vector::x() const {
	return elements[0];
}

float Vector::y() const {
	return elements[1];
}

float Vector::z() const {
	return elements[2];
}

float  Vector::e(int i) const {
	return (i < 1 || i > elements.size()) ? NULL : elements[i - 1];
}

int  Vector::dimensions() const {
	return elements.size();
}

vector <float> Vector::allElements() const {
	return elements;
}

float Vector::dot(const Vector* vec) const {
	if (vec->dimensions() != elements.size()) { return NULL; }
	int n = elements.size();
	float product = 0.0;
	vector<float> V = vec->allElements();
	do {
		product += elements[n - 1] * V[n - 1];
	} while (--n);
	return product;
}

float Vector::dot(const Vector& vec) const {
	if (vec.dimensions() != elements.size()) { return NULL; }
	int n = elements.size();
	float product = 0.0;
	vector<float> V = vec.allElements();
	do {
		product += elements[n - 1] * V[n - 1];
	} while (--n);
	return product;
}

float Vector::moduluse() const {
	return sqrt(this->dot(this));
}

void Vector::normalize() {
	float l = this->length();
	if (l <= 0.0000001) { return; }
	for (int i = 0; i < elements.size(); i++) {
		elements[i] = elements[i] / l;
	}
}

float Vector::length() {
	float sum = 0.0;
	for (int i = 0; i < elements.size(); i++) {
		sum += elements[i] * elements[i];
	}
	return sqrt(sum);
}

bool Vector::eql(const Vector& vec) const {
	int n = elements.size();
	if (vec.dimensions() != n) { return false; }
	vector<float> V = vec.allElements();
	do {
		if (abs(elements[n - 1] - V[n - 1]) > SYLVESTER_RRECISION) { return false; }
	} while (--n);
	return true;
}

Vector Vector::dup() const {
	return Vector(this->elements);
}

void Vector::each(void(*f)(float e, int idx)) {
	int n = elements.size(), k = n, i;
	do {
		i = k - n;
		f(elements[i], i + 1);
	} while (--n);
}

Vector Vector::map(float(*f)(float e, int idx)) {
	vector<float> ele;
	for (int i = 0; i < elements.size(); i++) {
		ele.push_back(f(elements[i], i));
	}
	return Vector(ele);
}

Vector Vector::toUnitVector() const {
	float l = this->moduluse();
	vector<float> newV;
	for (int i = 0; i < elements.size(); i++) {
		newV.push_back(elements[i] / l);
	}
	return Vector(newV);
}

float Vector::angleFrom(const Vector& vec) const {
	int n = this->dimensions();
	vector<float> V = vec.allElements();
	if (n != vec.dimensions()) { return NULL; }
	float dot = 0.0, mod1 = 0.0, mod2 = 0.0;
	for (int i = 0; i < elements.size(); i++) {
		dot += elements[i] * V[i];
		mod1 += elements[i] * elements[i];
		mod2 += V[i] * V[i];
	}
	mod1 = sqrt(mod1);
	mod2 = sqrt(mod2);
	if ((mod1 * mod2) == 0) { return NULL; }
	float theta = dot / (mod1 * mod2);
	if (theta < -1.0) { theta = -1.0; }
	if (theta > 1.0) { theta = 1.0; }
	return acos(theta);
}

float Vector::angleFrom(const Vector* vec) const {
	if (vec == NULL) {
		return NULL;
	}
	return angleFrom(*vec);
}

bool Vector::isParallelTo(const Vector& vec) const {
	float angle = this->angleFrom(vec);
	return (angle == NULL) ? NULL : (angle <= SYLVESTER_RRECISION);
}

bool Vector::isAntiparallelTo(const Vector& vec) const {
	float angle = this->angleFrom(vec);
	return (angle == NULL) ? NULL : (abs(angle - M_PI) <= SYLVESTER_RRECISION);
}

bool Vector::isPerpendicularTo(const Vector& vec) const {
	float dot = this->dot(vec);
	return (dot == NULL) ? NULL : (abs(dot) <= SYLVESTER_RRECISION);
}

Vector Vector::add(const Vector& vec) const {
	vector<float> V = vec.allElements();
	int n = elements.size();
	if (n != vec.dimensions()) { return Vector(); }
	vector<float> newV;
	for (int i = 0; i < n; i++) {
		float tmp = elements[i] + V[i];
		newV.push_back(tmp);
	}
	return Vector(newV);
}

Vector Vector::substract(const Vector& vec) const {
	vector<float> V = vec.allElements();
	int n = elements.size();
	if (n != vec.dimensions()) { return Vector(); }
	vector<float> newV;
	for (int i = 0; i < n; i++) {
		float tmp = elements[i] - V[i];
		newV.push_back(tmp);
	}
	return Vector(newV);
}

Vector Vector::multiply(float num) const {
	vector<float> newV;
	for (int i = 0; i < elements.size(); i++) {
		float tmp = elements[i] * num;
		newV.push_back(tmp);
	}
	return Vector(newV);
}

Vector Vector::cross(const Vector& vec) const {
	vector<float>V = vec.allElements();
	if (this->dimensions() != 3 || vec.dimensions() != 3) { return Vector(); }
	vector<float> newV = {
		(elements[1] * V[2]) - (elements[2] * V[1]),
		(elements[2] * V[0]) - (elements[0] * V[2]),
		(elements[0] * V[1]) - (elements[1] * V[0])
	};
	return Vector(newV);
}

Vector Vector::cross(const Vector* vec) const {
	if (vec == NULL) {
		return Vector();
	}
	return cross(*vec);
}

float Vector::max() const {
	auto p = max_element(elements.begin(), elements.end());
	return *p;
}

float Vector::min() const {
	auto p = min_element(elements.begin(), elements.end());
	return *p;
}

Vector Vector::min(const Vector& a, const Vector& b) const {
	vector<float> _a = a.allElements();
	vector<float> _b = b.allElements();
	if (_a.size() != _b.size()) {
		return Vector();
	}

	vector<float> newV;
	for (int i = 0; i < _a.size(); i++) {
		newV.push_back(std::min(_a[i], _b[i]));
	}
	return Vector(newV);
}

Vector Vector::max(const Vector& a, const Vector& b) const {
	vector<float> _a = a.allElements();
	vector<float> _b = b.allElements();
	if (_a.size() != _b.size()) {
		return Vector();
	}

	vector<float> newV;
	for (int i = 0; i < _a.size(); i++) {
		newV.push_back(std::max(_a[i], _b[i]));
	}
	return Vector(newV);
}

int Vector::indexOf(const float& x) const {
	vector<float> a = this->allElements();
	vector<float>::iterator it = find(a.begin(), a.end(), x);
	if (it == elements.end()) {
		return -1;
	}
	return it - elements.begin();
}

Matrix Vector::toDiagonalMatrix() const {
	int n = elements.size();
	vector<vector<float>> res(n, vector<float>(n, 0));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			res[i][j] = (i == j) ? elements[i] : 0.0;
		}
	}
	return Matrix(res);
}

Vector Vector::roundAll() const {
	vector<float> a = this->allElements();
	vector<float> newV;
	for (vector<float>::iterator it = a.begin(); it != a.end(); it++) {
		newV.push_back(round(*it));
	}
	return Vector(newV);
}

Vector Vector::snapTo(const float& x) const {
	vector<float> a = this->allElements();
	vector<float> newV;
	for (vector<float>::iterator it = a.begin(); it != a.end(); it++) {
		float tmp = abs((*it) - x) ? x : (*it);
		newV.push_back(tmp);
	}
	return Vector(newV);
}

float Vector::distanceFrom(const SylPrimitive* obj) const {
	if (obj->instanceof("plane") || obj->instanceof("line")) {
		Vector pt = this->dup();
		return obj->distanceFrom(pt);
	}
	return NULL;
}

float Vector::distanceFrom(const Vector& obj) const {
	vector<float> el = obj.allElements();
	float sum = 0.0, part;
	if (el.size() != elements.size()) {
		return NULL;
	}
	for (int i = 0; i < el.size(); i++) {
		part = elements[i] - el[i];
		sum += part * part;
	}
	return sqrt(sum);
}

bool Vector::liesOn(const Line& line) const {
	Vector pt = this->dup();
	return line.contains(pt);
}

//2D
Vector Vector::rotate(const float& t, const Vector& point) const {
	vector<float> V = point.allElements();
	if (V.size() != 2) {
		return Vector();
	}
	Vector axis;
	Matrix rotMat = Matrix().rotation(t, axis);
	vector<vector<float>> R = rotMat.allElements();
	float x = elements[0] - V[0];
	float y = elements[1] - V[1];
	vector<float> newV = { V[0] + R[0][0] * x + R[0][1] * y, V[1] + R[1][0] * x + R[1][1] * y };
	return Vector(newV);
}

Vector Vector::rotate(const float& t, const SylPrimitive* obj) const {
	if (obj->instanceof("line")) {
		vector<float> C = obj->pointClosestTo(*this)->allElements();
		vector<vector<float>>R = Matrix().rotation(t, *obj->getDirection()).allElements();
		float x = elements[0] - C[0];
		float y = elements[1] - C[1];
		float z = elements[2] - C[2];
		vector<float> newV = {
			C[0] + R[0][0] * x + R[0][1] * y + R[0][2] * z,
			C[1] + R[1][0] * x + R[1][1] * y + R[1][2] * z,
			C[2] + R[2][0] * x + R[2][1] * y + R[2][2] * z
		};
		return Vector(newV);
	}
	return Vector();
}

Vector Vector::rotate(const float& t, const Line& line) const {
	vector<float> C = line.pointClosestTo(*this)->allElements();
	vector<vector<float>>R = Matrix().rotation(t, *line.getDirection()).allElements();
	float x = elements[0] - C[0];
	float y = elements[1] - C[1];
	float z = elements[2] - C[2];
	vector<float> newV = {
		C[0] + R[0][0] * x + R[0][1] * y + R[0][2] * z,
		C[1] + R[1][0] * x + R[1][1] * y + R[1][2] * z,
		C[2] + R[2][0] * x + R[2][1] * y + R[2][2] * z
	};
	return Vector(newV);
}

Vector Vector::reflectionIn(const SylPrimitive* obj) const {
	vector<float> P = elements;
	if (P.size() == 2) {
		P.push_back(0.0);
	}
	Vector point(P);
	Vector* _C = obj->pointClosestTo(point);
	vector<float> C = _C->allElements();
	vector<float> v = {
		C[0] + (C[0] - P[0]),
		C[1] + (C[1] - P[1]),
		C[2] + (C[2] - P[2])
	};
	return Vector(v);
}

Vector Vector::reflectionIn(const Vector& point) const {
	vector<float> newV;
	vector<float> Q = point.allElements();
	if (Q.size() != elements.size()) {
		return Vector();
	}
	for (int i = 0; i < elements.size(); i++) {
		newV.push_back((Q[i - 1] + (Q[i - 1] - elements[i])));
	}
	return Vector(newV);
}

Vector Vector::slice(const int& first, const int& last) const {
	if (last <= first || first < 0 || last < 0) {
		return Vector();
	}
	vector<float> a = this->allElements();
	vector<float> n = doSlice(a.begin() + first, a.begin() + last);
	return Vector(n);
}

Vector Vector::to3D() const {
	Vector vec = this->dup();
	switch (vec.dimensions()) {
	case 3:
		break;
	case 2:
		vec.addElement(0.0);
		break;
	default:
		return vec.slice(0, 3);
	}
	return vec;
}

void Vector::setElements(Vector& vec) {
	elements = vec.allElements();
}

void Vector::setElements(vector<float>& vec) {
	elements = vec;
}

Vector Vector::zero(int n) const {
	vector<float> newV;
	do {
		newV.push_back(0.0);
	} while (--n);
	return Vector(newV);
}

Vector Vector::rand(int n) const {
	vector<float> newV;
	do {
		newV.push_back(drand48());
	} while (--n);
	return Vector(newV);
}

Vector Vector::ensure3() const {
	vector<float> newV = { elements[0], elements[1], elements[2] };
	return Vector(newV);
}

Vector Vector::ensure4(const float& w) const {
	vector<float> newV = { elements[0], elements[1], elements[2], w };
	return Vector(newV);
}

Vector Vector::divideByW() const {
	float w = elements[elements.size() - 1];
	vector<float> newV;
	for (int i = 0; i < elements.size(); i++) {
		newV.push_back(elements[i] / w);
	}
	return Vector(newV);
}

Vector Vector::componentDivide(const Vector& v) const {
	vector<float> tmp = v.allElements();
	if (elements.size() != tmp.size()) {
		return Vector();
	}
	vector<float> newV;
	for (int i = 0; i < elements.size(); i++) {
		newV.push_back(elements[i] / tmp[i]);
	}
	return Vector(newV);
}