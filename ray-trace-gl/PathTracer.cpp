#include "PathTracer.h"
#include "Shader.h"

PathTracer::PathTracer() {
	//create VBO
	GLfloat vertices[] = {
		-1.0, -1.0,
		-1.0, +1.0,
		+1.0, -1.0,
		+1.0, +1.0
	};
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//create FBO
	glGenFramebuffers(1, &frameBuffer);

	GLenum err1 = glGetError();
	if (err1 != GL_NO_ERROR) {
		std::cout << "something wrong in path trace init 1: " << err1 << endl;
	}

	//create texture
	for (int i = 0; i < 2; i++) {
		glGenTextures(1, &textures[i]);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_FLOAT, NULL);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	renderProgram = compileShader(renderVertexSource, renderFragmentSource);
	renderVertexAttribute = glGetAttribLocation(renderProgram, "vertex");
	glVertexAttribPointer(renderVertexAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(renderVertexAttribute);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLenum err2 = glGetError();
	if (err2 != GL_NO_ERROR) {
		std::cout << "something wrong in path trace init 2: "<< err2 << endl;
	}
}

PathTracer::~PathTracer() {
	if (vertexBuffer != 0xffffffff) {
		glDeleteBuffers(1, &vertexBuffer);
	}
}

void PathTracer::setObjects(vector<Hitable*> _objects) {
	sampleCount = 0;
	objects = _objects;
	if (tracerProgram != NULL) {
		glDeleteProgram(tracerProgram);
	}
	std::string tmpShaderSrc = makeTracerFragmentSource(objects);
	tracerProgram = compileShader(tracerVertexSource, makeTracerFragmentSource(objects));
	tracerVertexAttribute = glGetAttribLocation(tracerProgram, "vertex");
	glVertexAttribPointer(tracerVertexAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(tracerVertexAttribute);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "something wrong in path trace set objects" << endl;
	}
}

void PathTracer::setObjects(Hitable* _objects[], int n) {
	sampleCount = 0;
	for (int i = 0; i < n; i++) {
		objects.push_back(_objects[i]);
	}
	if (tracerProgram != NULL) {
		glDeleteProgram(tracerProgram);
	}
	std::string tmp = makeTracerFragmentSource(objects);
	tracerProgram = compileShader(tracerVertexSource, makeTracerFragmentSource(objects));
	tracerVertexAttribute = glGetAttribLocation(tracerProgram, "vertex");
	glVertexAttribPointer(tracerVertexAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(tracerVertexAttribute);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "something wrong in path trace set objects" << endl;
	}
}

Vector PathTracer::getEyeRay(const Matrix& matrix, const float& x, const float& y) {
	vector<float> tmp = { x, y, 0, 1 };
	Vector re = matrix.multiply(Vector(tmp)).divideByW();
	return Vector(re.x(), re.y(), re.z()).substract(Vector(0, 0, 2.5));
}

void PathTracer::setUniforms() {
	GLuint eyeLocation = glGetUniformLocation(tracerProgram, "eye");
	GLuint glossinessLocation = glGetUniformLocation(tracerProgram, "glossiness");
	GLuint ray00Location = glGetUniformLocation(tracerProgram, "ray00");
	GLuint ray01Location = glGetUniformLocation(tracerProgram, "ray01");
	GLuint ray10Location = glGetUniformLocation(tracerProgram, "ray10");
	GLuint ray11Location = glGetUniformLocation(tracerProgram, "ray11");
	GLuint timeLocation = glGetUniformLocation(tracerProgram, "timeSinceStart");
	GLuint weightLocation = glGetUniformLocation(tracerProgram, "textureWeight");
	if (eyeLocation != 0xffffffff) {
		glUniform3f(eyeLocation, uniforms.eye.x(), uniforms.eye.y(), uniforms.eye.z());
	}
	if (glossinessLocation != 0xffffffff) {
		glUniform1f(glossinessLocation, uniforms.glossiness);
	}
	if (ray00Location != 0xffffffff) {
		glUniform3f(ray00Location, uniforms.ray00.x(), uniforms.ray00.y(), uniforms.ray00.z());
	}
	if (ray01Location != 0xffffffff) {
		glUniform3f(ray01Location, uniforms.ray01.x(), uniforms.ray01.y(), uniforms.ray01.z());
	}
	if (ray10Location != 0xffffffff) {
		glUniform3f(ray10Location, uniforms.ray10.x(), uniforms.ray10.y(), uniforms.ray10.z());
	}
	if (ray11Location != 0xffffffff) {
		glUniform3f(ray11Location, uniforms.ray11.x(), uniforms.ray11.y(), uniforms.ray11.z());
	}
	if (timeLocation != 0xffffffff) {
		glUniform1f(timeLocation, uniforms.timeSinceStar);
	}
	if (weightLocation != 0xffffffff) {
		glUniform1f(weightLocation, uniforms.textureWeight);
	}
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "something wrong in path trace set uniform: "<< err << endl;
	}
}

void PathTracer::update(const Matrix& matrix, float timeSinceStart) {
	uniforms.eye = Vector(0, 0, 2.5);
	uniforms.glossiness = 0.6;
	uniforms.ray00 = getEyeRay(matrix, -1.0, -1.0);
	uniforms.ray01 = getEyeRay(matrix, -1.0, 1.0);
	uniforms.ray10 = getEyeRay(matrix, 1.0, -1.0);
	uniforms.ray11 = getEyeRay(matrix, 1.0, 1.0);
	uniforms.timeSinceStar = timeSinceStart;
	uniforms.textureWeight = float(sampleCount) / (sampleCount + 1.0);
	glUseProgram(tracerProgram);
	setUniforms();
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->setUniforms(this);
	}
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[1], 0);//level is 0

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
	}
	glEnableVertexAttribArray(tracerVertexAttribute);
	glVertexAttribPointer(tracerVertexAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(tracerVertexAttribute);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tempTexture = textures[0];
	textures[0] = textures[1];
	textures[1] = tempTexture;
	sampleCount++;

	GLenum err2 = glGetError();
	if (err2 != GL_NO_ERROR) {
		std::cout << "something wrong in trace update 2: "<< err2 << endl;
	}
}

void PathTracer::render() {
	glUseProgram(renderProgram);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(tracerVertexAttribute);
	glVertexAttribPointer(tracerVertexAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(tracerVertexAttribute);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint PathTracer::getVertexBuffer() const {
	return vertexBuffer;
}

GLuint PathTracer::getFrameBuffer() const {
	return frameBuffer;
}

GLuint PathTracer::getRenderProgram() const {
	return renderProgram;
}

GLuint PathTracer::getTracerProgram() const {
	return tracerProgram;
}

GLuint PathTracer::getRenderVertexAttribute() const {
	return renderVertexAttribute;
}

GLuint PathTracer::getTracerVertexAttribute() const {
	return tracerVertexAttribute;
}

int PathTracer::getSampleCount() const {
	return sampleCount;
}