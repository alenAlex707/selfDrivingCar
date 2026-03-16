#include "Car.h"
#include <math.h>

Car::Car(float x, float y)
{
  this->x = x;
  this->y = y;

  angle = 0.0f;
  rotationSpeed = 180.0f;

  vel_x = 0.0f;
  vel_y = 0.0f;

  speed = 0.0f;
  acceleration = 1500.0f;
  retardation = -3.5f;

  width = 40;
  height = 20;
  origin = {width / 2, height / 2};
}

void Car::Update(float dt, const vector<Wall> &walls)
{
  float rad = angle * DEG2RAD;

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

  if (fabs(speed) < 5.0f)
    speed = 0;

  speed += speed * retardation * dt;

  vel_x = cos(rad) * speed;
  vel_y = sin(rad) * speed;

  x += vel_x * dt;
  y += vel_y * dt;
  sensor.UpdateVal(x, y, angle, walls);
}

void Car::Draw(bool trackSet)
{
  Rectangle rect = {x, y, width, height};
  DrawRectanglePro(rect, origin, angle, {0, 200, 220, 255});
  rect.x = x;
  rect.y = y;

  sensor.DrawSensor(trackSet);
}