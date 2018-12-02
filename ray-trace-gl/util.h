#pragma once
#ifndef UTIL_H
#define UTIL_H
#include <gl/glut.h>
#include <gl/glew.h>
#include "sylvester_matrix.h"
#include "sylvester_vector.h"
#include <vector>

Vector getEyeRay(const Matrix& matrix, float x, float y) {
	vector<float> tmp = {x, y, 0, 1};
	return matrix.multiply(Vector(tmp)).divideByW().substract(Vector(0, 0, 0));
}


#endif // !UTIL_H
