#pragma once

#include <vector>

#include "raylib.h"
#include "Sensor.h"
#include "Utils.h"
#include "NeuralNetwork.h"

using namespace std;

class Car
{
public:
  bool foundPath;
  bool alive;
  float fitnessLast3sec;
  float Timer3sec;

  float x;
  float y;

  float width;
  float height;

  float angle;
  float rotationSpeed;

  float vel_x;
  float vel_y;

  float speed;
  float acceleration;
  float retardation;

  Sensor sensor;

  Vector2 origin;
  Vector2 spawn;
  float spawnAngle;

  // neuralNetwork
  NeuralNetwork brain;
  float fitness;

  Car();

  void reset();
  void Update(bool trackSet, float dt, const vector<Wall> &walls, const Wall &finishLine);
  void Draw(bool trackSet);
};