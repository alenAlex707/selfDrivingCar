#pragma once

#include "raylib.h"
#include <vector>

using namespace std;

struct Wall
{
	Vector2 a;
	Vector2 b;
};

float getDistance(Vector2 a, Vector2 b);
Vector2 getIntersection(Wall wall, Vector2 sensorStart, Vector2 sensorEnd);
float getStartAngle(vector<Vector2> path);
