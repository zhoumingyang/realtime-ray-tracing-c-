#include "Cube.h"
#include "PathTracer.h"
#include <iostream>

Cube::Cube() {
	minCorner = Vector(-0.5, -0.5, -0.5);
	maxCorner = Vector(0.5, 0.5, 0.5);
	minStr = "cubeMin";
	maxStr = "cubeMax";
	intersectStr = "tCube";
	temporaryTranslation = Vector(0, 0, 0);
}

Cube::Cube(const Vector& _minCorner, const Vector& _maxCorner, const std::string& id) {
	minCorner = _minCorner;
	maxCorner = _maxCorner;
	minStr = "cubeMin" + id;
	maxStr = "cubeMax" + id;
	intersectStr = "tCube" + id;
	temporaryTranslation = Vector(0, 0, 0);
}

Cube::~Cube() {

}

std::string Cube::getMinStr() const {
	return minStr;
}

std::string Cube::getMaxStr() const {
	return maxStr;
}

std::string Cube::getIntersectStr() const {
	return intersectStr;
}

Vector Cube::getTemporaryTranslation() const {
	return temporaryTranslation;
}

std::string Cube::getId() const {
	return id;
}

std::string Cube::getGlobalCode() const {
	const std::string uv3 = "uniform vec3 ";
	return "" + 
		uv3 + minStr + "; \n" + 
		uv3 + maxStr + "; \n";
}

std::string Cube::getIntersectCode() const {
	const std::string vc2 = "vec2 ";
	return "" + vc2 + intersectStr + " = intersectCube(origin, ray, " + minStr+ ", " + maxStr + "); \n";
}

std::string Cube::getShadowTestCode() const {
	return "" +
		getIntersectCode() +
		" if(" + intersectStr + ".x > 0.0 && " + intersectStr + ".x < 1.0 && " 
		+ intersectStr + ".x < " + intersectStr + ".y) return 0.0; \n";
}

std::string Cube::getMinimumIntersectCode() const {
	const std::string first = " if( ";
	return "" +
		first + this->intersectStr + ".x > 0.0 && " + this->intersectStr + ".x < " + 
		this->intersectStr + ".y && " + this->intersectStr + ".x < t) t = " + this->intersectStr + ".x; \n";
}

std::string Cube::getNormalCalculationCode() const {
	const std::string first = " else if(t == ";
	return "" +
		first + this->intersectStr + ".x && " + this->intersectStr + ".x < " + this->intersectStr + 
		".y) normal = normalForCube(hit, " + this->minStr + ", " + this->maxStr + "); \n";
}

void Cube::setUniforms(PathTracer* pt) {
	GLuint minStrLocation = glGetUniformLocation(pt->getTracerProgram(), minStr.c_str());
	GLuint maxStrLocation = glGetUniformLocation(pt->getTracerProgram(), maxStr.c_str());
	if (minStrLocation != 0xffffffff) {
		glUniform3f(minStrLocation, minCorner.x(), minCorner.y(), minCorner.z());
	}
	if (maxStrLocation != 0xffffffff) {
		glUniform3f(maxStrLocation, maxCorner.x(), maxCorner.y(), maxCorner.z());
	}
	if (glGetError() != GL_NO_ERROR) {
		std::cout << "something wrong in set cube uniforms" << endl;
	}
}

void Cube::temporaryTranslate(const Vector& translation) {
	temporaryTranslation = translation;
}

void Cube::translate(const Vector& translation) {
	minCorner = minCorner.add(translation);
	maxCorner = maxCorner.add(translation);
}

Vector Cube::getMinCorner() const {
	return minCorner.add(temporaryTranslation);
}

Vector Cube::getMaxCorner() const {
	return maxCorner.add(temporaryTranslation);
}

float Cube::intersect(const Vector& origin, const Vector& ray) const {
	return dointersect(origin, ray, getMinCorner(), getMaxCorner());
}

float Cube::dointersect(const Vector& origin, const Vector& ray, const Vector& cubeMin, const Vector& cubeMax) const {
	Vector tMin = cubeMin.substract(origin).componentDivide(ray);
	Vector tMax = cubeMax.substract(origin).componentDivide(ray);
	Vector t1 = Vector().min(tMin, tMax);
	Vector t2 = Vector().max(tMin, tMax);
	float tNear = t1.max();
	float tFar  = t2.max();
	if (tNear > 0 && tNear < tFar) {
		return tNear;
	}
	return FLT_MAX;
}