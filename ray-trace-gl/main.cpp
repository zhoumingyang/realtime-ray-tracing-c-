#include "sylvester_vector.h"
#include "sylvester_matrix.h"
#include "sylvester_line.h"
#include "sylvester_plane.h"
#include "Shader.h"
#include "Sphere.h"
#include "Cube.h"
#include "Light.h"
#include "PathTracer.h"
#include "Renderer.h"
#include <iostream>
#include <windows.h>

int nextObjectId = 0;
vector<Hitable*> objects;
Vector eye(0, 0, 2.5);
Renderer* render;
DWORD now, start;

void dislpay(float e, int i) {
	cout << e << " " << i << endl;
}

void makeSphereColumn(vector<Hitable*>& objects) {
	objects.push_back(new Sphere(Vector(0.0, 0.75, 0.0), 0.25, to_string(nextObjectId++)));
	objects.push_back(new Sphere(Vector(0.0, 0.25, 0.0), 0.25, to_string(nextObjectId++)));
	objects.push_back(new Sphere(Vector(0.0, -0.25, 0.0), 0.25, to_string(nextObjectId++)));
	objects.push_back(new Sphere(Vector(0.0, -0.75, 0.0), 0.25, to_string(nextObjectId++)));
}

Matrix makeLookAt(float ex, float ey, float ez,
	float cx, float cy, float cz,
	float ux, float uy, float yz) {
	Vector eye(ex, ey, ez);
	Vector center(cx, cy, cz);
	Vector up(ux, uy, yz);

	Vector z = eye.substract(center).toUnitVector();
	Vector x = up.cross(z).toUnitVector();
	Vector y = z.cross(x).toUnitVector();

	vector<vector<float>> M = {
		{ x.x(), x.y(), x.z(), 0.0 },
		{ y.x(), y.y(), y.z(), 0.0 },
		{ z.x(), z.y(), z.z(), 0.0 },
		{ 0.0, 0.0, 0.0, 1.0 }
	};

	vector<vector<float>> T = {
		{ 1.0, 0.0, 0.0, -ex },
		{ 0.0, 1.0, 0.0, -ey },
		{ 0.0, 0.0, 1.0, -ez },
		{ 0.0, 0.0, 0.0, 1.0 }
	};

	Matrix m(M);
	Matrix t(T);
	return m.multiply(t);
}

Matrix makeFrsutum(float left, float right, float bottom, float top, float znear, float zfar) {
	float X = 2.0 * znear / (right - left);
	float Y = 2.0 * znear / (top - bottom);
	float A = (right + left) / (right - left);
	float B = (top + bottom) / (top - bottom);
	float C = -(zfar + znear) / (zfar - znear);
	float D = -2 * zfar * znear / (zfar - znear);

	vector<vector<float>> M = {
		{ X, 0.0, A, 0.0 },
		{ 0.0, Y, B, 0.0 },
		{ 0.0, 0.0, C, D },
		{ 0.0, 0.0, -1.0, 0.0}
	};

	return Matrix(M);
}

Matrix makePerspective(float fovy, float aspect, float znear, float zfar) {
	float ymax = znear * tan(fovy * M_PI / 360.0);
	float ymin = -ymax;
	float xmin = ymin * aspect;
	float xmax = ymax * aspect;
	return makeFrsutum(xmin, xmax, ymin, ymax, znear, zfar);
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	objects.push_back(new Light());
	makeSphereColumn(objects);
	render = new Renderer();
	render->setObjects(objects);
	start = GetTickCount();
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		cout << "some thing wrong init: " << err << endl;
	}
}

void RenderScene() {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		cout << "some thing wrong in render scene: " << err << endl;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Matrix modelview = makeLookAt(eye.x(), eye.y(), eye.z(), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	Matrix projection = makePerspective(55.0, 1.0, 0.1, 100.0);
	Matrix modelviewProjection = projection.multiply(modelview);
	now = GetTickCount();
	float diff = (now - start) * 0.001;
	//diff = 8.824;
	render->update(modelviewProjection, diff);
	render->render();
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ray tracing");
	glutDisplayFunc(RenderScene);
	glutIdleFunc(RenderScene);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return 1;
	}
	init();
	glutMainLoop();
	return 0;
}