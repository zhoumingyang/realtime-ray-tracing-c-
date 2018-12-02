#pragma once
#ifndef LIGHT_H
#define LIGHT_H
#include "hitable.h"
class PathTracer;
class Light : public Hitable {
private:
	Vector temporaryTranslation;
public:
	Light();
	~Light();
	virtual std::string getGlobalCode() const;
	virtual std::string getIntersectCode() const;
	virtual std::string getShadowTestCode() const;
	virtual std::string getMinimumIntersectCode() const;
	virtual std::string getNormalCalculationCode() const;
	void setUniforms(PathTracer* pt);
	static void clampPosition(Vector& position);
	virtual void temporaryTranslate(const Vector& translation);
	virtual void translate(const Vector& translation);
	virtual Vector getMinCorner() const;
	virtual Vector getMaxCorner() const;
	virtual float intersect(const Vector& origin, const Vector& ray) const;
};
#endif // !LIGHT_H