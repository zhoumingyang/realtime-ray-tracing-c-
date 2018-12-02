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
	Vector getEyeRay(const Matrix& matrix, const float& x, const float& y);
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
	PathTracer();
	~PathTracer();
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
};
#endif // !PATHTRACER_H
