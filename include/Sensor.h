#pragma once

#include "raylib.h"
#include <vector>
#include "Utils.h"

using namespace std;

class Sensor
{
public:
    float carX;
    float carY;
    float carAngle;

    float sensorLength;
    int sensorCount;
    vector<float> sensorAngles;

    vector<float> sensorValues;

    vector<Wall> walls;

    Sensor();

    void UpdateVal(float x, float y, float theta, const vector<Wall> &walls);
    void DrawSensor(bool trackSet);
};