#include "Light.h"
#include "PathTracer.h"
#include <math.h>
#include <iostream>
Vector light(0.4, 0.5, -0.6);
const float lightSize = 0.1;

Light::Light() {
	temporaryTranslation = Vector(0.0, 0.0, 0.0);
}

Light::~Light() {

}

std::string Light::getGlobalCode() const {
	return "uniform vec3 light; \n";
};

std::string Light::getIntersectCode() const {
	return "";
}

std::string Light::getShadowTestCode() const {
	return "";
}

std::string Light::getMinimumIntersectCode() const {
	return "";
}

std::string Light::getNormalCalculationCode() const {
	return "";
}

void Light::setUniforms(PathTracer* pt) {
	GLuint lightLocation = glGetUniformLocation(pt->getTracerProgram(), "light");
	if (lightLocation != 0xffffffff) {
		Vector _light = light.add(temporaryTranslation);
		glUniform3f(lightLocation, _light.x(), _light.y(), _light.z());
	}
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "something wrong in set light uniforms: "<< err << endl;
	}
}

void Light::clampPosition(Vector& position) {
	vector<float> elements = position.allElements();
	for (int i = 0; i < elements.size(); i++) {
		elements[i] = std::max(lightSize - 1, std::min(lightSize - 1, elements[i]));
	}
	position = Vector(elements);
}

void Light::temporaryTranslate(const Vector& translation) {
	Vector tempLight = light.add(translation);
	Light::clampPosition(tempLight);
	temporaryTranslation = tempLight.substract(light);
}

void Light::translate(const Vector& translation) {
	light = light.add(translation);
	Light::clampPosition(light);
}

Vector Light::getMinCorner() const {
	Vector tmp = Vector(lightSize, lightSize, lightSize);
	return light.add(temporaryTranslation).substract(tmp);
}

Vector Light::getMaxCorner() const {
	Vector tmp = Vector(lightSize, lightSize, lightSize);
	return light.add(temporaryTranslation).add(tmp);
}

float Light::intersect(const Vector& origin, const Vector& ray) const {
	return FLT_MAX;
}