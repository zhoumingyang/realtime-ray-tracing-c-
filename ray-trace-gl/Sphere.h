#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include "hitable.h"
class PathTracer;
class Sphere:public Hitable {
public:
	Sphere();
	Sphere(const Vector& _center, const float& _radius, const std::string& _id);
	~Sphere();
	Vector getCenter() const;
	float getRadius() const;
	string getCenterStr() const;
	string getRadiusStr() const;
	Vector getTemporaryTranslation() const;
	string getId() const;
	virtual string getGlobalCode() const;
	virtual string getIntersectCode() const;
	virtual string getShadowTestCode() const;
	virtual string getMinimumIntersectCode() const;
	virtual string getNormalCalculationCode() const;
	void setUniforms(PathTracer* pt);
	virtual void temporaryTranslate(const Vector& translation);
	virtual void translate(const Vector& translation);
	virtual Vector getMinCorner() const;
	virtual Vector getMaxCorner() const;
	virtual float intersect(const Vector& origin, const Vector& ray) const;
	float dointersect(const Vector& origin, const Vector& ray, const Vector& _center, const float& _radius) const;
private:
	Vector center;
	float radius;
	std::string centerStr;
	std::string radiusStr;
	std::string intersectStr;
	Vector temporaryTranslation;
	std::string id;
};
#endif // !SPHERE_H
