#pragma once
#ifndef PATHTRACER_H
#define PATHTRACER_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include "hitable.h"

class PathTracer {
private:
	GLuint vertexBuffer;
	GLuint frameBuffer;
	GLuint textures[2];
	GLuint renderProgram;
	GLuint tracerProgram;
	GLuint renderVertexAttribute;
	GLuint tracerVertexAttribute;
	int sampleCount;
	vector<Hitable*> objects;
	void setUniforms();
public:
	struct Uniforms {
		Vector eye;
		float glossiness;
		Vector ray00;
		Vector ray01;
		Vector ray10;
		Vector ray11;
		float timeSinceStar;
		float textureWeight;
	}uniforms;
	int material;
	PathTracer();
	~PathTracer();
	Vector getEyeRay(const Matrix& matrix, const float& x, const float& y);
	void setObjects(vector<Hitable*> _objects);
	void setObjects(Hitable* _objects[], int n);
	void update(const Matrix& matrix, float timeSinceStart);
	void render();
	GLuint getVertexBuffer() const;
	GLuint getFrameBuffer() const;
	GLuint getRenderProgram() const;
	GLuint getTracerProgram() const;
	GLuint getRenderVertexAttribute() const;
	GLuint getTracerVertexAttribute() const;
	int getSampleCount() const;
	void setMaterial(int _material);
	int getMaterial() const;
};
#endif // !PATHTRACER_H
