#include "Sphere.h"
#include "PathTracer.h"
#include <iostream>

Sphere::Sphere() {
	vector<float> c = { 1.0, 1.0, 1.0 };
	center = Vector(c);
	radius = 0;
	centerStr = "sphereCenter";
	radiusStr = "sphereRadius";
	intersectStr = "tSphere";
	temporaryTranslation = Vector().zero(3);
	id = "";
}

Sphere::Sphere(const Vector& _center, const float& _radius, const std::string& _id) {
	center = _center;
	radius = _radius;
	id = _id;
	centerStr = "sphereCenter" + _id;
	radiusStr = "sphereRadius" + _id;
	intersectStr = "tSphere" + _id;
	temporaryTranslation = Vector().zero(3);
}

Sphere::~Sphere() {

}

Vector Sphere::getCenter() const { 
	return center;
}

float  Sphere::getRadius() const {
	return radius;
}

string Sphere::getCenterStr() const {
	return centerStr;
}

string Sphere::getRadiusStr() const {
	return radiusStr;
}

Vector Sphere::getTemporaryTranslation() const {
	return temporaryTranslation;
}

string Sphere::getId() const {
	return id;
}

string Sphere::getGlobalCode() const {
	const string uv3 = "uniform vec3 ";
	const string uf = "uniform float ";
	return ""+ uv3 + this->centerStr + ";\n" + uf + this->radiusStr + ";\n";
}

string Sphere::getIntersectCode() const {
	const string flt = "float ";
	return "" + 
		flt + this->intersectStr + " = intersectSphere(origin, ray, " + 
		this->centerStr + ", " + this->radiusStr + "); \n";
}

string Sphere::getShadowTestCode() const {
	return "" + 
	this->getIntersectCode() + 
	"if( " + this->intersectStr + " < 1.0) return 0.0; \n";
}

string Sphere::getMinimumIntersectCode() const {
	const string first = " if(";
	return "" +
		first + this->intersectStr + " < t) t = " + this->intersectStr + "; \n";
}

string Sphere::getNormalCalculationCode() const {
	const string first = "else if(t == ";
	return "" + 
		first + this->intersectStr + ") normal = normalForSphere(hit, " + this->centerStr + ", " + this->radiusStr + "); \n";
}

void Sphere::setUniforms(PathTracer* pt) {
	GLuint centerLocation = glGetUniformLocation(pt->getTracerProgram(), centerStr.c_str());
	GLuint radiusLocation = glGetUniformLocation(pt->getTracerProgram(), radiusStr.c_str());
	if (centerLocation != 0xffffffff) {
		Vector _center = center.add(temporaryTranslation);
		glUniform3f(centerLocation, _center.x(), _center.y(), _center.z());
	}
	if (radiusLocation != 0xffffffff) {
		glUniform1f(radiusLocation, radius);
	}
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "something wrong in sphere uniforms: "<< err << endl;
	}
}

void Sphere::temporaryTranslate(const Vector& translation) {
	temporaryTranslation = translation;
}

void Sphere::translate(const Vector& translation) {
	center = center.add(translation);
}

Vector Sphere::getMinCorner() const {
	vector<float> r = { radius, radius, radius };
	Vector rd(r);
	return center.add(this->temporaryTranslation).substract(rd);
}

Vector Sphere::getMaxCorner() const {
	vector<float> r = { radius, radius, radius };
	Vector rd(r);
	return center.add(this->temporaryTranslation).add(rd);
}

float Sphere::intersect(const Vector& origin, const Vector& ray) const {
	return dointersect(origin, ray, center.add(temporaryTranslation), radius);
}

float Sphere::dointersect(const Vector& origin, const Vector& ray, const Vector& _center, const float& _radius) const {
	Vector toSphere = origin.substract(_center);
	float a = ray.dot(ray);
	float b = 2.0 * toSphere.dot(ray);
	float c = toSphere.dot(toSphere) - _radius * _radius;
	float discriminant = b * b - 4 * a*c;
	if (discriminant > 0) {
		float t = (-b - sqrt(discriminant)) / (2.0 * a);
		if (t > 0) {
			return t;
		}
	}
	return FLT_MAX;
}