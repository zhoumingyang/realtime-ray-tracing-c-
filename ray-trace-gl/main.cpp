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
#include "Scene.h"

#define MATERIALDIFFUSE 1
#define MATERIALMIRROR 2
#define MATERIALGLOSSY 3
#define SCENECHANGE 4
#define SCENESPHERECOLUMN 1
#define SCENESTACKS 2
#define SCENETABLECHAIR 3

int material = MATERIALDIFFUSE;
vector<Hitable*> objects;
Vector eye(0, 0, 2.5);
Renderer* render;
DWORD now, start;

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
	render->update(modelviewProjection, diff);
	render->render();
	glutSwapBuffers();
	glutPostRedisplay();
}

void processSceneMenuEvents(int option) {
	if (objects.size() > 0) {
		for (int i = 0; i < objects.size(); i++) {
			delete objects[i];
		}
		objects = vector<Hitable*>();
		objects.push_back(new Light());
	}
	switch (option) {
	case SCENESPHERECOLUMN:
		makeSphereColumn(objects);
		break;
	case SCENESTACKS:
		makeStacks(objects);
		break;
	case SCENETABLECHAIR:
		makeTableAndChair(objects);
		break;
	default:
		makeSphereColumn(objects);
		break;
	}
	if (objects.size()) {
		render->setObjects(objects);
	}

}

void processMenuEvents(int option) {
	switch (option) {
	case MATERIALDIFFUSE:
		render->setMaterial(MATERIALDIFFUSE);
		break;
	case MATERIALMIRROR:
		render->setMaterial(MATERIALMIRROR);
		break;
	case MATERIALGLOSSY:
		render->setMaterial(MATERIALGLOSSY);
		break;
	}
	render->setObjects(objects);
}

void createMaterialMenus() {
	int menu, sceneSubMenu;

	sceneSubMenu = glutCreateMenu(processSceneMenuEvents);
	glutAddMenuEntry("sphere colume", SCENESPHERECOLUMN);
	glutAddMenuEntry("stacks", SCENESTACKS);
	glutAddMenuEntry("table chair", SCENETABLECHAIR);

	menu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("diffuse", MATERIALDIFFUSE);
	glutAddMenuEntry("mirror", MATERIALMIRROR);
	glutAddMenuEntry("glossy", MATERIALGLOSSY);

	glutAddSubMenu("scene", sceneSubMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void doPickObject(int x, int y) {
	float t;
	Vector origin = Vector(eye);
	Matrix tmpMatrix = render->getModelViewProjection().inverse();
	Vector ray = render->getPathTracer()->getEyeRay(tmpMatrix, (float(x) / 512.0) * 2.0 - 1.0, 1.0 - (float(y) / 512.0) * 2.0);
	Hitable* selectedObject = render->getSelectedObject();
	if (NULL != selectedObject) {
		Vector minBounds = selectedObject->getMinCorner();
		Vector maxBounds = selectedObject->getMaxCorner();
		Cube* tmpCube = new Cube();
		t = tmpCube->dointersect(origin, ray, minBounds, maxBounds);
		if (t < FLT_MAX) {
			Vector hit = origin.add(ray.multiply(t));
			return;
		}
	}
	t = FLT_MAX;
	render->setSelectedObject(NULL);
	for (int i = 0; i < objects.size(); i++) {
		float objectT = objects[i]->intersect(origin, ray);
		if (objectT < t) {
			t = objectT;
			render->setSelectedObject(objects[i]);
		}
	}
}

void processMousePickEvent(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if ((x >= 0 && x < 512) && (y >= 0 && y < 512)) {
			doPickObject(x, y);
		}
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ray tracing");
	glutDisplayFunc(RenderScene);
	glutIdleFunc(RenderScene);
	glutMouseFunc(processMousePickEvent);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return 1;
	}
	init();
	createMaterialMenus();
	glutMainLoop();
	return 0;
}