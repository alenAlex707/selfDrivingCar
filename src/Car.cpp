#include "Car.h"
#include <math.h>
#include <Utils.h>
#include <iostream>
using namespace std;

Car::Car()
{
  foundPath = false;
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
  acceleration = 700.0f;
  retardation = -2.5f;

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
  foundPath = false;
}

void Car::Update(bool trackSet, float dt, const vector<Wall> &walls, const Wall &finishLine)
{
  if (trackSet)
  {
    Timer3sec += dt;

    float rad = angle * DEG2RAD;

    vector<float> neuralOut = brain.forward(sensor.sensorValues);

    angle += neuralOut[0] * rotationSpeed * dt;
    speed += neuralOut[1] * acceleration * dt;

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

    speed += speed * retardation * dt;

    // reward system for car being in track
    float sensorSum{};
    for (int i = 0; i < 5; i++)
    {
      sensorSum += sensor.sensorValues[i];
    }

    fitness += speed * dt + sensorSum * 0.1f;

    if (speed < 0)
    {
      speed = 0;
    }

    vel_x = cos(rad) * speed;
    vel_y = sin(rad) * speed;

    Vector2 oldPos = {x, y};
    x += vel_x * dt;
    y += vel_y * dt;
    sensor.UpdateVal(x, y, angle, walls);

    if (sensor.hasCollided())
    {
      alive = false;
    }

    bool offTrack = true;

    for (int i = 0; i < sensor.sensorValues.size(); i++)
    {

      if (sensor.sensorValues[i] < 1.0f)
      {
        offTrack = false;
      }
    }

    if (offTrack)
    {
      alive = false;
    }

    if (Timer3sec >= 3.0f)
    {
      if (fitness - fitnessLast3sec < 350.0f)
      {
        alive = false;
      }

      Timer3sec = 0;
      fitnessLast3sec = fitness;
    }

    float crossOffset = 25.0f;

    Vector2 cross = getIntersection(finishLine, oldPos, {x + cos(angle * DEG2RAD) * crossOffset, y + sin(angle * DEG2RAD) * crossOffset});
    if (cross.x != -1 && cross.y != -1)
    {
      cout << "reached finish track" << endl;
      fitness += 10000;
      foundPath = true;
    }
  }
}

void Car::Draw(bool trackSet, Color color)
{
  if (trackSet)
  {
    Rectangle rect = {x, y, width, height};
    DrawRectanglePro(rect, origin, angle, color);
  }

  sensor.DrawSensor(trackSet);
}