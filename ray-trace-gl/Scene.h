#pragma once
#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <iostream>
#include "hitable.h"
extern int nextObjectId;
extern void makeSphereColumn(vector<Hitable*>& objects);
extern void makeStacks(vector<Hitable*>& objects);
extern void makeTableAndChair(vector<Hitable*>& objects);
extern void makeSphereAndCube(vector<Hitable*>& objects);
extern void makeCubeAndSpheres(vector<Hitable*>& objects);
extern void makeSpherePyramid(vector<Hitable*>& objects);
#endif // !SCENE_H

