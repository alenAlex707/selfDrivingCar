#pragma once

#include "raylib.h"
#include <vector>
#include "Sensor.h"
#include "NeuralNetwork.h"

using namespace std;

class Car
{
public:
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

  // neuralNetwork
  NeuralNetwork brain;
  float fitness;

  Car(float x, float y);

  void Update(float dt, const vector<Wall> &walls);
  void Draw(bool trackSet);
};