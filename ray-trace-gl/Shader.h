#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <Gl/glew.h>
#include <GL/glut.h>
#include <string>
#include <string.h>
#include <iostream>
#include "hitable.h"
extern std::string renderVertexSource;
extern std::string renderFragmentSource;
extern std::string lineVertexSource;
extern std::string lineFragmentSource;
extern std::string bounces;
extern std::string epsilon;
extern std::string infinity;
extern float lightSize;
extern float lightVal;
extern std::string tracerVertexSource;
extern std::string tracerFragmentSourceHeader;
extern std::string intersectCubeSource;
extern std::string normalForCubeFunction;
extern std::string normalForCubeSource;
extern std::string intersectSphereSource;
extern std::string normalForSphereSource;
extern std::string randomSource;
extern std::string cosineWeightDirectionSource;
extern std::string uniformlyRandomDirectionSource;
extern std::string uniformlyRandomVectorSource;
extern std::string specularReflection;
extern std::string newDiffuseRay;
extern std::string newReflectiveRay;
extern std::string newGlossyRay;
extern std::string yellowBlueCornellBox;
extern std::string redGreenCornellBox;
std::string makeShadow(vector<Hitable*> objects);
std::string makeCalculateColor(vector<Hitable*> objects);
std::string makeMain();
extern std::string makeTracerFragmentSource(vector<Hitable*> objects);
GLuint compileSource(const std::string& source, GLuint type);
extern GLuint compileShader(const std::string& vertexSource, const std::string& fragmentSource);
#endif // !SHADER_H