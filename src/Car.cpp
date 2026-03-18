#include "Car.h"
#include <math.h>
#include <iostream>
using namespace std;

Car::Car()
{
  alive = true;
  fitness = 0.0f;
  fitnessLast3sec = 0.0f;
  Timer3sec = 0.0f;

  this->x = 0;
  this->y = 0;

  angle = 0.0f;
  rotationSpeed = 180.0f;

  vel_x = 0.0f;
  vel_y = 0.0f;

  speed = 0.0f;
  acceleration = 800.0f;
  retardation = -3.5f;

  width = 40;
  height = 20;
  origin = {width / 2, height / 2};
}

void Car::reset()
{
  this->x = spawn.x;
  this->y = spawn.y;
  this->speed = 0;
  this->angle = spawnAngle;

  alive = true;
  fitness = 0;
}

void Car::Update(bool trackSet, float dt, const vector<Wall> &walls)
{
  if (trackSet)
  {
    Timer3sec += dt;

    float rad = angle * DEG2RAD;

    vector<float> neuralOut = brain.forward(sensor.sensorValues);

    angle += neuralOut[0] * rotationSpeed * dt;
    speed += ((neuralOut[1] + 1.0f) / 2.0f) * acceleration * dt;

    /*
    if (IsKeyDown(KEY_W))
      speed += acceleration * dt;
    if (IsKeyDown(KEY_A))
      angle -= rotationSpeed * dt;
    if (IsKeyDown(KEY_S))
      speed -= acceleration * dt;
    if (IsKeyDown(KEY_D))
      angle += rotationSpeed * dt;
    if (IsKeyDown(KEY_SPACE))
      speed += speed * retardation * dt * 1.4;
   */

    speed += speed * retardation * dt;
    fitness += speed * dt;

    vel_x = cos(rad) * speed;
    vel_y = sin(rad) * speed;

    x += vel_x * dt;
    y += vel_y * dt;
    sensor.UpdateVal(x, y, angle, walls);

    if (sensor.hasCollided())
    {
      alive = false;
    }

    if (Timer3sec >= 3.0f)
    {
      if (fitness - fitnessLast3sec < 500.0f)
      {
        alive = false;
      }
      Timer3sec = 0;
      fitnessLast3sec = fitness;
    }
    cout << fitness - fitnessLast3sec << endl;
  }
}

void Car::Draw(bool trackSet)
{
  if (trackSet)
  {
    Rectangle rect = {x, y, width, height};
    DrawRectanglePro(rect, origin, angle, {0, 200, 220, 255});
  }

  sensor.DrawSensor(trackSet);
}