#pragma once
#ifndef CUBE_H
#define CUBE_H
#include "hitable.h"
class PathTracer;
class Cube : public Hitable {
private:
	Vector minCorner;
	Vector maxCorner;
	std::string minStr;
	std::string maxStr;
	std::string intersectStr;
	Vector temporaryTranslation;
	std::string id;
public:
	Cube();
	Cube(const Vector& _minCorner, const Vector& _maxCorner, const std::string& id);
	~Cube();
	Vector getMinCorner() const;
	Vector getMaxCorner() const;
	std::string getMinStr() const;
	std::string getMaxStr() const;
	std::string getIntersectStr() const;
	Vector getTemporaryTranslation() const;
	std::string getId() const;
	virtual string getGlobalCode() const;
	virtual string getIntersectCode() const;
	virtual string getShadowTestCode() const;
	virtual string getMinimumIntersectCode() const;
	virtual string getNormalCalculationCode() const;
	void setUniforms(PathTracer* pt);
	virtual void temporaryTranslate(const Vector& translation);
	virtual void translate(const Vector& translation);
	virtual float intersect(const Vector& origin, const Vector& ray) const;
	float dointersect(const Vector& origin, const Vector& ray, const Vector& cubeMin, const Vector& cubeMax) const;
};
#endif // !CUBE_H
