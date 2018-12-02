#pragma once
#ifndef RENDERER_H
#define RENDERER_H
#include <GL/glew.h>
#include <GL/glut.h>
#include "sylvester_matrix.h"
#include <vector>
#include "hitable.h"
#include "drand.h"
class PathTracer;
class Renderer {
private:
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint lineProgram;
	vector<Hitable*> objects;
	PathTracer* pathTracer;
	GLuint vertexAttribute;
	Matrix modelviewProjection;
	Hitable* selectedObject;
	void setUniforms();
public:
	struct Uniforms {
		Vector cubeMin;
		Vector cubeMax;
		Matrix modelviewProjection;
	}uniforms;
	Renderer();
	~Renderer();
	void setObjects(vector<Hitable*> _objects);
	void setObjects(Hitable* _objects[], int n);
	void update(const Matrix& modelviewProjection, float timeSinceStart);
	void render();
	GLuint getVertexBuffer() const;
	GLuint getIndexBuffer() const;
	GLuint getLineProgram() const;
	GLuint getVertexAttribute() const;
	Matrix getModelViewProjection() const;
	void setModelViewProjection(const Matrix& mvp);
	Hitable getSelectedObject() const;
	void setSelectedObject(Hitable& object);
};
#endif // !RENDERER_H