#include "Scene.h"
#include "Sphere.h"
#include "Cube.h"

int nextObjectId = 0;

void makeSphereColumn(vector<Hitable*>& objects) {
	objects.push_back(new Sphere(Vector(0.0, 0.75, 0.0), 0.25, to_string(nextObjectId++)));
	objects.push_back(new Sphere(Vector(0.0, 0.25, 0.0), 0.25, to_string(nextObjectId++)));
	objects.push_back(new Sphere(Vector(0.0, -0.25, 0.0), 0.25, to_string(nextObjectId++)));
	objects.push_back(new Sphere(Vector(0.0, -0.75, 0.0), 0.25, to_string(nextObjectId++)));
}

void makeStacks(vector<Hitable*>& objects) {
	objects.push_back(new Cube(Vector(-0.5, -0.75, -0.5), Vector(0.5, -0.7, 0.5), to_string(nextObjectId++)));

	objects.push_back(new Cube(Vector(-0.45, -1, -0.45), Vector(-0.4, -0.45, -0.4), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(0.4, -1, -0.45), Vector(0.45, -0.45, -0.4), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(-0.45, -1, 0.4), Vector(-0.4, -0.45, 0.45), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(0.4, -1, 0.4), Vector(0.45, -0.45, 0.45), to_string(nextObjectId++)));

	objects.push_back(new Cube(Vector(-0.3, -0.5, -0.3), Vector(0.3, -0.45, 0.3), to_string(nextObjectId++)));

	objects.push_back(new Cube(Vector(-0.25, -0.7, -0.25), Vector(-0.2, -0.25, -0.2), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(0.2, -0.7, -0.25), Vector(0.25, -0.25, -0.2), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(-0.25, -0.7, 0.2), Vector(-0.2, -0.25, 0.25), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(0.2, -0.7, 0.2), Vector(0.25, -0.25, 0.25), to_string(nextObjectId++)));

	// upper level
	objects.push_back(new Cube(Vector(-0.25, -0.25, -0.25), Vector(0.25, -0.2, 0.25), to_string(nextObjectId++)));
}

void makeTableAndChair(vector<Hitable*>& objects) {
	objects.push_back(new Cube(Vector(-0.5, -0.35, -0.5), Vector(0.3, -0.3, 0.5), to_string(nextObjectId++)));

	// table legs
	objects.push_back(new Cube(Vector(-0.45, -1, -0.45), Vector(-0.4, -0.35, -0.4), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(0.2, -1, -0.45), Vector(0.25, -0.35, -0.4), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(-0.45, -1, 0.4), Vector(-0.4, -0.35, 0.45), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(0.2, -1, 0.4), Vector(0.25, -0.35, 0.45), to_string(nextObjectId++)));

	// chair seat
	objects.push_back(new Cube(Vector(0.3, -0.6, -0.2), Vector(0.7, -0.55, 0.2), to_string(nextObjectId++)));

	// chair legs
	objects.push_back(new Cube(Vector(0.3, -1, -0.2), Vector(0.35, -0.6, -0.15), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(0.3, -1, 0.15), Vector(0.35, -0.6, 0.2), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(0.65, -1, -0.2), Vector(0.7, 0.1, -0.15), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(0.65, -1, 0.15), Vector(0.7, 0.1, 0.2), to_string(nextObjectId++)));

	// chair back
	objects.push_back(new Cube(Vector(0.65, 0.05, -0.15), Vector(0.7, 0.1, 0.15), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(0.65, -0.55, -0.09), Vector(0.7, 0.1, -0.03), to_string(nextObjectId++)));
	objects.push_back(new Cube(Vector(0.65, -0.55, 0.03), Vector(0.7, 0.1, 0.09), to_string(nextObjectId++)));

	// sphere on table
	objects.push_back(new Sphere(Vector(-0.1, -0.05, 0), 0.25, to_string(nextObjectId++)));
}