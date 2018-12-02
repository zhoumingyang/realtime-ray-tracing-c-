#include "Renderer.h"
#include "PathTracer.h"
#include "Shader.h"

Renderer::Renderer() {
	GLfloat vertices[] = {
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		1.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,
		1.0, 1.0, 1.0
	};

	GLuint indices[] = {
		0, 1, 1, 3, 3, 2, 2, 0,
		4, 5, 5, 7, 7, 6, 6, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glCreateBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	lineProgram = compileShader(lineVertexSource, lineFragmentSource);
	vertexAttribute = glGetAttribLocation(lineProgram, "vertex");
	glVertexAttribPointer(vertexAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(vertexAttribute);
	selectedObject = NULL;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "something wrong in renderer init: " << err << endl;
	}
	pathTracer = new PathTracer();
}

Renderer::~Renderer() {
	if (pathTracer != NULL) {
		delete pathTracer;
	}
	if (vertexBuffer != 0xffffffff) {
		glDeleteBuffers(1, &vertexBuffer);
	}
	if (indexBuffer != 0xffffffff) {
		glDeleteBuffers(1, &indexBuffer);
	}
}

void Renderer::setObjects(vector<Hitable*> _objects) {
	objects = _objects;
	selectedObject = _objects[1];
	pathTracer->setObjects(_objects);
}

void Renderer::setObjects(Hitable* _objects[], int n) {
	for (int i = 0; i < n; i++) {
		objects.push_back(_objects[i]);
	}
	pathTracer->setObjects(_objects, n);
}

void Renderer::update(const Matrix& _modelviewProjection, float timeSinceStart) {
	Vector trans = Vector(drand48() * 2.0 - 1.0, drand48() * 2.0 - 1.0, 0);
	Vector tmpTrans = trans.multiply(1.0 / 512.0);
	Matrix jitter = Matrix().translation(tmpTrans);
	Matrix tmpMatrix = jitter.multiply(_modelviewProjection);
	Matrix inverse = tmpMatrix.inverse();
	modelviewProjection = _modelviewProjection;
	pathTracer->update(inverse, timeSinceStart);
}

void Renderer::render() {
	pathTracer->render();
	if (selectedObject != NULL) {
		glUseProgram(lineProgram);
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnableVertexAttribArray(vertexAttribute);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		uniforms.cubeMin = selectedObject->getMinCorner();
		uniforms.cubeMax = selectedObject->getMaxCorner();
		uniforms.modelviewProjection = modelviewProjection;
		setUniforms();
		glVertexAttribPointer(vertexAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(vertexAttribute);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Renderer::setUniforms() {
	GLuint cubeMaxLocation = glGetUniformLocation(lineProgram, "cubeMax");
	GLuint cubeMinLocation = glGetUniformLocation(lineProgram,"cubeMin");
	GLuint MVPLocation = glGetUniformLocation(lineProgram, "modelviewProjection");
	if (cubeMinLocation != 0xffffffff) {
		glUniform3f(cubeMinLocation, uniforms.cubeMin.x(), uniforms.cubeMin.y(), uniforms.cubeMin.z());
	}
	if (cubeMaxLocation != 0xffffffff) {
		glUniform3f(cubeMaxLocation, uniforms.cubeMax.x(), uniforms.cubeMax.y(), uniforms.cubeMax.z());
	}
	if (MVPLocation != 0xffffffff) {
		vector<float> f = uniforms.modelviewProjection.flatten();
		glUniformMatrix4fv(MVPLocation, 1, false, &f[0]);
	}
	if (glGetError() != GL_NO_ERROR) {
		std::cout << "something wrong in renderer" << endl;
	}
}

GLuint Renderer::getVertexBuffer() const {
	return vertexBuffer;
}

GLuint Renderer::getIndexBuffer() const {
	return indexBuffer;
}

GLuint Renderer::getLineProgram() const {
	return lineProgram;
}

GLuint Renderer::getVertexAttribute() const {
	return vertexAttribute;
}

Matrix Renderer::getModelViewProjection() const {
	return modelviewProjection;
}

void Renderer::setModelViewProjection(const Matrix& mvp) {
	modelviewProjection = mvp;
}

Hitable Renderer::getSelectedObject() const {
	return *selectedObject;
}

void Renderer::setSelectedObject(Hitable& object) {
	selectedObject = &object;
}